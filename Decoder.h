#ifndef DECODER_H
#define DECODER_H

#include "ALU.h"

struct Decoder {
    int type;    // 0 = R, 1 = I, 2 = J
    int data;
    int opcode;
    int RD;
    int RS;
    int RT;
    int SHAMT;
    int IMM;
    int address;
};
extern struct Decoder decoder;
void Decode(int x);

#endif
