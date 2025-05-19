#ifndef REGISTERS_H
#define REGISTERS_H
#include <string.h>  // for strcpy
#include <stdlib.h>  // for atoi

typedef struct {
    char* type;
    int data;
} reg;

// Initialize the registers (must call before using)
void initRegisters(void);

// Get a pointer to a register by index (0 to 32)
reg* getRegister(int index);

// Get a pointer to the PC register (index 32)
reg* getPC(void);

// Increment the PC register by 1
void incrementPC(void);

// Create a new register (dynamically allocated)
reg* createRegister(int data, const char* type);

// Get register data
int getData(reg* r);

// Get register type string
const char* getType(reg* r);

// Overwrite register data (can't overwrite zero register)
void overwriteData(reg* r, int data);

// Overwrite register type (can't overwrite zero register)
void overwriteType(reg* r, const char* type);
reg* createRegister(int data, const char* type);
void overwriteType(reg* r, const char* type);
#endif // REGISTERS_H
