#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include "registers.h"
#include "memory.h"
#include "Parser.h"

#define IF 0
#define ID 1
#define EX 2
#define MEM 3
#define WB 4

#define OPCODE_ADD 0
#define OPCODE_ADDI 7
#define OPCODE_LW 8
#define OPCODE_SW 9
#define OPCODE_BNE 10
#define OPCODE_J 11

extern registerHome myRegisters;  // Assuming this is declared in registers.h
extern int parseFile(const char* filename);  // Prevent implicit declaration

int flush_timer = 0;
int clock_cycle = 1;
PipelineStage pipeline[5] = {0};

int sign_extend(int value, int bits) {
    int mask = 1 << (bits - 1);
    return (value ^ mask) - mask;
}

void fetch() {
    if (flush_timer > 0 || clock_cycle % 2 == 0) return;
    int pc = getPC()->data;
    pipeline[IF].instruction = FetchIndex(pc);
    pipeline[IF].valid = 1;
    incrementPC();
}

void decode() {
    if (!pipeline[ID].valid) return;
    int instr = pipeline[ID].instruction;
    int opcode = (instr >> 28) & 0xF;
    pipeline[ID].opcode = opcode;
if (pipeline[EX].valid) {
    int prevOpcode = pipeline[EX].opcode;
    int prevDest = pipeline[EX].R1;

    if (prevOpcode != OPCODE_BNE && prevOpcode != OPCODE_J && prevOpcode != OPCODE_SW) {
        if (pipeline[ID].R1 == prevDest) {
            printf("Forwarding: R1 = R%d using ALU result %d from EX stage\n", prevDest, pipeline[EX].result);
            // simulate forward: optionally store into register or temporary variable
            myRegisters.regHome[pipeline[ID].R1].data = pipeline[EX].result;
        }
        if (pipeline[ID].R2 == prevDest) {
            printf("Forwarding: R2 = R%d using ALU result %d from EX stage\n", prevDest, pipeline[EX].result);
            myRegisters.regHome[pipeline[ID].R2].data = pipeline[EX].result;
        }
    }
}

    switch (opcode) {
        case OPCODE_ADD:
            pipeline[ID].R1 = (instr >> 23) & 0x1F;
            pipeline[ID].R2 = (instr >> 18) & 0x1F;
            pipeline[ID].R3 = (instr >> 13) & 0x1F;
            break;
        case OPCODE_ADDI:
        case OPCODE_LW:
        case OPCODE_SW:
        case OPCODE_BNE:
            pipeline[ID].R1 = (instr >> 23) & 0x1F;
            pipeline[ID].R2 = (instr >> 18) & 0x1F;
            pipeline[ID].immediate = sign_extend(instr & 0x3FFFF, 18);
            break;
        case OPCODE_J:
            pipeline[ID].address = instr & 0x0FFFFFFF;
            break;
    }

}

void execute() {
    if (!pipeline[EX].valid) return;
    int op = pipeline[EX].opcode;

    switch (op) {
        case OPCODE_ADD:
            pipeline[EX].result = getData(&myRegisters.regHome[pipeline[EX].R2]) +
                                  getData(&myRegisters.regHome[pipeline[EX].R3]);
            break;
        case OPCODE_ADDI:
            pipeline[EX].result = getData(&myRegisters.regHome[pipeline[EX].R2]) + pipeline[EX].immediate;
            break;
        case OPCODE_LW:
        case OPCODE_SW:
            pipeline[EX].mem_address = getData(&myRegisters.regHome[pipeline[EX].R2]) + pipeline[EX].immediate;
            pipeline[EX].mem_data = getData(&myRegisters.regHome[pipeline[EX].R1]);
            break;
        case OPCODE_BNE: {
            int a = getData(&myRegisters.regHome[pipeline[EX].R1]);
            int b = getData(&myRegisters.regHome[pipeline[EX].R2]);
            if (a != b) {
                int new_pc = getPC()->data + pipeline[EX].immediate;
                overwriteData(getPC(), new_pc);
                pipeline[IF].valid = 0;
                pipeline[ID].valid = 0;
                flush_timer = 2;
            }
            break;
        }
        case OPCODE_J:
            overwriteData(getPC(), pipeline[EX].address);
            pipeline[IF].valid = 0;
            pipeline[ID].valid = 0;
            flush_timer = 2;
            break;
    }
}

void memory_access() {
    if (!pipeline[MEM].valid) return;
    int addr = pipeline[MEM].mem_address;
    switch (pipeline[MEM].opcode) {
        case OPCODE_LW:
            pipeline[MEM].result = FetchIndex(addr);
            break;
        case OPCODE_SW:
            overwrite(pipeline[MEM].mem_data, addr);
            printf("Memory Update: MEM[%d] = %d (stored in MEM stage)\n", addr, pipeline[MEM].mem_data);
            break;
    }
}

void writeback() {
    if (!pipeline[WB].valid) return;
    int dest = pipeline[WB].R1;
    if ((pipeline[WB].opcode == OPCODE_ADD || pipeline[WB].opcode == OPCODE_ADDI || pipeline[WB].opcode == OPCODE_LW) && dest != 0) {
        overwriteData(&myRegisters.regHome[dest], pipeline[WB].result);
        printf("Register Update: %s = %d (updated in WB stage)\n",
               getType(&myRegisters.regHome[dest]), getData(&myRegisters.regHome[dest]));
    }
}

void advance_pipeline() {
    int insertBubble = 0;
    if (pipeline[ID].valid && pipeline[EX].valid) {
        int ex_opcode = pipeline[EX].opcode;
        int id_r1 = pipeline[ID].R1;
        int id_r2 = pipeline[ID].R2;

        if (ex_opcode == OPCODE_LW) {
            int ex_dest = pipeline[EX].R1; // LW writes to R1
            if (ex_dest == id_r1 || ex_dest == id_r2) {
                insertBubble = 1;
                printf("STALL: Load-use hazard detected → inserting bubble\n");
            }
        }
    }
    pipeline[WB] = pipeline[MEM];
    pipeline[MEM] = pipeline[EX];
    if (insertBubble) {
        memset(&pipeline[EX], 0, sizeof(PipelineStage)); // insert NOP (bubble)
    } else {
        pipeline[EX] = pipeline[ID];
    }
    pipeline[ID] = pipeline[IF];
    memset(&pipeline[IF], 0, sizeof(PipelineStage));
    if (flush_timer > 0) flush_timer--;
}

void print_pipeline_state() {
    printf("\n===== Clock Cycle %d =====\n", clock_cycle);
    const char* stage_names[5] = {"IF", "ID", "EX", "MEM", "WB"};
    for (int i = 4; i >= 0; i--) {
        if (!pipeline[i].valid) {
            printf("%s: [EMPTY]\n", stage_names[i]);
        } else {
            printf("%s: Instr=0x%08X, Opcode=%d, R1=%d, R2=%d, R3=%d, Imm=%d, Addr=%d\n",
                stage_names[i],
                pipeline[i].instruction,
                pipeline[i].opcode,
                pipeline[i].R1,
                pipeline[i].R2,
                pipeline[i].R3,
                pipeline[i].immediate,
                pipeline[i].address
            );
        }
    }
    printf("---------------------------\n");
}

void print_final_state() {
    printf("\n===== FINAL REGISTER STATE =====\n");
    for (int i = 0; i < 33; i++) {
        printf("%s = %d\n", getType(&myRegisters.regHome[i]), getData(&myRegisters.regHome[i]));
    }

    printf("\n===== FINAL MEMORY CONTENT =====\n");
    for (int i = 0; i < 2048; i++) {
        if (memory[i].used || strcmp(memory[i].type, "Data") == 0) {
            printf("MEM[%d] (%s) = %d\n", i, memory[i].type, memory[i].data);
        }
    }
}

int is_pipeline_empty() {
    for (int i = 0; i < 5; i++) {
        if (pipeline[i].valid) return 0;
    }
    return 1;
}

void simulate() {
    while (1) {
        writeback();
        memory_access();
        execute();
        decode();
        fetch();
        print_pipeline_state();
        advance_pipeline();
        clock_cycle++;
        if (is_pipeline_empty() && getPC()->data >= 1024) break;
    }
    print_final_state();
}
int main() {
    parseFile("Program1.txt");
    simulate();
    return 0;
}