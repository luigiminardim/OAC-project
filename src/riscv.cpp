#include "globals.hpp"
#include <stdint.h>

int32_t mem[MEM_SIZE];

void init() { return; }
void fetch(instruction_context_st &ic) { return; }
void decode(instruction_context_st &ic) { return; }
void print_instr(instruction_context_st &ic) { return; }
INSTRUCTIONS get_instr_code(uint32_t opcode, uint32_t func3, uint32_t func7) { return INSTRUCTIONS::I_add; }
FORMATS get_i_format(uint32_t opcode, uint32_t func3, uint32_t func7) { return FORMATS::RType; }
void debug_decode(instruction_context_st &ic) { return; }
void dump_breg() { return; }
void dump_asm(int start, int end) { return; }
void dump_mem(int start_byte, int end_byte, char format) { return; }
int load_mem(const char *fn, int start) { return 0; }
void execute(instruction_context_st &ic) { return; }
void step() { return; }
void run() { return; }
