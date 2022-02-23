#include "globals.hpp"
#include <string>
#include <fstream>
#include <stdint.h>

int32_t mem[MEM_SIZE];

uint32_t pc, // contador de programa
    ri,      // registrador de intrucao
    sp,      // stack pointe4r
    gp;      // global pointer

/**
 * Ler o código e os dados contidos nos arquivos para a memória do simulador.
 */
int load_mem(const char *fn, int start)
{
  std::ifstream file(fn, std::ifstream::binary);
  if (file.is_open())
  {
    char *int8mem = (char *)mem;
    file.readsome(int8mem + start, MEM_SIZE * 4 - start);
    return 1;
  }
  else
  {
    return 0;
  }
}

void init()
{
  load_mem("assets/code.bin", 0);
  load_mem("assets/data.bin", DATA_SEGMENT_START);
  pc = 0x00000000;
  ri = 0x00000000;
  sp = 0x00003ffc;
  gp = 0x00001800;
}

void fetch(instruction_context_st &ic) { return; }
void decode(instruction_context_st &ic) { return; }
void print_instr(instruction_context_st &ic) { return; }
INSTRUCTIONS get_instr_code(uint32_t opcode, uint32_t func3, uint32_t func7) { return INSTRUCTIONS::I_add; }
FORMATS get_i_format(uint32_t opcode, uint32_t func3, uint32_t func7) { return FORMATS::RType; }
void debug_decode(instruction_context_st &ic) { return; }
void dump_breg() { return; }
void dump_asm(int start, int end) { return; }

/**
 * @brief Imprime o conteúdo da memória a partir do endereço start até o endereço end. O formato
 * pode ser em hexa (‘h’) ou decimal (‘d’).
 *
 * @param start_byte
 * Endereço inicial.
 *
 * @param end_byte
 * Endereço final
 *
 * @param format
 * 'h' para hexa ou 'd' para decimal
 */
void dump_mem(int start_byte, int end_byte, char format)
{
  auto base = format == 'h' ? std::hex : std::dec;
  uint8_t *int8Mem = (uint8_t *)mem;
  for (int i = start_byte; i < end_byte; i++)
  {
    int32_t number = int8Mem[i];
    std::cout << base << number << std::dec << " ";
  }
  std::cout << std::endl;
}

void execute(instruction_context_st &ic) { return; }
void step() { return; }
void run() { return; }
