#include <stdio.h>
#include "ALU.h"
#include "Decoder.h"
#include "Memory.h"
#include "Parser.h"
#include "registers.h"
typedef struct {
    int instruction; 
    int active;      
} PipelineStage;

struct Decoder decoder;
PipelineStage IF_stage, ID_stage, EX_stage, MEM_stage, WB_stage;

void execution(){
    CreateMemory();
    initRegisterHome(&myRegisters);
    int instructionCount = parseFile("Program1.txt");

    int clock = 1;
    int done = 0;
    
    PipelineStage IF = {0, 0}, ID = {0, 0}, EX = {0, 0}, MEM = {0, 0}, WB = {0, 0};

    while (!done) {
        printf("Clock Cycle %d:\n", clock);

        if (WB.active) {
            Decode(WB.instruction);
            overwriteData(getRegister(decoder.RD-1),decoder.data);
            printf("WB Stage: instruction %d\n", WB.instruction);
        }

        if (MEM.active) {
            Decode(MEM.instruction);
            printf("MEM Stage: instruction %d\n", MEM.instruction);
        }

        if (EX.active) {
            Decode(EX.instruction);
            ALU(decoder.opcode, decoder.RD, decoder.RS, decoder.RT,
                decoder.SHAMT, decoder.IMM, decoder.address);
            printf("EX Stage: instruction %d\n", EX.instruction);
        }

        // 2. Decode Stage
        if (ID.active) {
            Decode(ID.instruction);
            printf("ID Stage: instruction %d\n", ID.instruction);
        }
        if (clock % 2 == 1 && memory[getPC()->data].used == 1) {
            IF.instruction = memory[getPC()->data].data;
            IF.active = 1;
            printf("IF Stage: fetched instruction %d from PC = %d\n", IF.instruction, getPC()->data);
            incrementPC();
        } else {
            IF.active = 0;
        }
        WB = MEM;
        MEM = EX;
        EX = ID;
        ID = IF;

        if ((getPC()->data >= instructionCount && !IF.active && !ID.active && !EX.active && !MEM.active && !WB.active)) {
            done = 1;
        }

        clock++;
    }

    printf("Execution complete in %d clock cycles\n", clock - 1);
}
