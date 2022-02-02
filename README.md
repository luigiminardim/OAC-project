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

# Respostas às perguntas do projeto 1.

> Resposta à pergunta: qual a diferença entre endereços de bytes, half word e word?

Dado um espaço de memória, a diferença entre o byte, half word e word é a quantidade de bits que
esse espaço contém. 1 byte se refere à 8 bits, uma half word se refere a 16 bits e um word se refere
a 32 bits.

> Qual compilador empregado?

Neste projeto se utilizou o g++ como compilador. Esta escolha foi feita para se aproveitar dos
recursos de formatação na saída de strings.

> Qual sistema operacional utilizado?

O sistema operacional utilizado para desenvolvimento foi o
(Ubuntu 20.04.3 LTS)[https://ubuntu.com/download/desktop]

> Qual IDE utilizada (Eclipse, XCode, etc)?

Para o desenvolvimento desse projeto não foi utilizada nenhuma IDE. Mas foi utilizado o editor de
textos (Visual Studio Code)[https://code.visualstudio.com/].
