#include <iostream>
#include <iomanip>
#include "globals.hpp"
#include "trabalho1.hpp"

int main()
{
  init();
  std::cout << "Instruções:" << std::endl;
  dump_mem(0, 1012, 'h');
  std::cout << "Dados:" << std::endl;
  dump_mem(DATA_SEGMENT_START, MEM_SIZE * 4, 'h');
}
