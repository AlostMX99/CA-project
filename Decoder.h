#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

typedef struct {
    int type, data, opcode, RS, RT, RD, IMM, SHAMT, address;
} Decoder;

// Global decoder instance (declare extern here, define in one .c file)
extern Decoder decoder;

// Function prototypes
void fetchInstruction(int opcode);
void fetch(int opcode);
void Decode(int instruction);

#endif // DECODER_H
