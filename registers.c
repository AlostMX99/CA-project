#include "registers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Static array of 33 registers: R1-R31, zero, PC
static reg registers[33];

void initRegisters(void) {
    // Initialize R1 to R31
    for (int i = 0; i < 31; i++) {
        char name[4];
        snprintf(name, sizeof(name), "R%d", i + 1);
        registers[i].type = strdup(name);
        registers[i].data = 0;
    }

    // zero register at index 31
    registers[31].type = strdup("zero");
    registers[31].data = 0;

    // PC register at index 32
    registers[32].type = strdup("pc");
    registers[32].data = 0;
}

reg* getRegister(int index) {
    if (index < 0 || index > 32) {
        return NULL; // invalid index
    }
    return &registers[index];
}

reg* getPC(void) {
    return &registers[32];
}

void incrementPC(void) {
    registers[32].data++;
}

reg* createRegister(int data, const char* type) {
    reg* r = (reg*) malloc(sizeof(reg));
    if (!r) return NULL;
    r->data = data;
    r->type = strdup(type);
    return r;
}

int getData(reg* r) {
    return r->data;
}

const char* getType(reg* r) {
    return r->type;
}

void overwriteData(reg* r, int data) {
    if (strcmp(r->type, "zero") == 0) {
        printf("Error: Cannot modify zero register data.\n");
        return;
    }
    r->data = data;
}

void overwriteType(reg* r, const char* type) {
    if (strcmp(r->type, "zero") == 0) {
        printf("Error: Cannot modify zero register type.\n");
        return;
    }
    free(r->type);
    r->type = strdup(type);
}
