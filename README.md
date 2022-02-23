# Projeto de Organização de Arquitetura de Computadores

Este projeto consiste na simulação de instruções do RISCV RV32I em linguagem C.

O respositório desse projeto pode ser encontrado em https://github.com/luigiminardim/OAC-project.

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
- `make run`: Se não houver o executável (`make build`), gera o executável e o inicia.
- `make test`: Gera o executável de teste e o inicia.
- `make clean`: Remove os arquivos objeto da pasta `build/objs`.
- `make reset`: Remove os arquivos objeto da pasta `build/`.
