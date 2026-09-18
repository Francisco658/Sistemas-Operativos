#include "estruturas.h"


/*Programa programaPosicao (Lista l, int posicao){

    Programa p = malloc(sizeof(Programa));

    if(l!=NULL){
        for(int i=0; i < posicao; i++){
            if(l==NULL) break;
            l=l->prox;
        }

    if(l == NULL){
        p=NULL;
        return p;
    }

    p = l->p;

    } else p = NULL;

    return p;
}
*/

/*
Lista status (Lista l){
    Lista aux = l;

    while(aux != NULL){
        aux->p->time = 0; //stop time;
        aux=aux->prox;
    }


    return l;
}*/

/*
Programa removePosicao (Lista l, int posicao){      
// devolve sempre bem. Se for o primeiro elemento nao atualiza a lista em condicoes ver amanha

    Programa p = malloc(sizeof(Programa));

    if(l==NULL) return NULL;

    if(posicao==0){
        p = l->p;
        Lista aux = l;
        l = l->prox;
        aux=NULL;
        free(aux);
    } else{
        Lista ant = l;
        Lista next = l;
        int i=0;

        for(; i < posicao; i++){
            ant = next;
            next = next ->prox;
            if(next ->prox == NULL) break;
        }
        i++;
        if(i < posicao) return NULL;
        p = next->p;
        ant->prox = next->prox;
        next=NULL;
        free(next);
    }

    return p;
}

Lista addProgramaLista (Programa p, Lista l){

    if(l==NULL){

        l = malloc(sizeof(Lista));
        l->p = p;
        l->prox=NULL;

    } else{

        Lista aux = l;

        while(aux->prox!=NULL){
            aux = aux->prox;
        }
        aux->prox = malloc(sizeof(Lista));
        aux->prox->p = p;
        aux->prox->prox = NULL;
        

    }

    return l;
}*/


/*void contaTempo (Lista l, int pos){

	Lista aux = l;

	for(int i=0; i<pos; i++){
		aux=aux->prox;
	}

    l->p->time = 0;
	time_t t = time(NULL);
    l->p->time = (double)t;

}

void imprimeTeste(Lista l,int control){
    Lista aux=l;

    for(int i=0; i<control; i++) aux=aux->prox;

    printf("%f\n", aux->p->time);

}

void contaTempo1 (Lista l, int pos){

    Lista aux = l;

    for(int i=0; i<pos; i++){
        aux=aux->prox;
    }

    aux->p->clock = clock();

}

void imprimeTempo1 (Lista l, clock_t end_time){

    l->p->time = (double)(end_time - l->p->clock);
    l->p->time = (double) (l->p->time / CLOCKS_PER_SEC);   
    printf("%f\n", l->p->time);
}
*/

/*
void constroiStruct(int argc,char** argv, Programa p){

    p=malloc(sizeof(Programa));
    p->programa = strdup(argv[2]);

}*/


int numeroArgumentos(char *programa) {
    
    int contador = 1;
    for (int i = 0; programa[i] != '\0'; i++){
        if (programa[i] == ' '){
            contador++;
        }
    }
    return contador;
}

int numeroProgramas(char *programa) {
    int contador = 1;
    for (int i = 0; programa[i] != '\0'; i++){
        if (programa[i] == '|'){
            contador++;
        }
    }
    return contador;
}

void constroiStruct(int argc, char** argv, Programa p){

    if (p != NULL) {
        char *aux = strdup(argv[3]);
        const char sep[2] = " ";
        char *token = strtok(aux, sep);
        strcpy(p->programa, token);
    }

}

void constroiStruct2(int numProgs, char** pipeline, Programa programa){

    if (programa != NULL) {
        char* programaPipeline = (char*) malloc(SIZE * sizeof(char*));

        for (int j = 0; j < numProgs; j++){
            char** argumentos = constroiArray2(pipeline[j]);
            for (int k = 0; argumentos[k] != NULL; k++) {
                if (k == 0) {
                    strcat(programaPipeline,argumentos[0]);
                    strcat(programaPipeline," | ");
                }
            }
        }

        programaPipeline[strlen(programaPipeline)-2] = '\0';
        strcpy(programa->programa,programaPipeline);
    }
}

char** constroiArray (int argc, char** argv) {

    char* programa = strdup(argv[3]);
    int numArgs = numeroArgumentos(programa);

    char** arrayArgumentos = malloc((numArgs+1) * sizeof(char*));

    for (int i = 0; i < numArgs; i++){
        arrayArgumentos[i] = (char*) malloc(SIZE * sizeof(char*));
    }

    int j = 0;
    char* token = strtok(programa, " ");

    for (j = 0; token != NULL; j++) {
        arrayArgumentos[j] = strdup(token);
        token = strtok(NULL," ");
    }
    arrayArgumentos[j] = NULL;

    return arrayArgumentos;
            
}

char** constroiArray2 (char* programaIn) {

    char* programa = strdup(programaIn);
    int numArgs = numeroArgumentos(programa);

    char** arrayArgumentos = malloc((numArgs+1) * sizeof(char*));

    for (int i = 0; i < numArgs; i++){
        arrayArgumentos[i] = (char*) malloc(SIZE * sizeof(char*));
    }

    int j;
    char* token = strtok(programa, " ");

    for (j = 0; token != NULL; j++) {
        arrayArgumentos[j] = strdup(token);
        token = strtok(NULL," ");
    }
    arrayArgumentos[j] = NULL;

    return arrayArgumentos;
            
}

char** divideProgramas (int argc, char** argv) {

    char* pipeline = strdup(argv[3]);
    int numProgs = numeroProgramas(pipeline);

    char** arrayProgramas = malloc((numProgs+1) * sizeof(char*));

    for (int i = 0; i < numProgs; i++) {
        arrayProgramas[i] = (char*) malloc(SIZE * sizeof(char*));
    }

    int j;
    char* token = strtok(pipeline, "|");

    for (j = 0; token != NULL; j++){
        arrayProgramas[j] = strdup(token);
        token = strtok(NULL,"|");
    }

    return arrayProgramas;

}