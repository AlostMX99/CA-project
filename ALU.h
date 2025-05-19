// alu.h

#ifndef ALU_H
#define ALU_H

#include <stdint.h>

extern struct Slot memory[2048];
extern int registers[32];

int ALU(void);

#endif
