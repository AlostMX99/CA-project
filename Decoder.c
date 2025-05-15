#ifndef M2_DECODER
#define M2_DECODER

struct {
    int type;    // 0 -> R, 1 -> I, 2 -> J
    int data;
    int opcode;
    int RD;
    int RS;
    int RT;
    int SHAMT;
    int IMM;
    int address;
} Decoder;

void fetchInstruction(int opcode) {
    if (opcode == 0 || opcode == 1 || opcode == 8 || opcode == 9) {
        Decoder.type = 0;
    } else if (opcode == 7) {
        Decoder.type = 2;
    } else {
        Decoder.type = 1;
    }
}

void fetch(int opcode) {
    if (Decoder.type == 0) {
        Decoder.RD = (Decoder.data >> 23) & 0x0F;
        Decoder.RS = (Decoder.data >> 18) & 0x0F;
        Decoder.RT = (Decoder.data >> 13) & 0x0F;
        Decoder.SHAMT = (Decoder.data >> 12) & 0x0F;
    } else if (Decoder.type == 1) {
        Decoder.RS = (Decoder.data >> 23) & 0x0F;  // bits 25:21
        Decoder.RT = (Decoder.data >> 18) & 0x0F;  // bits 20:16
        Decoder.IMM = Decoder.data & 0x3FFFF;      // bits 17:0 (assumed 18 bits)
    } else {
        Decoder.address = Decoder.data & 0x1FFFFFFF; // bits 28:0
    }
}

void Decode(int x) {
    Decoder.data = x;
    Decoder.opcode = (Decoder.data >> 28) & 0x0F;
    fetchInstruction(Decoder.opcode);
    fetch(Decoder.opcode);
}

#endif
