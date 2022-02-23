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
