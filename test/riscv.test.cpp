#include "catch.hpp"
#include "globals.hpp"

#include <stdint.h>

TEST_CASE("load_mem")
{
  SECTION("A função deve carregar as instruções corretamente na memória.")
  {
    load_mem("assets/code.bin", 0);
    REQUIRE(mem[0] == 0xffe00313);              // primeira instrução;
    REQUIRE(mem[1] == 0x00300393);              // segunda instrução;
    REQUIRE(mem[0x000003f0 / 4] == 0x00008067); // última instrução;
  }
  SECTION("A função deve carregar os dados corretamente na memória.")
  {
    load_mem("assets/data.bin", DATA_SEGMENT_START);
    REQUIRE(mem[DATA_SEGMENT_START / 4] == 0xfafef1f0);          // primeiros 4 bytes em data;
    REQUIRE(mem[DATA_SEGMENT_START / 4 + 1] == 0x000a0009);      // segundos 4 bytes em data;
    REQUIRE(mem[(DATA_SEGMENT_START + 0x14) / 4] == 0x004c4941); // últimos bytes in data;
  }
}

TEST_CASE("init")
{
  SECTION("A função deve carregar as instruções e os dados corretamente na memória.")
  {
    init();
    REQUIRE(mem[1] == 0x00300393);                               // segunda instrução;
    REQUIRE(mem[0x000003f0 / 4] == 0x00008067);                  // última instrução;
    REQUIRE(mem[DATA_SEGMENT_START / 4 + 1] == 0x000a0009);      // segundos 4 bytes em data;
    REQUIRE(mem[(DATA_SEGMENT_START + 0x14) / 4] == 0x004c4941); // últimos bytes in data;
  }
  SECTION("A função deve iniciar corretamente os valores dos registradores.")
  {
    init();
    REQUIRE(pc == 0x00000000);
    REQUIRE(ri == 0x00000000);
    REQUIRE(sp == 0x00003ffc);
    REQUIRE(gp == 0x00001800);
  }
}

TEST_CASE("fetch")
{
  SECTION("A função deve carregar a instrução endereçada pelo pc.")
  {
    mem[1] = 0xffe00313;
    pc = 4;
    instruction_context_st instruction_context;
    fetch(instruction_context);
    REQUIRE(ri == 0xffe00313);
    REQUIRE(instruction_context.ir == 0xffe00313);
  }
  SECTION("A função deve incrementar pc.")
  {
    mem[1] = 0xffe00313;
    pc = 4;
    instruction_context_st instruction_context;
    fetch(instruction_context);
    REQUIRE(pc == 8);
    REQUIRE(instruction_context.pc == 8);
  }
}

TEST_CASE("decode")
{
  instruction_context_st instruction_context;
  SECTION("add")
  {
    ri = 0x00730e33; // add t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_add);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("addi")
  {
    ri = 0x80030313; // addi t1, t1, -2048
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_addi);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.imm12_i == -2048);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("and")
  {
    ri = 0x00737e33; // and t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_and);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("andi")
  {
    ri = 0x58437313; // addi t1, t1, 0x584
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_andi);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.imm12_i == 0x584);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("auipc")
  {
    ri = 0x00000397; // auipc t2, 0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_auipc);
    REQUIRE(instruction_context.imm20_u == 0);
    REQUIRE(instruction_context.rd == 7);
  }
  SECTION("beq")
  {
    ri = 0x00628663; // beq t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_beq);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bne")
  {
    ri = 0x00629a63; // bne t0, t1, 0x00000014;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bne);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x00000014);
  }
  SECTION("bge")
  {
    ri = 0x00535663; // beq t1, t0, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bge);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 5);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bgeu")
  {
    ri = 0x00537663; // beq t1, t0, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bgeu);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 5);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("blt")
  {
    ri = 0x0062c663; // blt t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_blt);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bltu")
  {
    ri = 0x0062e663; // bltu t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bltu);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("jal")
  {
    ri = 0x3a0000ef; // jal ra, 0x000003a0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UJType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_jal);
    REQUIRE(instruction_context.rd == 1);
    REQUIRE(instruction_context.imm21 == 0x000003a0);
  }
  SECTION("jalr")
  {
    ri = 0x00040067; // jal zero, s0, 0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_jalr);
    REQUIRE(instruction_context.rs1 == 8);
    REQUIRE(instruction_context.rd == 0);
    REQUIRE(instruction_context.imm12_i == 0);
  }
  SECTION("lb")
  {
    ri = 0x00028303; // addi t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lb);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("or")
  {
    ri = 0x00736e33; // or t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_or);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("lbu")
  {
    ri = 0x0002c303; // lbu t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lbu);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("lw")
  {
    ri = 0x0002a303; // lw t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lw);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("lui")
  {
    ri = 0x00009337; // lui t1, 9
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lui);
    REQUIRE(instruction_context.imm20_u == 9);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("sltu")
  {
    ri = 0x00003033; // sltu zero, zero, zero
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sltu);
  }
  SECTION("ori")
  {
    ri = 0x0036313; // sltu zero, zero, zero
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_ori);
  }
  SECTION("sb")
  {
    ri = 0x007280a3; // sb t2, 1(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sb);
  }
  SECTION("slli")
  {
    ri = 0x00c39393; // slli t2, t2, 12
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slli);
  }
  SECTION("slt")
  {
    ri = 0x00732e33; // slt t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slt);
  }
  SECTION("srai")
  {
    ri = 0x00732e33; // slt t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slt);
  }
  SECTION("srli")
  {
    ri = 0x01835393; // srli t2, t1, 24
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_srli);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.shamt == 24);
    REQUIRE(instruction_context.rd == 7);
  }
  SECTION("sub")
  {
    ri = 0x40730e33; // sub t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sub);
  }
  SECTION("sw")
  {
    ri = 0x0072a023; // sw t2, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sw);
    REQUIRE(instruction_context.imm12_s == 0);
  }
  SECTION("xor")
  {
    ri = 0x00734e33; // xor t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_xor);
  }
  SECTION("ecall")
  {
    ri = 0x00000073; // ecall
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_ecall);
  }
}

TEST_CASE("execute")
{
  {
  }
  SECTION("addi")
  {
  }
  SECTION("and")
  {
  }
  SECTION("andi")
  {
  }
  SECTION("auipc")
  {
  }
  SECTION("beq")
  {
  }
  SECTION("bne")
  {
  }
  SECTION("bge")
  {
  }
  SECTION("bgeu")
  {
  }
  SECTION("blt")
  {
  }
  SECTION("bltu")
  {
  }
  SECTION("jal")
  {
  }
  SECTION("jalr")
  {
  }
  SECTION("lb")
  {
  }
  SECTION("or")
  {
  }
  SECTION("lbu")
  {
  }
  SECTION("lw")
  {
  }
  SECTION("lui")
  {
  }
  SECTION("nop")
  {
  }
  SECTION("sltu")
  {
  }
  SECTION("ori")
  {
  }
  SECTION("sb")
  {
  }
  SECTION("slli")
  {
  }
  SECTION("slt")
  {
  }
  SECTION("srai")
  {
  }
  SECTION("srli")
  {
  }
  SECTION("sub")
  {
  }
  SECTION("sw")
  {
  }
  SECTION("xor")
  {
  }
  SECTION("ecall")
  {
  }
}
