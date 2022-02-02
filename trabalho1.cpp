#pragma once

#include <stdint.h>
#include <iostream>
#include "globals.cpp"

/** Lê um inteiro alinhado - endereços múltiplos de 4. 
 * 
 * A função deve checar se o endereço é um múltiplo de 4 (%4 == 0). 
 * Se não for, deve escrever uma mensagem de erro e retornar zero. 
 * Se o endereço estiver correto, a função deve: 
 *   - Dividi-lo por 4 para obter o índice do vetor memória 
 *   - Retornar o valor lido da memória
 * 
 * @param address
 * Endereço especiCicado.
 * 
 * @param kte
 * Deslocamento para frente ou para trás com relação ao endereço
 * 
 * @return int32_t 
 */
int32_t lw(uint32_t address, int32_t kte)
{
  if (address % 4 != 0)
  {
    std::cerr << "Error: address % 4 != 0" << std::endl;
    return 0;
  }
  else
  {
    uint32_t address_index = address / 4;
    uint32_t index = address_index + kte;
    return mem[index];
  }
}