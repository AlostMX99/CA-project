#include <stdio.h>
#include <stdint.h>
#include "decoder.h"
#include "registers.h"
#include "memory.c"

extern struct Slot memory[2048];
extern int registers[32];

int ALU(int opcode, int r1, int r2, int r3, int shamt, int imm, int address) {

    reg* pc = getPC();

    switch (opcode) {
        case 0: 
            return registers[r1] = registers[r2] + registers[r3];

        case 1: 
            return registers[r1] = registers[r2] - registers[r3];

        case 2: 
            return registers[r1] = registers[r2] * imm;

        case 3: 
            return registers[r1] = registers[r2] + imm;

        case 4:
            if (registers[r1] != registers[r2])
                pc->data += 1 + imm;
            return -1;

        case 5:
            return registers[r1] = registers[r2] & imm;

        case 6:
            return registers[r1] = registers[r2] | imm;

        case 7:
            pc->data = (pc->data & 0xF0000000) | address;
            return -1;

        case 8: 
            return registers[r1] = registers[r2] << shamt;

        case 9: 
            return registers[r1] = (unsigned int)registers[r2] >> shamt;

        case 10: 
            return registers[r1] = memory[registers[r2] + imm].data;

        case 11:
            memory[registers[r2] + imm].data = registers[r1];
            return -1;

        default:
            printf("ALU ERROR: Unknown opcode %d\n", opcode);
            return 0;
    }
}