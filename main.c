#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main () {

    


int idProducent = fork();


    switch(idProducent){

        //obsługa błędów fork()
        case -1:
            perror("Producent fork() error");
            exit(1);

        //pierwszy proces potomny zostaje producentem
        case 0:
            execlp("ls", "ls", NULL);
     
            //obsługa błędów execlp()
            perror("Producent execlp() error");
            exit(1);
           
        default:
            printf("konsument0");
            break;
    }
}