#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SIZE 64


/*typedef struct Programa{
    pid_t pid;
    char *flag;
    char *output;
    double time;
} *Programa;

typedef struct ProgramaQueue{
    pid_t pid;
    char *programa;
    double time;
} *ProgramaQueue;

typedef struct ProgramaExec{
    char *flag;
    char *programa;
    char **argumentos;
} *ProgramaExec;*/

/*typedef struct ProgramaExec{
    char *flag;
    char *programa;
    char **argumentos;
} *ProgramaExec;*/

typedef struct Programa{
    pid_t pid;
    char programa[64];
    struct timeval time;
} *Programa;

/*typedef struct ProgramaLista{
    Programa p;
    struct ProgramaLista *prox;
} *Lista;*/

/*
Programa programaPosicao (Lista l, int posicao);

Lista status (Lista);

Programa removePosicao (Lista, int);

Lista addProgramaLista (Programa, Lista);

void contaTempo (Lista, int);

void imprimeTeste(Lista,int);

void contaTempo1 (Lista, int);

void imprimeTempo1 (Lista,clock_t);*/

int numeroArgumentos(char *);

int numeroProgramas(char *programa);

void constroiStruct(int argc, char** argv, Programa p);

void constroiStruct2(int numProgs, char** pipeline, Programa programa);

char** constroiArray (int argc, char** argv);

char** constroiArray2 (char* programaIn);

char** divideProgramas (int argc, char** argv);