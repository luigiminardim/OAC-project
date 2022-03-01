#include <string>
#include <fstream>
#include <stdint.h>

#include "globals.hpp"
#include "trabalho1.hpp"

int32_t mem[MEM_SIZE];

uint32_t pc;

int32_t breg[32];

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
  breg[REGISTERS::SP] = 0x00003ffc;
  breg[REGISTERS::GP] = 0x00001800;
}

void fetch(instruction_context_st &ic)
{
  ic.pc = pc;
  ic.ir = lw(ic.pc, 0);
  pc = pc + 4;
}

int32_t extend32(int32_t number, uint qtd_number_bits)
{
  bool is_negative = get_bit(number, qtd_number_bits - 1);
  int32_t anti_mask = (is_negative ? 0xFFFFFFFF : 0x00000000) << qtd_number_bits;
  return number | anti_mask;
}

void decode(instruction_context_st &ic)
{
  ic.rs1 = (REGISTERS)(get_field(ic.ir, 15, 0x1f));
  ic.rs2 = (REGISTERS)(get_field(ic.ir, 20, 0x1f));
  ic.rd = (REGISTERS)(get_field(ic.ir, 7, 0x1f));
  ic.imm12_i = extend32(get_field(ic.ir, 20, 0xfff), 12);
  ic.imm20_u = extend32(get_field(ic.ir, 12, 0xfffff), 20);
  ic.imm12_s = extend32(
      (get_field(ic.ir, 25, 0x7f)) << 5 |
          (get_field(ic.ir, 7, 0x1f)),
      12);
  ic.imm13 = extend32(
      (get_bit(ic.ir, 31)) << 12 |
          (get_bit(ic.ir, 7)) << 11 |
          (get_field(ic.ir, 25, 0x3f)) << 5 |
          (get_field(ic.ir, 8, 0xf)) << 1,
      13);
  ic.imm21 = extend32(
      (get_bit(ic.ir, 31)) << 20 |
          (get_field(ic.ir, 12, 0xff)) << 12 |
          (get_bit(ic.ir, 20)) << 11 |
          (get_field(ic.ir, 21, 0x3FF)) << 1,
      21);
  ic.shamt = get_field(ic.ir, 20, 0x1f);
  uint32_t opcode = (ic.ir >> 0) & 0x7f;
  uint32_t funct3 = (ic.ir >> 12) & 0x7;
  uint32_t funct7 = (ic.ir >> 25) & 0x7f;
  if (opcode == OPCODES::RegType)
  {
    ic.ins_format = FORMATS::RType;
    if (funct3 == FUNCT3::ADDSUB3)
    {
      if (funct7 == FUNCT7::ADD7)
      {
        ic.ins_code = INSTRUCTIONS::I_add;
      }
      else if (funct7 == FUNCT7::SUB7)
      {
        ic.ins_code = INSTRUCTIONS::I_sub;
      }
    }
    else if (funct3 == FUNCT3::AND3)
    {
      ic.ins_code = INSTRUCTIONS::I_and;
    }
    else if (funct3 == FUNCT3::OR3)
    {
      ic.ins_code = INSTRUCTIONS::I_or;
    }
    else if (funct3 == FUNCT3::SLTU3)
    {
      ic.ins_code = INSTRUCTIONS::I_sltu;
    }
    else if (funct3 == FUNCT3::SLT3)
    {
      ic.ins_code = INSTRUCTIONS::I_slt;
    }
    else if (funct3 == FUNCT3::XOR3)
    {
      ic.ins_code = INSTRUCTIONS::I_xor;
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
    else if (funct3 == FUNCT3::ORI3)
    {
      ic.ins_code = INSTRUCTIONS::I_ori;
    }
    else if (funct3 == FUNCT3::SLLI3)
    {
      ic.ins_code = INSTRUCTIONS::I_slli;
    }
    else if (funct3 == FUNCT3::SRI3)
    {
      if (funct7 == FUNCT7::SRLI7)
      {
        ic.ins_code = INSTRUCTIONS::I_srli;
      }
      else if (funct7 == FUNCT7::SRAI7)
      {
        ic.ins_code = INSTRUCTIONS::I_srai;
      }
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
  else if (opcode == OPCODES::JALR)
  {
    ic.ins_format = FORMATS::IType;
    ic.ins_code = INSTRUCTIONS::I_jalr;
  }
  else if (opcode == OPCODES::ILType)
  {
    ic.ins_format = FORMATS::IType;
    if (funct3 == FUNCT3::LB3)
    {
      ic.ins_code = INSTRUCTIONS::I_lb;
    }
    else if (funct3 == FUNCT3::LBU3)
    {
      ic.ins_code = INSTRUCTIONS::I_lbu;
    }
    else if (funct3 == FUNCT3::LW3)
    {
      ic.ins_code = INSTRUCTIONS::I_lw;
    }
  }
  else if (opcode == OPCODES::LUI)
  {
    ic.ins_format = FORMATS::UType;
    ic.ins_code = INSTRUCTIONS::I_lui;
  }
  else if (opcode == OPCODES::StoreType)
  {
    ic.ins_format = FORMATS::SType;
    if (funct3 == FUNCT3::SB3)
    {
      ic.ins_code = INSTRUCTIONS::I_sb;
    }
    else if (funct3 == FUNCT3::SW3)
    {
      ic.ins_code = INSTRUCTIONS::I_sw;
    }
  }
  else if (opcode == OPCODES::ECALL)
  {
    ic.ins_format = FORMATS::IType;
    ic.ins_code = INSTRUCTIONS::I_ecall;
  }
}

void dump_breg()
{
  std::cout << "{";
  for (int i = 0; i < 32; i++)
  {
    std::cout << std::dec << "X" << i << ": 0x" << std::hex << breg[i] << ", ";
  }
  std::cout << "}";
}

void dump_asm(int start, int end) { return; }

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

void execute(instruction_context_st &ic)
{
  breg[REGISTERS::ZERO] = 0;
  if (ic.ins_code == INSTRUCTIONS::I_add)
  {
    breg[ic.rd] = breg[ic.rs1] + breg[ic.rs2];
  }
  else if (ic.ins_code == INSTRUCTIONS::I_addi)
  {
    breg[ic.rd] = breg[ic.rs1] + ic.imm12_i;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_and)
  {
    breg[ic.rd] = breg[ic.rs1] & breg[ic.rs2];
  }
  else if (ic.ins_code == INSTRUCTIONS::I_andi)
  {
    breg[ic.rd] = breg[ic.rs1] & ic.imm12_i;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_auipc)
  {
    breg[ic.rd] = ic.pc + (ic.imm20_u << 12);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_beq)
  {
    if (breg[ic.rs1] == breg[ic.rs2])
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_bne)
  {
    if (breg[ic.rs1] != breg[ic.rs2])
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_bge)
  {
    if (breg[ic.rs1] >= breg[ic.rs2])
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_bgeu)
  {
    if (((uint32_t)breg[ic.rs1]) >= ((uint32_t)breg[ic.rs2]))
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_blt)
  {
    if (breg[ic.rs1] < breg[ic.rs2])
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_bltu)
  {
    if (((uint32_t)breg[ic.rs1]) < ((uint32_t)breg[ic.rs2]))
    {
      pc = ic.pc + ic.imm13;
    }
  }
  else if (ic.ins_code == INSTRUCTIONS::I_jal)
  {
    breg[ic.rd] = ic.pc + 4;
    pc = ic.pc + ic.imm21;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_jalr)
  {
    pc = breg[ic.rs1] + ic.imm12_i;
    breg[ic.rd] = ic.pc + 4;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_lb)
  {
    breg[ic.rd] = lb(breg[ic.rs1], ic.imm12_i);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_or)
  {
    breg[ic.rd] = breg[ic.rs1] | breg[ic.rs2];
  }
  else if (ic.ins_code == INSTRUCTIONS::I_lbu)
  {
    breg[ic.rd] = lbu(breg[ic.rs1], ic.imm12_i);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_lw)
  {
    breg[ic.rd] = lw(breg[ic.rs1], ic.imm12_i);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_lui)
  {
    breg[ic.rd] = ic.imm20_u << 12;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_sltu)
  {
    breg[ic.rd] = ((uint32_t)breg[ic.rs1]) < ((uint32_t)breg[ic.rs2]) ? 1 : 0;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_nop)
  {
  }
  else if (ic.ins_code == INSTRUCTIONS::I_ori)
  {
    breg[ic.rd] = breg[ic.rs1] | ic.imm12_i;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_sb)
  {
    sb(breg[ic.rs1], ic.imm12_s, breg[ic.rs2]);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_slli)
  {
    breg[ic.rd] = breg[ic.rs1] << ic.shamt;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_slt)
  {
    breg[ic.rd] = breg[ic.rs1] < breg[ic.rs2] ? 1 : 0;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_srai)
  {
    breg[ic.rd] = breg[ic.rs1] >> ic.shamt;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_srli)
  {
    breg[ic.rd] = ((uint32_t)breg[ic.rs1]) >> ic.shamt;
  }
  else if (ic.ins_code == INSTRUCTIONS::I_sub)
  {
    breg[ic.rd] = breg[ic.rs1] - breg[ic.rs2];
  }
  else if (ic.ins_code == INSTRUCTIONS::I_sw)
  {
    sw(breg[ic.rs1], ic.imm12_s, breg[ic.rs2]);
  }
  else if (ic.ins_code == INSTRUCTIONS::I_xor)
  {
    breg[ic.rd] = breg[ic.rs1] ^ breg[ic.rs2];
  }
  else if (ic.ins_code == INSTRUCTIONS::I_ecall)
  {
    if (breg[REGISTERS::A7] == 1)
    {
      std::cout << breg[REGISTERS::A0];
    }
    else if (breg[REGISTERS::A7] == 4)
    {
      char *strPtr = (char *)mem + breg[REGISTERS::A0];
      std::cout << strPtr;
    }
    else if (breg[REGISTERS::A7] == 10)
    {
      pc = DATA_SEGMENT_START;
    }
  }
  breg[REGISTERS::ZERO] = 0;
}

void step()
{
  instruction_context_st ic;
  fetch(ic);
  decode(ic);
  static bool should_stop = false;
  if (ic.pc == -1)
    should_stop = true;
  if (should_stop)
  {
    std::cout << std::hex
              << "pc: " << ic.pc << "\n"
              << "ir: " << ic.ir << "\n"
              << "code: " << std::dec << ic.ins_code << "\n";
    dump_breg();
    std::cout << "\n";
    std::cin.get();
  }
  execute(ic);
}

void run()
{
  while (pc < DATA_SEGMENT_START)
  {
    step();
  }
}
