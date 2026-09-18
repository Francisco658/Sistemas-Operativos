# Sistemas Operativos - Trabalho Prático

Trabalho Prático no âmbito da Unidade Curricular de Sistemas Operativos

### Grupo de Trabalho:
- [Afonso Bessa](https://github.com/AsseB2519)
- [João Barroso](https://github.com/JoaoBarroso25)
- [Francisco Claudino](https://github.com/carapokebao)

**<ins> Grupo </ins>**
* [Afonso Bessa](https://github.com/AsseB2519) - a95225
* [Francisco Claudino](https://github.com/carapokebao) - a89493
* [João Barroso](https://github.com/JoaoBarroso25) - a95195

**Licenciatura em Engenharia Informática**

**Universidade do Minho (2021/2022)**

## Rastreamento e Monitorização da Execução de Programas

Este Trabalho Prático tem como objetivo implementar um serviço de monitorização de programas em uma máquina. 

O serviço deve permitir que os utilizadores executem programas através de um **Cliente** e obtenham o tempo de execução total. Além disso, um **Administrador** de sistemas deve ser capaz de visualizar todos os programas atualmente em execução e o tempo gasto por cada um. O **Servidor** também deve fornecer estatísticas sobre programas já terminados, como o tempo de execução agregado de um conjunto de programas. 

Em suma, este projeto envolve o desenvolvimento de um sistema que permite aos utilizadores monitorar o desempenho de programas em uma máquina e obter estatísticas úteis sobre eles.

### Programa Cliente e Servidor

Este projeto consiste no desenvolvimento de dois programas: 
- **Cliente** (chamado "tracer"),
- **Servidor** (chamado "monitor"). 

O **Cliente** deve ser criado com uma interface de linha de comando para interagir com o utilizador. O **Servidor** deve manter informações relevantes na memória e em arquivos para suportar as funcionalidades descritas a seguir. O **Cliente** deve usar a saída padrão para apresentar as respostas necessárias ao utilizador, enquanto o **Servidor** pode usar a saída padrão para apresentar informações de depuração, se necessário.

### Funcionalidades

#### Básicas
- Execução de programas do utilizador
- Consulta de programas em execução
- Servidor
#### Avançadas
- Execução encadeada de programas
- Armazenamento de informação sobre programas terminados
- Consulta de programas terminados

Mais promenor sobre cada uma destas funcionalidades, bem como, Interface e Modo de Utilização ou até da Makefile encontra-se no [Enunciado](Enunciado_do_Trabalho_Prático.pdf) de apresentação do Projeto.
