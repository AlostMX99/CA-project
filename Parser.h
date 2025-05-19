#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdint.h>
#include "memory.h"

// Pipeline stage indices
#define IF  0
#define ID  1
#define EX  2
#define MEM 3
#define WB  4

// Opcodes
#define OPCODE_ADD  0
#define OPCODE_ADDI 7
#define OPCODE_LW   8
#define OPCODE_SW   9
#define OPCODE_BNE  10
#define OPCODE_J    11

// Pipeline stage structure
typedef struct {
    int instruction;   // Full instruction word
    int opcode;        // Decoded opcode
    int R1, R2, R3;   // Register operands
    int shamt;         // Shift amount
    int immediate;     // Immediate value
    int address;       // Target address for jumps/branches
    int result;        // Result of ALU or other operation
    int mem_address;   // Memory address for load/store
    int mem_data;      // Data to write/read from memory
    int valid;         // Flag indicating if stage holds a valid instruction
} PipelineStage;

// External global variables
extern int flush_timer;           // Timer to track pipeline flushes
extern int clock_cycle;           // Current clock cycle count
extern PipelineStage pipeline[5]; // Array representing pipeline stages IF, ID, EX, MEM, WB

// Function declarations

/**
 * Sign-extend a value with the given bit width.
 * @param value The integer value to extend.
 * @param bits Number of bits in the original value.
 * @return Sign-extended integer.
 */
int sign_extend(int value, int bits);

/**
 * Fetch stage of the pipeline.
 */
void fetch(void);

/**
 * Decode stage of the pipeline.
 */
void decode(void);

/**
 * Execute stage of the pipeline.
 */
void execute(void);

/**
 * Memory access stage of the pipeline.
 */
void memory_access(void);

/**
 * Writeback stage of the pipeline.
 */
void writeback(void);

/**
 * Advance pipeline by one cycle.
 */
void advance_pipeline(void);

/**
 * Print current pipeline state.
 */
void print_pipeline_state(void);

/**
 * Print final state after simulation ends.
 */
void print_final_state(void);

/**
 * Check if pipeline is empty (no valid instructions).
 * @return 1 if empty, 0 otherwise.
 */
int is_pipeline_empty(void);

/**
 * Run the full pipeline simulation.
 */
void simulate(void);

/**
 * Parse input file with instructions.
 * @param fileName Path to instruction file.
 * @return 0 on success, non-zero on failure.
 */
int parseFile(const char* fileName);

#endif // PIPELINE_H
