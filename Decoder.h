#ifndef M2_DECODER_H
#define M2_DECODER_H


struct DecoderStruct {
    int type;     // 0 -> R, 1 -> I, 2 -> J
    int data;
    int opcode;
    int RD;
    int RS;
    int RT;
    int SHAMT;
    int IMM;
    int address;
};

// Global instance of the decoder
extern struct DecoderStruct Decoder;

// Function declarations
void fetchInstruction(int opcode);
void fetch(int opcode);
void Decode(int x);

#endif // M2_DECODER_H
