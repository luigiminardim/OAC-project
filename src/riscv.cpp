#include <string>
#include <fstream>
#include <stdint.h>

#include "globals.hpp"
#include "trabalho1.hpp"

int32_t mem[MEM_SIZE];

uint32_t pc, // contador de programa
    ri,      // registrador de intrucao
    sp,      // stack pointe4r
    gp;      // global pointer

int32_t imm12_i, // constante 12 bits
    imm12_s,     // constante 12 bits
    imm13,       // constante 13 bits
    imm20_u,     // constante 20 bis mais significativos
    imm21;       // constante 21 bits

uint32_t opcode, // codigo da operacao
    rs1,         // indice registrador rs
    rs2,         // indice registrador rt
    rd,          // indice registrador rd
    shamt,       // deslocamento
    funct3,      // campos auxiliares
    funct7;      // constante instrucao tipo J

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

/**
 * @brief  Busca a instrução a ser executada da memória e atualiza o pc.
 *
 * @param ic
 */
void fetch(instruction_context_st &ic)
{
  ic.ir = ri = lw(pc, 0);
  ic.pc = pc = pc + 4;
}

int32_t extend32(int32_t number, uint qtd_number_bits)
{
  bool is_negative = get_bit(number, qtd_number_bits - 1);
  int32_t anti_mask = (is_negative ? 0xFFFFFFFF : 0x00000000) << qtd_number_bits;
  return number | anti_mask;
}

void decode(instruction_context_st &ic)
{
  rs1 = ic.rs1 = (REGISTERS)(get_field(ri, 15, 0x1f));
  rs2 = ic.rs2 = (REGISTERS)(get_field(ri, 20, 0x1f));
  rd = ic.rd = (REGISTERS)(get_field(ri, 7, 0x1f));
  imm12_i = ic.imm12_i = extend32(get_field(ri, 20, 0xfff), 12);
  imm20_u = ic.imm20_u = extend32(get_field(ri, 12, 0xfffff), 20);
  imm13 = ic.imm13 = extend32(
      (get_bit(ri, 31)) << 12 |
          (get_bit(ri, 7)) << 11 |
          (get_field(ri, 25, 0x3f)) << 5 |
          (get_field(ri, 8, 0xf)) << 1,
      13);
  imm21 = ic.imm21 = extend32(
      (get_bit(ri, 31)) << 20 |
          (get_field(ri, 12, 0xff)) << 12 |
          (get_bit(ri, 20)) << 11 |
          (get_field(ri, 21, 0x3FF)) << 1,
      21);
  opcode = (ri >> 0) & 0x7f;
  funct3 = (ri >> 12) & 0x7;
  funct7 = (ri >> 25) & 0x7f;
  if (opcode == OPCODES::RegType)
  {
    ic.ins_format = FORMATS::RType;
    if (funct3 == FUNCT3::ADDSUB3)
    {
      if (funct7 == FUNCT7::ADD7)
      {
        ic.ins_code = INSTRUCTIONS::I_add;
      }
    }
    else if (funct3 == FUNCT3::AND3)
    {
      ic.ins_code = INSTRUCTIONS::I_and;
    }
  }
  else if (opcode == OPCODES::ILAType)
  {
    ic.ins_format = FORMATS::IType;
    if (funct3 == FUNCT3::ADDI3)
    {
      ic.ins_code = INSTRUCTIONS::I_addi;
    }
    else if (funct3 == FUNCT3::ANDI3)
    {
      ic.ins_code = INSTRUCTIONS::I_andi;
    }
  }
  else if (opcode == OPCODES::AUIPC)
  {
    ic.ins_format = FORMATS::UType;
    ic.ins_code = INSTRUCTIONS::I_auipc;
  }
  else if (opcode == OPCODES::BType)
  {
    ic.ins_format = FORMATS::SBType;
    if (funct3 == FUNCT3::BEQ3)
    {
      ic.ins_code = INSTRUCTIONS::I_beq;
    }
    else if (funct3 == FUNCT3::BNE3)
    {
      ic.ins_code = INSTRUCTIONS::I_bne;
    }
    else if (funct3 == FUNCT3::BGE3)
    {
      ic.ins_code = INSTRUCTIONS::I_bge;
    }
    else if (funct3 == FUNCT3::BGEU3)
    {
      ic.ins_code = INSTRUCTIONS::I_bgeu;
    }
    else if (funct3 == FUNCT3::BLT3)
    {
      ic.ins_code = INSTRUCTIONS::I_blt;
    }
    else if (funct3 == FUNCT3::BLT3)
    {
      ic.ins_code = INSTRUCTIONS::I_blt;
    }
    else if (funct3 == FUNCT3::BLTU3)
    {
      ic.ins_code = INSTRUCTIONS::I_bltu;
    }
  }
  else if (opcode == OPCODES::JAL)
  {
    ic.ins_format = FORMATS::UJType;
    ic.ins_code = INSTRUCTIONS::I_jal;
  }
}

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
