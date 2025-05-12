#ifndef M2_DECODER
#define M2_DECODER
struct
{
    int type; //0 -> R, 1 -> I, 2 -> J.
    int data;
    int opcode;
    int RD;
    int RS;
    int RT;
    int SHAMT;
    int IMM;
    int address;
}Decoder;

void fetchInstruction(int opcode){
    if( opcode== 0 || opcode == 1 || opcode == 8 || opcode == 9 ){
        type = 0;
    }
    else if (opcode == 7){
        type = 2;
    }
    else{
        type = 1;
    }
}
void fetch(int opcode){
    if(type == 0){
        R1 = (data>>23)&0b1111;     
        R2 = (data>>18)&0b1111;      
        R3 = (data>>13)&0b1111;      
        SHAMT = (data>>12)&0b1111;   
    }
    else if(type == 1){
        R1 = (data>>23)&0b1111;      // bits25:21
        R2 = (datat>>18)&0b1111;
        imm = data&0b11111111111111111111;     // bits15:0
    }
    else{
        address = data&0b1111111111111111111111111111; // bits28:0
    }
}
void Decode(int x){
    data = x;
    opcode = (data >> 28)&0b00000000000000000000000000001111;
    fetchInstruction(opcode);
    fetch(opcode);
};
#endif