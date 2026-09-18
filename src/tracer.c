#include "estruturas.h"

int main (int argc, char** argv) {

    int fd = open("FIFO",O_WRONLY);

    if (argc == 4 && strcmp(argv[1],"execute") == 0) {
        
        if (strcmp(argv[2], "-u") == 0) {

            Programa programa = malloc(sizeof(Programa));
            constroiStruct(argc,argv,programa);

            char** argumentos = constroiArray(argc,argv);

            struct timeval start;
            struct timeval end;

            pid_t pid = fork();

            if (pid == 0) {

                printf("Running PID %d\n", getpid());

                int result = execvp(argumentos[0],argumentos);
                if (result < 0){
                    perror("Erro no exec.");
                }

                _exit(-1);

            } else {

                gettimeofday(&start,NULL);
                programa->pid = pid;
                programa->time = start;
                write(fd,programa,10*sizeof(Programa));

                int status = 0;
                wait(&status);
                gettimeofday(&end,NULL);
                programa->time = end;
                write(fd,programa,sizeof(Programa));

                long miliseconds = (long) (end.tv_sec - start.tv_sec) * 1000;
                printf("Ended in %ld ms\n", miliseconds);
            }

        } else {
            if (strcmp(argv[2],"-p") == 0){
                
                int numProgs = numeroProgramas(argv[3]);
                char** pipeline = divideProgramas(argc,argv);

                Programa pipelineProgramas = malloc(sizeof(Programa));
                constroiStruct2(numProgs,pipeline,pipelineProgramas);

                int status;
                pid_t pidArray[numProgs];
                int pipes[numProgs-1][2];

                for (int i = 0; i < numProgs; i++) {

                    if (i == 0) {

                        if (pipe(pipes[i]) == -1){
                           
                            perror("Erro na criação do Pipe Anónimo.");
                            _exit(-1);

                        }

                        if ((pidArray[i] = fork()) == 0){

                            close(pipes[i][0]);
                            dup2(pipes[i][1],1);
                            close(pipes[i][1]);

                            char** programaExec = constroiArray2(pipeline[0]);

		                    execvp(programaExec[0],programaExec);
                            _exit(0);

                            } else {

                                printf("Running PID %d\n",getpid());
                                close(pipes[i][1]);

                            }

                        } else if (i == (numProgs-1)){

                            if ((pidArray[i] = fork()) == 0){

                                dup2(pipes[i-1][0],0);
                                close(pipes[i-1][0]);
                                close(pipes[i-1][1]);

                                char** programaExecLast = constroiArray2(pipeline[numProgs-1]);

		                        execvp(programaExecLast[0],programaExecLast);
                                    
                                _exit(0);

                            } else {

                                close(pipes[i-1][0]);
                                close(pipes[i-1][1]);

                            }

                        } else {

                            if (pipe(pipes[i]) == -1){
                           
                                perror("Erro na criação do Pipe Anónimo.");
                                _exit(-1);

                            }

                            if ((pidArray[i] = fork()) == 0){
                                
                                close(pipes[i][0]);
                                dup2(pipes[i-1][0],0);
                                close(pipes[i-1][0]);
                                dup2(pipes[i][1],1);
                                close(pipes[i][1]);

                                char** programaExecMiddle = constroiArray2(pipeline[i]);

		                        execvp(programaExecMiddle[0],programaExecMiddle);      

                                _exit(0);

                            } else {

                                close(pipes[i-1][0]);
                                close(pipes[i][1]);

                            }

                        }
                }

                for (int i = 0; i < numProgs; i++) {
                    waitpid(pidArray[i], &status,0);
                }

                /*gettimeofday(&start,NULL);
                pipelineProgramas->pid = getpid();
                pipelineProgramas->time = start;
                write(fd,pipelineProgramas,sizeof(Programa));
                    
                // esperar pelo final da pipeline
                waitpid(pidInicial, &statusFinal, 0);
                gettimeofday(&end,NULL);
                pipelineProgramas->time = end;
                write(fd,pipelineProgramas,sizeof(Programa));

                long miliseconds = (long) (end.tv_sec - start.tv_sec) * 1000;
                printf("Start : %ld\n", start.tv_sec);
                printf("End : %ld\n", end.tv_sec);
                printf("Ended in %ld ms\n", miliseconds);*/
                    
            }
        }
    }

    close(fd);
    return 0;
}

// Gestão automática dos Pids no Monitor
// Variavel Global guardada num ficheiro
// Atribuiçao dos Pids de forma manual