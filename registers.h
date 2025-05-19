#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdio.h>
#include <stdlib.h>

// Define reg structure representing a single register with type and data.
typedef struct {
    const char* type;
    int data;
} reg;

// Define registerHome holding an array of 33 registers.
typedef struct {
    reg regHome[33];
} registerHome;

// Global registerHome instance declared elsewhere
extern registerHome myRegisters;

// Functions to manipulate registers

// Returns pointer to register at given index (0-32)
reg* getRegister(int index);

// Creates a new register with given data and type (returns pointer)
reg* createRegister(int data, const char* type);

// Returns data from given register
int getData(const reg* r);

// Returns type string from given register
const char* getType(const reg* r);

// Overwrites the data of given register
void overwriteData(reg* r, int data);

// Overwrites the type string of given register
void overwriteType(reg* r, const char* type);

// Initializes the register home (all registers)
void initRegisterHome(registerHome* home);

// Returns pointer to the Program Counter register (assumed at some fixed index)
reg* getPC(void);

// Increments the Program Counter register by 1
void incrementPC(void);

#endif // REGISTERS_H
