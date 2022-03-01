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
    REQUIRE(breg[REGISTERS::SP] == 0x00003ffc);
    REQUIRE(breg[REGISTERS::GP] == 0x00001800);
  }
}

TEST_CASE("fetch")
{
  SECTION("A função deve carregar a instrução endereçada pelo pc.")
  {
    mem[1] = 0xffe00313;
    pc = 4;
    instruction_context_st ic;
    fetch(ic);
    REQUIRE(ic.ir == 0xffe00313);
  }
  SECTION("A função deve incrementar pc.")
  {
    mem[1] = 0xffe00313;
    pc = 4;
    instruction_context_st ic;
    fetch(ic);
    REQUIRE(ic.pc == 4);
    REQUIRE(pc == 8);
  }
}

TEST_CASE("decode")
{
  instruction_context_st instruction_context;
  SECTION("add")
  {
    instruction_context.ir = 0x00730e33; // add t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_add);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("addi")
  {
    instruction_context.ir = 0x80030313; // addi t1, t1, -2048
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_addi);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.imm12_i == -2048);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("and")
  {
    instruction_context.ir = 0x00737e33; // and t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_and);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("andi")
  {
    instruction_context.ir = 0x58437313; // addi t1, t1, 0x584
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_andi);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.imm12_i == 0x584);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("auipc")
  {
    instruction_context.ir = 0x00000397; // auipc t2, 0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_auipc);
    REQUIRE(instruction_context.imm20_u == 0);
    REQUIRE(instruction_context.rd == 7);
  }
  SECTION("beq")
  {
    instruction_context.ir = 0x00628663; // beq t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_beq);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bne")
  {
    instruction_context.ir = 0x00629a63; // bne t0, t1, 0x00000014;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bne);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x00000014);
  }
  SECTION("bge")
  {
    instruction_context.ir = 0x00535663; // beq t1, t0, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bge);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 5);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bgeu")
  {
    instruction_context.ir = 0x00537663; // beq t1, t0, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bgeu);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 5);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("blt")
  {
    instruction_context.ir = 0x0062c663; // blt t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_blt);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("bltu")
  {
    instruction_context.ir = 0x0062e663; // bltu t0, t1, 0x0000000c;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SBType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_bltu);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.rs2 == 6);
    REQUIRE(instruction_context.imm13 == 0x0000000c);
  }
  SECTION("jal")
  {
    instruction_context.ir = 0x3a0000ef; // jal ra, 0x000003a0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UJType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_jal);
    REQUIRE(instruction_context.rd == 1);
    REQUIRE(instruction_context.imm21 == 0x000003a0);
  }
  SECTION("jalr")
  {
    instruction_context.ir = 0x00040067; // jal zero, s0, 0;
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_jalr);
    REQUIRE(instruction_context.rs1 == 8);
    REQUIRE(instruction_context.rd == 0);
    REQUIRE(instruction_context.imm12_i == 0);
  }
  SECTION("lb")
  {
    instruction_context.ir = 0x00028303; // addi t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lb);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("or")
  {
    instruction_context.ir = 0x00736e33; // or t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_or);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.rs2 == 7);
    REQUIRE(instruction_context.rd == 28);
  }
  SECTION("lbu")
  {
    instruction_context.ir = 0x0002c303; // lbu t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lbu);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("lw")
  {
    instruction_context.ir = 0x0002a303; // lw t1, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lw);
    REQUIRE(instruction_context.rs1 == 5);
    REQUIRE(instruction_context.imm12_i == 0x0);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("lui")
  {
    instruction_context.ir = 0x00009337; // lui t1, 9
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::UType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_lui);
    REQUIRE(instruction_context.imm20_u == 9);
    REQUIRE(instruction_context.rd == 6);
  }
  SECTION("sltu")
  {
    instruction_context.ir = 0x00003033; // sltu zero, zero, zero
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sltu);
  }
  SECTION("ori")
  {
    instruction_context.ir = 0x0036313; // sltu zero, zero, zero
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_ori);
  }
  SECTION("sb")
  {
    instruction_context.ir = 0x007280a3; // sb t2, 1(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sb);
  }
  SECTION("slli")
  {
    instruction_context.ir = 0x00c39393; // slli t2, t2, 12
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slli);
  }
  SECTION("slt")
  {
    instruction_context.ir = 0x00732e33; // slt t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slt);
  }
  SECTION("srai")
  {
    instruction_context.ir = 0x00732e33; // slt t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_slt);
  }
  SECTION("srli")
  {
    instruction_context.ir = 0x01835393; // srli t2, t1, 24
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_srli);
    REQUIRE(instruction_context.rs1 == 6);
    REQUIRE(instruction_context.shamt == 24);
    REQUIRE(instruction_context.rd == 7);
  }
  SECTION("sub")
  {
    instruction_context.ir = 0x40730e33; // sub t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sub);
  }
  SECTION("sw")
  {
    instruction_context.ir = 0x0072a023; // sw t2, 0(t0)
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::SType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_sw);
    REQUIRE(instruction_context.imm12_s == 0);
  }
  SECTION("xor")
  {
    instruction_context.ir = 0x00734e33; // xor t3, t1, t2
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::RType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_xor);
  }
  SECTION("ecall")
  {
    instruction_context.ir = 0x00000073; // ecall
    decode(instruction_context);
    REQUIRE(instruction_context.ins_format == FORMATS::IType);
    REQUIRE(instruction_context.ins_code == INSTRUCTIONS::I_ecall);
  }
}

TEST_CASE("execute")
{
  instruction_context_st ic;
  SECTION("add")
  {
    ic.ins_code = INSTRUCTIONS::I_add;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 94;
    breg[REGISTERS::T2] = -64;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 94 + (-64));
  }
  SECTION("addi")
  {
    ic.ins_code = INSTRUCTIONS::I_addi;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 94;
    ic.imm12_i = -64;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 94 + (-64));
  }
  SECTION("and")
  {
    ic.ins_code = INSTRUCTIONS::I_and;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xFF00FF00;
    breg[REGISTERS::T2] = 0x0000FFFF;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x0000FF00);
  }
  SECTION("andi")
  {
    ic.ins_code = INSTRUCTIONS::I_andi;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0x00FF00FF;
    ic.imm12_i = 0x0FFFF;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x000000FF);
  }
  SECTION("auipc")
  {
    ic.ins_code = INSTRUCTIONS::I_auipc;
    ic.pc = 0x04;
    ic.rd = REGISTERS::T3;
    ic.imm20_u = 0x0FFFF;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x0FFFF004);
  }
  SECTION("beq")
  {
    ic.ins_code = INSTRUCTIONS::I_beq;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 6;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);

    breg[REGISTERS::T1] = 5;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);
  }
  SECTION("bne")
  {
    ic.ins_code = INSTRUCTIONS::I_bne;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 6;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);

    breg[REGISTERS::T1] = 5;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);
  }
  SECTION("bge")
  {
    ic.ins_code = INSTRUCTIONS::I_bge;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 6;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);

    breg[REGISTERS::T1] = 4;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);
  }
  SECTION("bgeu")
  {
    ic.ins_code = INSTRUCTIONS::I_bgeu;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 0x10000000;
    breg[REGISTERS::T2] = 0x10000001;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);

    breg[REGISTERS::T1] = 0x10000000;
    breg[REGISTERS::T2] = 0x10000000;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);
  }
  SECTION("blt")
  {
    ic.ins_code = INSTRUCTIONS::I_blt;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 6;
    breg[REGISTERS::T2] = 5;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);

    breg[REGISTERS::T1] = 5;
    breg[REGISTERS::T2] = 6;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);
  }
  SECTION("bltu")
  {
    ic.ins_code = INSTRUCTIONS::I_bltu;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    breg[REGISTERS::T1] = 0x10000000;
    breg[REGISTERS::T2] = 0x10000000;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04);

    breg[REGISTERS::T1] = 0x10000000;
    breg[REGISTERS::T2] = 0x10000001;
    pc = ic.pc = 0x04;
    ic.imm13 = 0x0000000c;
    execute(ic);
    REQUIRE(pc == 0x04 + 0x0000000c);
  }
  SECTION("jal")
  {
    ic.ins_code = INSTRUCTIONS::I_jal;
    ic.rd = REGISTERS::T3;
    pc = ic.pc = 0x04;
    ic.imm21 = 0x0000000c;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x08);
    REQUIRE(pc == 0x04 + 0x0000000c);
  }
  SECTION("jalr")
  {
    ic.ins_code = INSTRUCTIONS::I_jalr;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xfff00000;
    pc = ic.pc = 0x04;
    ic.imm12_i = 0x0000c;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x08);
    REQUIRE(pc == 0xfff00000 + 0x0000c);
  }
  SECTION("lb")
  {
    ic.ins_code = INSTRUCTIONS::I_lb;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    ic.imm12_i = 0x004;
    breg[REGISTERS::T1] = 0x000000ff;
    ((char *)mem)[0x004 + 0x000000ff] = 0xdd;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xffffffdd);
  }
  SECTION("or")
  {
    ic.ins_code = INSTRUCTIONS::I_or;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xFF00FF00;
    breg[REGISTERS::T2] = 0x0000FFFF;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xFF00FFFF);
  }
  SECTION("lbu")
  {
    ic.ins_code = INSTRUCTIONS::I_lbu;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    ic.imm12_i = 0x004;
    breg[REGISTERS::T1] = 0x000000ff;
    ((char *)mem)[0x004 + 0x000000ff] = 0xdd;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x000000dd);
  }
  SECTION("lw")
  {
    ic.ins_code = INSTRUCTIONS::I_lw;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    ic.imm12_i = 0x00004;
    breg[REGISTERS::T1] = 0x000000f0;
    mem[0x00004 + 0x000000f0 / 4] = 0xaabbccdd;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xaabbccdd);
  }
  SECTION("lui")
  {
    ic.ins_code = INSTRUCTIONS::I_lui;
    ic.rd = REGISTERS::T3;
    ic.imm20_u = 0xfffffabc;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xffabc000);
  }
  SECTION("nop")
  {
    execute(ic);
    REQUIRE(REGISTERS::ZERO == 0);
  }
  SECTION("sltu")
  {
    ic.ins_code = INSTRUCTIONS::I_sltu;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0x10000000;
    breg[REGISTERS::T2] = 0x10000001;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 1);

    breg[REGISTERS::T1] = 0x10000001;
    breg[REGISTERS::T2] = 0x10000000;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0);
  }
  SECTION("ori")
  {
    ic.ins_code = INSTRUCTIONS::I_ori;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xFF00FF00;
    ic.imm12_i = 0x0FFFF;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xFF00FFFF);
  }
  SECTION("sb")
  {
    ic.ins_code = INSTRUCTIONS::I_sb;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.imm12_s = 0x0000f;
    breg[REGISTERS::T1] = DATA_SEGMENT_START;
    breg[REGISTERS::T2] = 'a';
    execute(ic);
    REQUIRE(((int8_t *)mem)[0x0000f + DATA_SEGMENT_START] == 'a');
  }
  SECTION("slli")
  {
    ic.ins_code = INSTRUCTIONS::I_slli;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xFF00FF00;
    ic.shamt = 0x00004;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xF00FF000);
  }
  SECTION("slt")
  {
    ic.ins_code = INSTRUCTIONS::I_slt;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 5;
    breg[REGISTERS::T2] = 6;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 1);

    breg[REGISTERS::T1] = 5;
    breg[REGISTERS::T2] = 5;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0);
  }
  SECTION("srai")
  {
    ic.ins_code = INSTRUCTIONS::I_srai;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xff00ff00;
    ic.shamt = 0x00004;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xfff00ff0);
  }
  SECTION("srli")
  {
    ic.ins_code = INSTRUCTIONS::I_srli;
    ic.rs1 = REGISTERS::T1;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xff00ff00;
    ic.shamt = 0x00004;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0x0ff00ff0);
  }
  SECTION("sub")
  {
    ic.ins_code = INSTRUCTIONS::I_sub;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 94;
    breg[REGISTERS::T2] = -64;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 94 - (-64));
  }
  SECTION("sw")
  {
    ic.ins_code = INSTRUCTIONS::I_sw;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.imm12_s = 0x0000f;
    breg[REGISTERS::T2] = 0xab;
    breg[REGISTERS::T1] = DATA_SEGMENT_START;
    execute(ic);
    REQUIRE(mem[0x0000f + DATA_SEGMENT_START / 4] == 0xab);
  }
  SECTION("xor")
  {
    ic.ins_code = INSTRUCTIONS::I_xor;
    ic.rs1 = REGISTERS::T1;
    ic.rs2 = REGISTERS::T2;
    ic.rd = REGISTERS::T3;
    breg[REGISTERS::T1] = 0xff00ff00;
    breg[REGISTERS::T2] = 0x0000ffff;
    execute(ic);
    REQUIRE(breg[REGISTERS::T3] == 0xff0000ff);
  }
  SECTION("ecall")
  {
  }
}
