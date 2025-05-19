#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

typedef struct {
    uint32_t data;
    uint8_t opcode;
    uint8_t type;
    uint8_t RD;
    uint8_t RS;
    uint8_t RT;
    uint8_t SHAMT;
    int32_t IMM;       // signed immediate
    uint32_t address;
} Decoder;

// Global decoder instance (declare extern here, define in one .c file)
extern Decoder decoder;

// Function prototypes
void fetchInstruction(int opcode);
void fetch(int opcode);
void Decode(int instruction);

#endif // DECODER_H
