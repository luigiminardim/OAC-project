#include <iostream>
#include <iomanip>
#include "globals.cpp"
#include "trabalho1.cpp"

int main()
{
  // 1.Iniciar a memória;
  // a.
  sb(0, 0, 0x04);
  sb(0, 1, 0x03);
  sb(0, 2, 0x02);
  sb(0, 3, 0x01);
  // b.
  sb(4, 0, 0xFF);
  sb(4, 1, 0xFE);
  sb(4, 2, 0xFD);
  sb(4, 3, 0xFC);
  // c.
  sw(12, 0, 0xFF);
  // d.
  sw(16, 0, 0xFFFF);
  // e.
  sw(20, 0, 0xFFFFFFFF);
  // f.
  sw(24, 0, 0x80000000);

  // 2.Imprimir o conteúdo da memória em formato hexa;
  // a.
  std::cout << std::hex << "mem[0]=" << mem[0] << std::endl; // 01020304
  // b.
  std::cout << std::hex << "mem[1]=" << mem[1] << std::endl; // fcfdfeff
  // c.
  std::cout << std::hex << "mem[2]=" << mem[2] << std::endl; // 00000000
  // d.
  std::cout << std::hex << "mem[3]=" << mem[3] << std::endl; // 000000FF
  // e.
  std::cout << std::hex << "mem[4]=" << mem[4] << std::endl; // 0000FFFF
  // f.
  std::cout << std::hex << "mem[5]=" << mem[5] << std::endl; // FFFFFFFF
  // g.
  std::cout << std::hex << "mem[6]=" << mem[6] << std::endl; // 80000000

  // 3. Ler os dados e imprimir em hexadecimal;
  // a.
  std::cout << std::hex << lb(4, 0) << ' ' << lb(4, 1) << ' ' << lb(4, 2) << ' ' << lb(4, 3)
            << std::endl;
  // b.
  std::cout << lbu(4, 0) << ' ' << lbu(4, 1) << ' ' << lbu(4, 2) << ' ' << lbu(4, 3) << std::endl;
  // c.
  std::cout << lw(12, 0) << ' ' << lw(16, 0) << ' ' << lw(20, 0) << std::endl;
}