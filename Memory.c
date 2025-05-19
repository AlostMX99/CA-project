// Refactored memory.c for clean and safe operation
#include <string.h>
#include <stdio.h>
#include "memory.h"

int UseIndex = 1024; // Data memory starts at 1024
struct Slot memory[2048];

void CreateMemory() {
    for (int i = 0; i < 2048; i++) {
        if (i < 1024) {
            strcpy(memory[i].type, "Instruction");
            memory[i].used = 1; // instructions are always 'used'
        } else {
            strcpy(memory[i].type, "Data");
            memory[i].used = 0;
        }
    }
}

int FetchIndex(int index) {
    if (index < 0 || index >= 2048) {
        printf("Error: Memory fetch out of bounds (index = %d)\n", index);
        return 0;
    }
    return memory[index].data;
}

void overwrite(int NewData, int address) {
    if (address >= 1024 && address < 2048) {
        memory[address].data = NewData;
        memory[address].used = 1;
    } else {
        printf("Error: Attempted to write outside data memory (address = %d)\n", address);
    }
}

void store(int NewData) {
    if (UseIndex >= 1024 && UseIndex < 2048) {
        memory[UseIndex].data = NewData;
        memory[UseIndex].used = 1;
        UseIndex++;
    } else {
        printf("Error: Data memory is full or invalid UseIndex (%d)\n", UseIndex);
    }
}
