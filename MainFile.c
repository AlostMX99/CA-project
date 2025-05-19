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
struct ALU A1;
PipelineStage IF_stage, ID_stage, EX_stage, MEM_stage, WB_stage;

void execution(){
    CreateMemory();
    initRegisterHome(&myRegisters);
    int instructionCount = parseFile("Program1.txt");

    int clock = 0;
    int IDTTL = 0;
    int EXTTL = 0;
    int done = 0;
    int CurrInst;
    int result;
    
    PipelineStage IF = {0, 0}, ID = {0, 0}, EX = {0, 0}, MEM = {0, 0}, WB = {0, 0};

    while (!done) {
        printf("Clock Cycle %d:\n", clock);
        if (IF.active){
            MEM.active = 0;
            CurrInst = FetchIndex(getPC);
            MEM.active = 1;
            ID.active =1;
            IF.active = 0;
        }
        if (ID.active && IDTTL == 2) {
            Decode(CurrInst);
            IDTTL++;
        }
        else{
            IDTTL = 0;
            ID.active = 0;
            EX.active = 1;
        }

        if (EX.active && EXTTL == 2) {
            result = ALU();
            EXTTL++;
        }
        else{
            EX.active = 0;
            EXTTL = 0;
            if (!IF.active)
                MEM.active = 1;
        }


        if (MEM.active) {
           if (decoder.opcode == 10 ){
            int temp = FetchIndex(decoder.RS + decoder.RT);
            reg* tm = getRegister(decoder.RD);
            tm->data = temp;
           }
           else if (decoder.opcode == 11){
                int temp = FetchIndex(decoder.RS + decoder.RT);
                overwrite(decoder.RD,temp);
           }
            MEM.active = 0;
            WB.active = 1;
        }

        if (WB.active) {
            if(decoder.opcode != 4 || decoder.opcode != 7 || decoder.opcode != 11 ){
            overwrite(result,decoder.RD);
            }
            WB.active = 0;
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
