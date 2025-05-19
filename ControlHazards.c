#include <stdio.h>
#include "ALU.h"
#include "Decoder.h"
#include "Memory.h"
#include "Parser.h"
#include "registers.h"
#include "Memory.c"
typedef struct {
    int instruction;
    int active;
    int flushed;
    int pcAtDecode;
} PipelineStage;

struct Decoder decoder;
PipelineStage IF, ID, EX, MEM, WB;
int prevRD = -1;
int prevALUResult = 0;
int prevOpcode = -1;

void flushStage(PipelineStage* stage) {
    stage->active = 0;
    stage->flushed = 1;
    stage->instruction = 0;
}

int isBranchOrJump(int opcode) {
    return (opcode == 4 || opcode == 7); // BNE or J
}

void execution() {
    CreateMemory();
    initRegisterHome(&myRegisters);
    int instructionCount = parseFile("Program1.txt");

    int clock = 1;
    int done = 0;

    IF = (PipelineStage){0, 0, 0, 0};
    ID = EX = MEM = WB = IF;

    int branchPending = 0;
    int branchTarget = -1;

    while (!done) {
        printf("Clock Cycle %d:\n", clock);

        if (WB.active && !WB.flushed) {
            Decode(WB.instruction);
            overwriteData(getRegister(decoder.RD - 1), decoder.data);
            printf("WB Stage: instruction %d\n", WB.instruction);
        }

        if (MEM.active && !MEM.flushed) {
            Decode(MEM.instruction);
            printf("MEM Stage: instruction %d\n", MEM.instruction);
        }

        if (EX.active && !EX.flushed) {
    Decode(EX.instruction);
    printf("EX Stage: instruction %d\n", EX.instruction);

    if (decoder.opcode == 4) { // BNE
        int rs = getRegister(decoder.RS - 1)->data;
        int rt = getRegister(decoder.RT - 1)->data;
        if (rs != rt) {
            branchPending = 1;
            branchTarget = EX.pcAtDecode + 1 + decoder.IMM;
        }
    } else if (decoder.opcode == 7) { // J
        branchPending = 1;
        branchTarget = decoder.address;
    }

  
    int result = ALU();
    
    prevRD = decoder.RD;            
    prevOpcode = decoder.opcode;     
    prevALUResult = result;
}


      if (ID.active && !ID.flushed) {
    Decode(ID.instruction);

    
    if (prevOpcode != 4 && prevOpcode != 7 && prevOpcode != 11 && prevRD != -1) {
        
        if (prevOpcode == 0 || prevOpcode == 1 || prevOpcode == 8 || prevOpcode == 9) {
            if (decoder.RS == prevRD) {
                printf("Forwarding: RS = R%d using ALU result %d from previous EX\n", decoder.RS, prevALUResult);
                getRegister(decoder.RS - 1)->data = prevALUResult;
            }
            if (decoder.RT == prevRD) {
                printf("Forwarding: RT = R%d using ALU result %d from previous EX\n", decoder.RT, prevALUResult);
                getRegister(decoder.RT - 1)->data = prevALUResult;
            }
        }

       
        if (prevOpcode == 2 || prevOpcode == 3 || prevOpcode == 5 || prevOpcode == 6 || prevOpcode == 10) {
            if (decoder.RS == prevRD) {
                printf("Forwarding: RS = R%d using ALU result %d from previous EX\n", decoder.RS, prevALUResult);
                getRegister(decoder.RS - 1)->data = prevALUResult;
            }
        }
    }

    printf("ID Stage: instruction %d\n", ID.instruction);
    ID.pcAtDecode = getPC()->data - 1;
}


        if (clock % 2 == 1 && memory[getPC()->data].used == 1) {
            IF.instruction = memory[getPC()->data].data;
            IF.active = 1;
            IF.flushed = 0;
            printf("IF Stage: fetched instruction %d from PC = %d\n", IF.instruction, getPC()->data);
            incrementPC();
        } else {
            IF.active = 0;
        }

        if (branchPending) {
            flushStage(&IF);
            flushStage(&ID);
            getPC()->data = branchTarget;
            branchPending = 0;
            printf("Branch taken. PC updated to %d. Pipeline flushed.\n", branchTarget);
        }

        WB = MEM;
        MEM = EX;
        EX = ID;
        ID = IF;

        if (getPC()->data >= instructionCount && !IF.active && !ID.active && !EX.active && !MEM.active && !WB.active) {
            done = 1;
        }

        clock++;
    }

    printf("Execution complete in %d clock cycles\n", clock - 1);
}