#include "estruturas.h"


int main (int argc, char** argv) {

    // Criação dos Named Pipes que efetuam a comunicação Cliente-Servidor.
    int res = mkfifo("FIFO",0640);

    if (res == -1) {
        perror("Erro na criação do FiFo.");
    }

    int fd = open("FIFO",O_RDWR);
    if (fd < 0) {
        perror("Erro na abertura do FiFo.");
    }

    //while(1);

    int bytes_read = 0;
    Programa programa = malloc(sizeof(Programa));

    while ((bytes_read = read(fd,programa,sizeof(Programa))) > 0){
        printf("Programa : %s", programa->programa);
    }


    /*int bytes_read = 0;
    char buffer[64];
    int global = -1;
    pid_t pid;

    Lista l = NULL;
    printf("%ld\n", tv);
        
    while ((bytes_read = read(fd,buffer,64)) > 0) {

        int teste=0;
        printf("ola3\n");
        global++;
        char* programa = strdup(buffer);
        for (int i = 0; i < 64; i++){
            buffer[i] = '\0';
        }
        ProgramaExec p = NULL;

        p = constroiExec(programa, p);
        Programa s = malloc(sizeof(Programa));
        s->pid = pid;
        s->programa = strdup(p->programa);
        s->output = NULL;

        l = addProgramaLista(s,l);
        contaTempo1(l, global);

        pid = fork();
    
        if (pid == 0) {
            
            int control = global;
            printf("%d\n", control);

            //imprimeTeste(l,control);
            printf("ola4\n");
            //printf("\n");

            _exit(1);

            //printf("%d fifo\n",bytes_read);
            //printf("Servidor: %s\n",buffer);

        } else {
            int status=0;
            wait(&status);
            
            lock_t end_time = clock();
            printf("%ld\n", l->p->clock);
            printf("%ld\n", end_time);

            imprimeTempo1(l,end_time);
    */
    
    
    unlink("FIFO");
    return 0;
}