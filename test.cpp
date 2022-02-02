#define CATCH_CONFIG_MAIN

#include <stdint.h>
#include "catch.hpp"
#include "globals.cpp"
#include "trabalho1.cpp"

TEST_CASE("lw")
{
  SECTION("A função deve checar se o endereço é um múltiplo de 4 (%4 == 0).")
  {
    REQUIRE(lw(3, 0) == 0);
  }
  SECTION("Se o endereço estiver correto, a função deve retornar o valor da memória")
  {
    int32_t anyInteger = 0xBBCCDDFF;
    mem[2] = anyInteger;
    REQUIRE(lw(4, 1) == anyInteger);
  }
}