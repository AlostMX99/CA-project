#ifndef M2_DECODER_H
#define M2_DECODER_H

#include "Decoder.h"



void fetchInstruction(int opcode) {
    if (opcode == 0 || opcode == 1 || opcode == 8 || opcode == 9) {
        decoder.type = 0;  // R-type
    } else if (opcode == 7) {
        decoder.type = 2;  // J-type
    } else {
        decoder.type = 1;  // I-type
    }
}

void fetch(int opcode) {
    if (decoder.type == 0) {
        // R-type fields: assuming 4 bits for each register field and shift amount
        decoder.RD = (decoder.data >> 23) & 0x0F;    // bits 26:23
        decoder.RS = (decoder.data >> 18) & 0x0F;    // bits 21:18
        decoder.RT = (decoder.data >> 13) & 0x0F;    // bits 16:13
        decoder.SHAMT = (decoder.data >> 9) & 0x0F;  // bits 12:9
    } else if (decoder.type == 1) {
        // I-type fields:
        decoder.RS = (decoder.data >> 23) & 0x0F;    // bits 26:23
        decoder.RT = (decoder.data >> 18) & 0x0F;    // bits 21:18
        decoder.IMM = decoder.data & 0x3FFFF;        // bits 17:0 (18 bits)

        // Sign-extend IMM (if needed)
        if (decoder.IMM & (1 << 17)) { // if sign bit set
            decoder.IMM |= ~0x3FFFF;   // extend sign bits
        }
    } else {
        // J-type field:
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
