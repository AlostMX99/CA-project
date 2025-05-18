#include <stdio.h>
#include <stdint.h>
#include "decoder.h"
#include "registers.h"
#include "memory.c"

extern struct Slot memory[2048];
extern int registers[32];

int ALU() {
    reg* pc = getPC();
    int result = 0;
    int overflow = 0;

    switch (decoder.opcode) {
        case 0:
            result = registers[decoder.RS] + registers[decoder.RT];
            overflow = (((registers[decoder.RS] ^ result) & (registers[decoder.RT] ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in ADD\n");
            registers[decoder.RD] = result;
            return result;

        case 1:
            result = registers[decoder.RS] - registers[decoder.RT];
            overflow = (((registers[decoder.RS] ^ registers[decoder.RT]) & (registers[decoder.RS] ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in SUB\n");
            registers[decoder.RD] = result;
            return result;

        case 2:
            result = registers[decoder.RS] * decoder.IMM;
            if ((registers[decoder.RS] != 0) && (result / registers[decoder.RS] != decoder.IMM)) {
                printf("Signed Overflow Detected in MULI\n");
            }
            registers[decoder.RD] = result;
            return result;

        case 3:
            result = registers[decoder.RS] + decoder.IMM;
            overflow = (((registers[decoder.RS] ^ result) & (decoder.IMM ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in ADDI\n");
            registers[decoder.RD] = result;
            return result;

        case 4:
            if (registers[decoder.RD] != registers[decoder.RS])
                pc->data += 1 + decoder.IMM;
            return -1;

        case 5:
            registers[decoder.RD] = registers[decoder.RS] & decoder.IMM;
            return registers[decoder.RD];

        case 6:
            registers[decoder.RD] = registers[decoder.RS] | decoder.IMM;
            return registers[decoder.RD];

        case 7:
            pc->data = (pc->data & 0xF0000000) | decoder.address;
            return -1;

        case 8:
            registers[decoder.RD] = registers[decoder.RS] << (decoder.SHAMT & 0x1F);
            return registers[decoder.RD];

        case 9:
            registers[decoder.RD] = (unsigned int)registers[decoder.RS] >> (decoder.SHAMT & 0x1F);
            return registers[decoder.RD];

        case 10:
            registers[decoder.RD] = memory[registers[decoder.RS] + decoder.IMM].data;
            return registers[decoder.RD];

        case 11:
            memory[registers[decoder.RS] + decoder.IMM].data = registers[decoder.RD];
            return -1;

        default:
            printf("ALU ERROR: Unknown opcode %d\n", decoder.opcode);
            return 0;
    }
}
