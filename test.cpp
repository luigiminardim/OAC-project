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

TEST_CASE("lb")
{
  SECTION("Ler um byte com sinal negativo")
  {
    mem[1] = 0x0000F100; // Big Endian
    REQUIRE(lb(4, 1) == 0xFFFFFFF1);
  }
  SECTION("Ler um byte com sinal positivo")
  {
    mem[1] = 0x00007F00;
    REQUIRE(lb(4, 1) == 0x0000007F);
  }
}

TEST_CASE("lbu")
{
  SECTION("Ler um byte com sinal negativo")
  {
    mem[1] = 0x0000F100; // Big Endian
    REQUIRE(lbu(4, 1) == 0x000000F1);
  }
  SECTION("Ler um byte com sinal positivo")
  {
    mem[1] = 0x00007F00;
    REQUIRE(lbu(4, 1) == 0x0000007F);
  }
}

TEST_CASE("sw")
{
  SECTION("Se o endereço estiver correto, a função deve armazenar o valor na memória")
  {
    mem[2] = 0;
    int32_t anyInteger = 0xBBCCDDFF;
    sw(4, 1, anyInteger);
    REQUIRE(mem[2] == anyInteger);
  }
}