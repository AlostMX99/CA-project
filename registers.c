#include "registers.h"
#include <stdlib.h>
#include <stdio.h>
registerHome myRegisters;

reg* createRegister(int data, char* type){
    reg* r = (reg*) malloc(sizeof(reg));
    r->data = data;
    r->type = type;
    return r;
}
int getData(reg* r){
    return r->data;
}
char* getType(reg* r){
    return r->type;
}
void overwriteData(reg* r, int data) {
    if (strcmp(r->type, "zero") == 0) {
        printf("Error: Cannot modify zero register data.\n");
        return;
    }
    r->data = data;
}

void overwriteType(reg* r, char* type) {
    if (strcmp(r->type, "zero") == 0) {
        printf("Error: Cannot modify zero register type.\n");
        return;
    }
    free(r->type);
    r->type = strdup(type);
}

void initRegisterHome(registerHome* home) {
    for (int i = 1; i <= 31; i++) {
        char name[4];
        snprintf(name, sizeof(name), "R%d", i);
        home->regHome[i - 1].type = strdup(name);
        home->regHome[i - 1].data = 0;
    }
    home->regHome[31].type = strdup("zero");
    home->regHome[31].data = 0;

    home->regHome[32].type = strdup("pc");
    home->regHome[32].data = 0;
}

reg* getPC() {
    return &myRegisters.regHome[32];  // Index 32 holds the "pc"
}

void incrementPC() {
    myRegisters.regHome[32].data++;
}
reg* getRegister(int index) {
    if (index < 0 || index > 32) {
        printf("Invalid register index: %d\n", index);
        exit(1);
    }
    return &myRegisters.regHome[index];
}
