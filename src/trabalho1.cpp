#include "trabalho1.hpp"

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
 * Deslocamento para frente ou para trás com relação ao endereço.
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

/**
 * Lê um byte do vetor memória e retorna-o, estendendo o sinal para 32 bits. Lembrando que as
 * palavras da memória tem 4 bytes cada, para acessar um byte dentro da palavra pode-se:
 *   - Ler a palavra que contém o byte e, por operações de mascaramento, extrair byte endereçado, ou
 *   - Criar um ponteiro para byte e fazer um type cast (coerção de tipo) do endereço do vetor
       memória (int32_t *) para byte (int8_t *). Usando o ponteiro para byte pode-se acessar
       diretamente o dado desejado.
 *
 * @param address
 * Endereço especificado.
 *
 * @param kte
 * Deslocamento para frente ou para trás com relação ao endereço.
 *
 * @return int32_t
 */
int32_t lb(uint32_t address, int32_t kte)
{
  int8_t *byteMemory = (int8_t *)mem;
  return byteMemory[address + kte];
}

/**
 * Lê um byte do vetor memória e retorna-o como um número positivo, ou seja, todos os bits
 * superiores devem ser zerados.
 *
 * @param address
 * Endereço especificado.
 *
 * @param kte
 * Deslocamento para frente ou para trás com relação ao endereço.
 *
 * @return int32_t
 */
int32_t lbu(uint32_t address, int32_t kte)
{
  uint8_t *byteMemory = (uint8_t *)mem;
  return byteMemory[address + kte];
}

/** Escreve um inteiro alinhado na memória - endereços múltiplos de 4. O cálculo do endereço é
 * realizado da mesma forma que na operação lw().
 *
 * @param address
 * Endereço especificado.
 *
 * @param kte
 * Deslocamento para frente ou para trás com relação ao endereço.
 *
 * @param dado
 * Inteiro alinhado na memória.
 */
void sw(uint32_t address, int32_t kte, int32_t dado)
{
  if (address % 4 != 0)
  {
    std::cerr << "Error: address % 4 != 0" << std::endl;
    return;
  }
  else
  {
    uint32_t address_index = address / 4;
    uint32_t index = address_index + kte;
    mem[index] = dado;
    return;
  }
}

/**
 * Escreve um byte na memória. Caso utilize operações de mascaramento, a palavra que contém o byte
 * deve ser lida da memória, o byte deve ser posicionado corretamente através de deslocamentos e a
 * escrita ocorre utilizando máscaras. Alternativamente podese utilizar a coerção para (int8_t *) e
 * escrever diretamente no vetor memória.
 *
 * @param address
 * Endereço especificado.
 *
 * @param kte
 * Deslocamento para frente ou para trás com relação ao endereço.
 *
 * @param dado
 * Inteiro a ser armazenado.
 */
void sb(uint32_t address, int32_t kte, int8_t dado)
{
  int8_t *byteMemory = (int8_t *)mem;
  byteMemory[address + kte] = dado;
}
