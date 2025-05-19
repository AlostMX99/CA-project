#include <stdio.h>
#include "controlHazards.h"
#include "Decoder.h"
#include "registers.h"

// Use the global decoder or pass it if needed


// Enum-like macros for clarity
#define OPCODE_BNE 4
#define OPCODE_J   7

// Function to check if the instruction is a control-type (branch/jump)
int isControlHazard(int opcode) {
    return (opcode == OPCODE_BNE || opcode == OPCODE_J);
}

// Branch condition logic
int shouldTakeBranch(int instruction, int pc, int* targetPC) {
    Decode(instruction);

    if (decoder.opcode == OPCODE_BNE) {
        int rsVal = getRegister(decoder.RS - 1)->data;
        int rtVal = getRegister(decoder.RT - 1)->data;
        if (rsVal != rtVal) {
            *targetPC = pc + 1 + decoder.IMM;
            return 1;
        }
    } else if (decoder.opcode == OPCODE_J) {
        *targetPC = decoder.address;
        return 1;
    }
    return 0;
}

// Flush a pipeline stage
void flushStage(PipelineStage* stage) {
    stage->active = 0;
    stage->flushed = 1;
    stage->instruction = 0;
    stage->pcAtDecode = -1;
    printf("Flushed stage containing instruction %d\n", stage->instruction);
}

// Flush the pipeline from IF and ID stages
void flushControlHazard(PipelineStage* IF, PipelineStage* ID) {
    printf("Control hazard detected. Flushing IF and ID stages...\n");
    flushStage(IF);
    flushStage(ID);
}