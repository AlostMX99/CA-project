#include <string.h> 
#include "memory.h";

static int UseIndex= 1024;
struct Slot memory[2048];

void CreateMemory() {
    for (int i = 0; i < 2048; i++) {
        if (i < 1024) {
            strcpy(memory[i].type, "Instruction");
            //parsing instructions goes here
        } 
        else {
            strcpy(memory[i].type, "Data");
            memory[i].used=0;
        }
    }
}
int FetchIndex(int index){
    return memory[index].data;
}
void overwrite(int NewData,int address){
    memory[address].data = NewData;
    memory[address].used = 1;
}
void store(int NewData){
    if(UseIndex<2048){
        memory[UseIndex].data = NewData;
        UseIndex++;
    }
    else{
        printf("Memory is full, bozo");
    }
}
