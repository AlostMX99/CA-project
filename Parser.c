#include "Parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Memory.h"


static int instructionCount = 0;

int getOpcode(char* op) {
    if (strcmp(op, "ADD") == 0) return 0;
    if (strcmp(op, "SUB") == 0) return 1;
    if (strcmp(op, "MULI") == 0) return 2;
    if (strcmp(op, "ADDI") == 0) return 3;
    if (strcmp(op, "BNE") == 0) return 4;
    if (strcmp(op, "ANDI") == 0) return 5;
    if (strcmp(op, "ORI") == 0) return 6;
    if (strcmp(op, "J") == 0) return 7;
    if (strcmp(op, "SLL") == 0) return 8;
    if (strcmp(op, "SRL") == 0) return 9;
    if (strcmp(op, "LW") == 0) return 10;
    if (strcmp(op, "SW") == 0) return 11;
    return -1;
}

int regToInt(char* reg) {
    if (strcmp(reg, "R0") == 0) return 0;
    int val = atoi(reg + 1);
    return val;
}

int parseFile(const char* fileName) {

    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("fopen failed");
        return -1;
    }


    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (instructionCount >= 1024) {
            printf("Instruction memory full.\n");
            break;
        }

        char* op = strtok(line, " \t\n");
        if (!op) continue;

        int opcode = getOpcode(op);
        int instr = opcode << 28;

        if (opcode == 0 || opcode == 1 || opcode == 8 || opcode == 9) {
            // R-type
            char* rd = strtok(NULL, " \t\n");
            char* rs = strtok(NULL, " \t\n");
            char* rt = strtok(NULL, " \t\n");

            int rdVal = regToInt(rd);
            int rsVal = regToInt(rs);
            int rtVal = regToInt(rt);

            instr |= (rdVal & 0x1F) << 23;
            instr |= (rsVal & 0x1F) << 18;
            instr |= (rtVal & 0x1F) << 13;
            instr |= 0; // shamt if needed

        } else if (opcode == 7) {
            // J-type
            char* addr = strtok(NULL, " \t\n");
            int address = atoi(addr);
            instr |= (address & 0x0FFFFFFF);

        } else {
            // I-type
            char* rs = strtok(NULL, " \t\n");
            char* rt = strtok(NULL, " \t\n");
            char* imm = strtok(NULL, " \t\n");

            int rsVal = regToInt(rs);
            int rtVal = regToInt(rt);
            int immVal = atoi(imm); // signed

            instr |= (rsVal & 0x1F) << 23;
            instr |= (rtVal & 0x1F) << 18;
            instr |= (immVal & 0x3FFFF); // 18-bit imm
        }

        memory[instructionCount].data = instr;
        strcpy(memory[instructionCount].type, "Instruction");
        memory[instructionCount].used = 1;

        instructionCount++;
    }

    fclose(file);
    return instructionCount;
}
