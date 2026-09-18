#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define SIZE 64

typedef struct Programa{
    pid_t pid;
    char programa[64];
    struct timeval time;
} *Programa;

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

char** constroiArray (int argc, char** argv) {

    char* programa = strdup(argv[3]);
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

char** constroiArray2 (char* programaIn) {

    char* programa = strdup(programaIn);
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

/*
struct pedido
{
    char* file;
    char* output;
    char* transforms;
    char* id;
    int priority;
} *PEDIDO;

void executaPedido(PEDIDO ped){

    char* resposta = malloc(100 * sizeof(char));
    int nt = getNtransforms(ped)-1;
    char** transforms = listaT(ped);
    char* inF = getFilePath(ped);
    char* outF = getOutFilePath(ped);

    int fi = open(inF,O_RDONLY,S_IRWXU);
    int fo = open(outF,O_CREAT | O_WRONLY,S_IRWXU);

    pid_t pidloop;
    int stapl;

    if((pidloop=fork())==0){

        //executa pedido se só tiver uma transformação
        if(nt==1){
            pid_t pidP;
            int statT;
            
	        if((pidP=fork())==0){	
                dup2(fi,0); //direciona stdin para ficheiro de input
		        dup2(fo,1); //direciona stdout para ficheiro de output

                char* aux = malloc(50*sizeof(char));
                strcat(aux,"../SDStore-transf/./");
                strcat(aux,transforms[0]);	
		        execlp(aux,aux, NULL);

		        close(fi);
		        close(fo);
                exit(0);		
            }

            pid_t cpid = waitpid(pidP, &statT, 0);
            if (WIFEXITED(statT)){
                write(2,"so1",3);
            }
            else{
                write(2,"no1",3);
            }
            exit(0);
        }
        else{ //executa pedido com várias transformações

            pid_t pidT[nt]; //array com pids atuais
            int i;
            int status;
            int pipes[nt-1][2]; //pipes para comunicar entre processos

            for (i = 0; i < nt; i++)
            {
                if(i==0){ //primeira transformação

                    if(pipe(pipes[i])<0){ //inicializa pipe
                        perror("pipe");
                    }
                    if((pidT[i]=fork())==0){
                        close(pipes[i][0]);      //fecha leitura do primeiro pipe
                        dup2(pipes[i][1],1);     //direciona stdout para escrita do pipe
                        close(pipes[i][1]);      //fecha escrita do primeiro pipe
                        dup2(fi,0);              //direciona stdin para ficheiro de input

                        char* aux = malloc(50*sizeof(char));
                        strcat(aux,"../SDStore-transf/./");
                        strcat(aux,transforms[i]);	
		                execlp(aux,aux, NULL);   //executa a transform

                        exit(i);
                    }
                    close(pipes[i][1]); //fecha leitura do pipe
                }
                else if(i==nt-1){ //última transformação

                    close(pipes[i-1][1]); //fechar escrita do pipe anterior

                    if((pidT[i]=fork())==0){
                        dup2(pipes[i-1][0],0);    //direciona stdin para leitura do pipe
                        close(pipes[i-1][0]);     //fecha leitura do ultimo pipe
                        dup2(fo,1);               //direciona stdout para ficheiro de output

                        char* aux = malloc(50*sizeof(char));
                        strcat(aux,"../SDStore-transf/./");
                        strcat(aux,transforms[i]);	
		                execlp(aux,aux, NULL);    //executa a transform

                        exit(i);
                    }
                    close(pipes[i-1][0]); //fecha leitura do pipe anterior
                }
                else{
                    if(pipe(pipes[i])<0){
                        perror("pipe");
                    }
                    if((pidT[i]=fork())==0){
                        dup2(pipes[i-1][0],0);    //direciona stdin para leitura do pipe anterior
                        dup2(pipes[i][1],1);      //direciona stdout para escrita do pipe
                        close(pipes[i-1][0]);     //fecha leitura do pipe anterior
                        close(pipes[i][1]);       //fecha escrita do pipe 

                        char* aux = malloc(50*sizeof(char));
                        strcat(aux,"../SDStore-transf/./");
                        strcat(aux,transforms[i]);	
		                execlp(aux,aux, NULL);    //executa a transform
                        
                        exit(i);
                    }
                    close(pipes[i-1][0]);         //fecha leitura do pipe anterior
                }
            }
            //esperar que todas as transformações sejam executadas
            for (int i=0; i<nt; i++){

                pid_t cpid = waitpid(pidT[i], &status, 0);
                if (WIFEXITED(status)){
                    char* a = malloc(100 * sizeof(char));
                    sprintf(a,"transf n: %d\n", i);
                    write(2,a,strlen(a));
                }
                else{
                    write(2,"not2\n",6);
                }
            }
            exit(0);
        }
    }
    //esperar final do pedido
    pid_t toupid = waitpid(pidloop, &stapl, 0);
    // write(1,terminou,atualizacaodotempo)
    if (WIFEXITED(stapl)){
        int bytesin = countBytes(inF);
        int bytesout = countBytes(outF);
        sprintf(resposta,"concluded (bytes-input: %d, bytes-output: %d)\n", bytesin, bytesout);
        write(2,resposta,strlen(resposta));
    }
    else{
        write(2,"Nfez",4);
    }

    close(fi);
	close(fo);
}*/

/*
void executaPipeline (char** programas, int numProgs) {

    pid_t pidInicial;
    pid_t pidArray[numProgs]; //array com pids atuais
    int i = 0;
    int status;
    int statusFinal;
    int pipes[numProgs-1][2]; //pipes para comunicar entre processos

    if ((pidInicial = fork()) == 0) {
        for (i = 0; i < numProgs; i++){
            
            if (i == 0) { //primeira transformação

                if (pipe(pipes[i]) < 0){ //inicializa pipe
                    perror("Erro na criação do Pipe Anónimo.");
                }

                if ((pidArray[i] = fork()) == 0){
                    
                    close(pipes[i][0]);      //fecha leitura do primeiro pipe
                    dup2(pipes[i][1],1);     //direciona stdout para escrita do pipe
                    close(pipes[i][1]);      //fecha escrita do primeiro pipe

                    char** programaExec = constroiArray2(programas[0]);
                    for (int l = 0; programaExec[l] != NULL; l++){
                        //printf("%s\n",programaExec[l]);
                    }
		            execvp(programaExec[0],programaExec);   //executa a transform

                    _exit(0);
                }
            
                close(pipes[i][1]); //fecha leitura do pipe
            }   else if (i == (numProgs - 1)) { //última transformação

                close(pipes[i-1][1]); //fechar escrita do pipe anterior

                if ((pidArray[i] = fork()) == 0) {

                    dup2(pipes[i-1][0],0);    //direciona stdin para leitura do pipe
                    close(pipes[i-1][0]);     //fecha leitura do ultimo pipe

                    char** programaExecLast = constroiArray2(programas[numProgs-1]);
                    for (int l = 0; programaExecLast[l] != NULL; l++){
                        //printf("%s\n",programaExecLast[l]);
                    }

		            execvp(programaExecLast[0],programaExecLast);    //executa a transform

                    _exit(0);
                }

                close(pipes[i-1][0]); //fecha leitura do pipe anterior

            }   else {

                if (pipe(pipes[i]) < 0){
                    perror("Erro na criação do Pipe Anónimo.");
                }
                    
                if ((pidArray[i] = fork()) == 0){

                    dup2(pipes[i-1][0],0);    //direciona stdin para leitura do pipe anterior
                    dup2(pipes[i][1],1);      //direciona stdout para escrita do pipe
                    close(pipes[i-1][0]);     //fecha leitura do pipe anterior
                    close(pipes[i][1]);       //fecha escrita do pipe 

                    char** programaExecMiddle = constroiArray2(programas[i]);
                    for (int l = 0; programaExecMiddle[l] != NULL; l++){
                        //printf("%s\n",programaExecMiddle[l]);
                    }

		            execvp(programaExecMiddle[0],programaExecMiddle);    //executa a transform        
                        
                    _exit(0);
                }

                close(pipes[i-1][0]);         //fecha leitura do pipe anterior
            }
        }

        //esperar que todas as transformações sejam executadas
        for (int j = 0; j < numProgs; j++){
            waitpid(pidArray[i], &status, 0);
        }
        
        _exit(0);
       
        // esperar pelo final da pipeline
        waitpid(pidInicial, &statusFinal, 0);
        // write(1,terminou,atualizacaodotempo)
    }
}*/

int main (int argc, char** argv) {

    int numProgs = numeroProgramas(argv[3]);
    printf("Programas : %d\n", numProgs);

    char** programas = divideProgramas(argc,argv);

    for (int i = 0; i < numProgs; i++){
        printf("%s\n",programas[i]);
    }

    Programa programa = malloc(sizeof(Programa));

    //executaPipeline(programas,numProgs);

    constroiStruct2(numProgs,programas,programa);

    printf("%s\n", programa->programa);

    return 0;
}

    /*for(int i=0;i<n_op;i++){

                        opIndex = 4+i;

                        //No último comando, não é necessário criar pipe
                        if(i!=(n_op-1)){
                            if(pipe(p[i])==-1){
                                perror("Erro ao abrir o pipe: ");
                                exit(-1);
                            }
                        }

                        //Primeiro argumento, em que não lêmos de um pipe,
                        //mas sim do pedido original e escrevemos num pipe
                        if(i==0){

                            if(fork()==0){

                                close(p[i][0]);
                                //int fdInput = open(pedidoArr[2],O_RDONLY);

                                //dup2(fdInput,0);
                                //close(fdInput);
                                dup2(p[i][1],1);
                                close(p[i][1]);

                                char command[CommandSize];
                                sprintf(command,"%s%s",argv[2],pedidoArr[opIndex]);
                                execl(command,command,NULL);

                                // só será usado se o exec se vergar, não esquecer de ver isto!!
                                //talvez executar o _exit???
                                exit(-1);
                            }
                            else{

                                close(p[i][1]);

                            }

                        }

                        //Último argumento, que o conteúdo é lido de um pipe
                        //e escrito para o ficheiro de output recebido no pedido
                        else if(i==(n_op-1)){

                            if(fork()==0){
                                
                                int fdOutput = open(pedidoArr[3],O_WRONLY | O_CREAT,0666);

                                dup2(fdOutput,1);
                                close(fdOutput);
                                dup2(p[i-1][0],0);
                                close(p[i-1][0]);

                                char command[CommandSize];
                                sprintf(command,"%s%s",argv[2],pedidoArr[opIndex]);
                                execl(command,command,NULL);

                                // só será usado se o exec se vergar, não esquecer de ver isto!!
                                exit(-1);

                            }
                            else{

                                close(p[i-1][0]);                                    

                            }

                        }

                        //Caso intermédio de escrita do conteúdo em pipes
                        else{

                            if(fork()==0){
                                
                                close(p[i][0]);
                                dup2(p[i-1][0],0);
                                close(p[i-1][0]);
                                dup2(p[i][1],1);
                                close(p[i][1]);

                                char command[CommandSize];
                                sprintf(command,"%s%s",argv[2],pedidoArr[opIndex]);
                                execl(command,command,NULL);

                                // só será usado se o exec se vergar, não esquecer de ver isto!!
                                exit(-1);

                            }
                            else{

                                close(p[i-1][0]);
                                close(p[i][1]);

                            }

                        }

                    }

                    // Esperar para que todos os filhos
                    for(int i=0;i<n_op;i++) wait(NULL);*/