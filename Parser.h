#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdint.h>

// Pipeline stage indices
#define IF 0
#define ID 1
#define EX 2
#define MEM 3
#define WB 4

// Opcodes
#define OPCODE_ADD 0
#define OPCODE_ADDI 7
#define OPCODE_LW 8
#define OPCODE_SW 9
#define OPCODE_BNE 10
#define OPCODE_J 11

// Pipeline stage structure
typedef struct {
    int instruction;
    int opcode;
    int R1, R2, R3, shamt, immediate, address;
    int result, mem_address, mem_data;
    int valid;
} PipelineStage;

// External global variables
extern int flush_timer;
extern int clock_cycle;
extern PipelineStage pipeline[5];

// Function declarations
int sign_extend(int value, int bits);
void fetch();
void decode();
void execute();
void memory_access();
void writeback();
void advance_pipeline();
void print_pipeline_state();
void print_final_state();
int is_pipeline_empty();
void simulate();

#endif // PIPELINE_H