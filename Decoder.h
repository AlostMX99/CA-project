#ifndef Decoder
#define Decoder
struct Decoder
{
    int type; //0 -> R, 1 -> I, 2 -> J.
    int data;
    int opcode;
    int RD;
    int RS;
    int SHAMT;
    int IMM;
    int address;
}Decoder;
Decode(int x);
#endif  