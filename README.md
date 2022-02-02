# Projeto de Organização de Arquitetura de Computadores

Este trabalho consiste na simulação de instruções de acesso à memória do RISCV RV32I em
linguagem C.

## Como Utilizar

### Dependências

- [g++ 9.3.0](https://www.geeksforgeeks.org/compiling-with-g-plus-plus/): O comando g++ é um comando
  de invocação do compilador GNU c++, que é utilizado para o pré-processamento, compilação, montagem
  e ligação do código fonte para gerar um arquivo executável. As diferentes "opções" do comando g++
  permitem-nos parar este a execução do processo na fase intermédia.
- [GNU Make 4.2.1](https://www.gnu.org/software/make/): GNU Make é uma ferramenta que controla a
  geração de executáveis e outros ficheiros não-fonte de um programa a partir dos ficheiros-fonte do
  programa.

### Execução do programa

Para executar o programa é primeiro necessário produzir o arquivo executável. Para isso basta abrir
o terminal na pasta raiz e executar:
```sh
make
```
com isto, será produzido o arquivo executável `oac.exe` na pasta raiz. Agora basta apenas executar o
arquivo no terminal.
```sh
./oac.exe
```

### Execução dos testes
Para realizar a execução dos testes, basta utilizar o comando:
```sh
make tests
```
Este comando irá produzir o arquivo executável dos testes (`test.exe`) e executá-lo