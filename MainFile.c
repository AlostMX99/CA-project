#include <stdio.h>
#include "registers.h"
#include "decoder.h"
#include "registers.h"
int InstructionFetch(){
    fetch(getData(getPC()));
    incrementPC();
}