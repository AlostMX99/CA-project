#ifndef REGISTERS_H
#define REGISTERS_H


#include <stdio.h>	
extern registerHome myRegisters;
typedef struct 
{
    char* type;
    int  data;
}reg;

typedef struct
{
    reg regHome[33];
}registerHome;

reg* getRegister(int index);
reg* createRegister(int data, char* type);
int getData(reg* r);
char* getType(reg* r);
void overwriteData(reg* r, int data);
void overwriteType(reg* r, char* type);
void initRegisterHome(registerHome* home);
reg* getPC(void);
void incrementPC(void);

#endif