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
