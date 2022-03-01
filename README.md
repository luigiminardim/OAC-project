# Projeto de Organização de Arquitetura de Computadores

Este projeto consiste na simulação de instruções do RISCV RV32I em linguagem C.
O respositório desse projeto pode ser encontrado em https://github.com/luigiminardim/OAC-project.

Neste projeto, foram implementadas as instruções dos RISCV:
- add
- addi
- and
- andi
- auipc
- beq
- bne
- bge
- bgeu
- blt
- bltu
- jal
- jalr
- lb
- or
- lbu
- lw
- lui
- sltu
- ori
- sb
- slli
- slt
- srai
- srli
- sub
- sw
- xor
- ecall (print string, print integer, exit)

Para verificar se o simulador está funcionando corretamente deve-se utilizar o
(RARS)[https://github.com/TheThirdOne/rars] para geração de códigos de teste que incluam código
executável e dados. Os arquivo de código executável juntamente com o arquivo de dados verificam se
todas as instruções implementadas no simulador funcionam corretamente.
O código assembly que gerou os binários de dados e teste pode ser encontrado em `./assets/test.asm`.
Os binários de dados e código gerados por `./assets/test.asm` são respectivamente
`./assets/data.bin` e `./assets/code.bin`. Para testar o simulador utilizando os arquivos binários
supracitados, basta utilizar o comando `make run`, em que o usuário deve ver uma saída como:

```
Teste1 OK
Teste2 OK
Teste3 OK
Teste4 OK
Teste5 OK
Teste6 OK
Teste7 OK
Teste8 OK
Teste9 OK
Teste10 OK
Teste11 OK
Teste12 OK
Teste13 OK
Teste14 OK
Teste15 OK
Teste16 OK
Teste17 OK
Teste18 OK
Teste19 OK
Teste20 OK
Teste21 OK
Teste22 OK
```

Se o usário desejar trocar os arquivos binários de teste, basta montar os binários de dados e código
no (RARS)[https://github.com/TheThirdOne/rars]  e substituir os arquivos gerados em `./assets/` com
os mesmos nomes.

> Não se esqueça de ativar a opção "Text at Address 0" na hora de exportar os binários.

Há também testes unitários para funções internas do simulador. Estes testes unitários podem ser
vistos com o comando `make test`, que deve gerar uma saída como:

```
./build/test.exe
Error: address % 4 != 0
===============================================================================
All tests passed (176 assertions in 10 test cases)
```

## Como Utilizar

### Dependências

- [g++ 9.3.0](https://www.geeksforgeeks.org/compiling-with-g-plus-plus/): O comando g++ é um comando
  de invocação do compilador GNU c++, que é utilizado para o pré-processamento, compilação, montagem
  e ligação do código fonte para gerar um arquivo executável. As diferentes "opções" do comando g++
  permitem-nos parar este a execução do processo na fase intermédia.
- [GNU Make 4.2.1](https://www.gnu.org/software/make/): GNU Make é uma ferramenta que controla a
  geração de executáveis e outros ficheiros não-fonte de um programa a partir dos ficheiros-fonte do
  programa.

### Comandos

- `make build`: Produz o executável em `build/oac.exe`;
- `make run`: Gera o mesmo executável de `make build` e o inicia.
- `make test`: Gera o executável de teste e o inicia.
- `make clean`: Remove os arquivos objeto da pasta `build/objs`.
- `make reset`: Remove os arquivos objeto da pasta `build/`.
