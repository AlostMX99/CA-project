#ifndef CONTROL_HAZARDS_H
#define CONTROL_HAZARDS_H

#include "Decoder.h"

typedef struct {
    int instruction;
    int active;
    int flushed;
    int pcAtDecode;
} PipelineStage;

int isControlHazard(int opcode);
int shouldTakeBranch(int instruction, int pc, int* targetPC);
void flushStage(PipelineStage* stage);
void flushControlHazard(PipelineStage* IF, PipelineStage* ID);

#endif
