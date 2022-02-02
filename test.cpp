#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "globals.cpp"


TEST_CASE("MEMSIZE", "") {
  REQUIRE(MEM_SIZE == 4096);
}