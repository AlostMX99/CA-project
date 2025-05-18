#ifndef M2_DECODER
#define M2_DECODER

#include "Decoder.h"

struct Decoder decoder;

void fetchInstruction(int opcode) {
    if (opcode == 0 || opcode == 1 || opcode == 8 || opcode == 9) {
        decoder.type = 0;
    } else if (opcode == 7) {
        decoder.type = 2;
    } else {
        decoder.type = 1;
    }
}

void fetch(int opcode) {
    if (decoder.type == 0) {
        decoder.RD = (decoder.data >> 23) & 0x0F;
        decoder.RS = (decoder.data >> 18) & 0x0F;
        decoder.RT = (decoder.data >> 13) & 0x0F;
        decoder.SHAMT = (decoder.data >> 12) & 0x0F;
    } else if (decoder.type == 1) {
        decoder.RS = (decoder.data >> 23) & 0x0F;  // bits 25:21
        decoder.RT = (decoder.data >> 18) & 0x0F;  // bits 20:16
        decoder.IMM = decoder.data & 0x3FFFF;      // bits 17:0 (assumed 18 bits)
    } else {
        decoder.address = decoder.data & 0x1FFFFFFF; // bits 28:0
    }
}

void Decode(int x) {
    decoder.data = x;
    decoder.opcode = (decoder.data >> 28) & 0x0F;
    fetchInstruction(decoder.opcode);
    fetch(decoder.opcode);
}

#endif
