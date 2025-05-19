#include <stdio.h>
#include <stdint.h>
#include "decoder.h"
#include "ALU.h"
#include "registers.h"
#include "Memory.h"
Decoder decoder;
int ALU() {
    reg* pc = getPC();
    int result = 0;
    int overflow = 0;

    // Helper macro to get register data
    #define REG_DATA(i) getData(getRegister(i))
    #define SET_REG_DATA(i, val) overwriteData(getRegister(i), val)

    switch (decoder.opcode) {
        case 0: // ADD
            result = REG_DATA(decoder.RS) + REG_DATA(decoder.RT);
            overflow = (((REG_DATA(decoder.RS) ^ result) & (REG_DATA(decoder.RT) ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in ADD\n");
            SET_REG_DATA(decoder.RD, result);
            return result;

        case 1: // SUB
            result = REG_DATA(decoder.RS) - REG_DATA(decoder.RT);
            overflow = (((REG_DATA(decoder.RS) ^ REG_DATA(decoder.RT)) & (REG_DATA(decoder.RS) ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in SUB\n");
            SET_REG_DATA(decoder.RD, result);
            return result;

        case 2: // MULI
            result = REG_DATA(decoder.RS) * decoder.IMM;
            if ((REG_DATA(decoder.RS) != 0) && (result / REG_DATA(decoder.RS) != decoder.IMM)) {
                printf("Signed Overflow Detected in MULI\n");
            }
            SET_REG_DATA(decoder.RD, result);
            return result;

        case 3: // ADDI
            result = REG_DATA(decoder.RS) + decoder.IMM;
            overflow = (((REG_DATA(decoder.RS) ^ result) & (decoder.IMM ^ result)) >> 31) & 1;
            if (overflow) printf("Signed Overflow Detected in ADDI\n");
            SET_REG_DATA(decoder.RD, result);
            return result;

        case 4: // Branch not equal
            if (REG_DATA(decoder.RD) != REG_DATA(decoder.RS))
                pc->data += 1 + decoder.IMM;
            return -1;

        case 5: // ANDI
            SET_REG_DATA(decoder.RD, REG_DATA(decoder.RS) & decoder.IMM);
            return REG_DATA(decoder.RD);

        case 6: // ORI
            SET_REG_DATA(decoder.RD, REG_DATA(decoder.RS) | decoder.IMM);
            return REG_DATA(decoder.RD);

        case 7: // Jump
            pc->data = (pc->data & 0xF0000000) | decoder.address;
            return -1;

        case 8: // Shift left logical
            SET_REG_DATA(decoder.RD, REG_DATA(decoder.RS) << (decoder.SHAMT & 0x1F));
            return REG_DATA(decoder.RD);

        case 9: // Shift right logical
            SET_REG_DATA(decoder.RD, (unsigned int)REG_DATA(decoder.RS) >> (decoder.SHAMT & 0x1F));
            return REG_DATA(decoder.RD);

        case 10: // Load word
            SET_REG_DATA(decoder.RD, getData(&memory[REG_DATA(decoder.RS) + decoder.IMM]));
            return REG_DATA(decoder.RD);

        case 11: // Store word
            overwriteData(&memory[REG_DATA(decoder.RS) + decoder.IMM], REG_DATA(decoder.RD));
            return -1;

        default:
            printf("ALU ERROR: Unknown opcode %d\n", decoder.opcode);
            return 0;
    }

    #undef REG_DATA
    #undef SET_REG_DATA
}
