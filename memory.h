#ifndef MEMORY_H
#define MEMORY_H

// Structure for each memory slot
struct Slot {
    char type[20];
    int data;
    int used; // 1 if used, 0 otherwise
};

// Global memory and index tracker
extern struct Slot memory[2048];
extern int UseIndex;

// Function declarations
void CreateMemory();
int FetchIndex(int index);
void overwrite(int NewData, int address);
void store(int NewData);

#endif // MEMORY_H
