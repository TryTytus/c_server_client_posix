//
// Created by Piotr Bartman on 3/27/23.
//

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024
#define CMD_MAX 32

/**
 * (i) Zaimplementować program tworzący proces serwera.
 *     Serwer tworzy ogólnodostępną kolejkę FIFO (podaną jako argument), i czeka na zgłoszenia klientów.
 *     Klient przesyła request w postaci: <path/to/client/fifo>:<data>
 *     Po otrzymaniu request'a na kolejkę serwer zmienia przysłane dane na wielkie litery
 *     i odsyła klientowi na wskazaną przez niego kolejkę fifo.
 *
 * (ii) Zmodyfikować poprzedni program, tak, by kolejka utworzona przez klienta była dwukierunkowa,
 *      klient publiczną kolejką powinien przesyłać nazwę stworzonej przez siebie kolejki.
 *      Dalsza wymiana komunikatów powinna odbywać się poprzez kolejkę stworzoną przez klienta.
 *      Klient kolejką tą powinien wysyłać polecenia, zadaniem serwera jest wykonywanie tych
 *      poleceń i odsyłanie wyników.
*/

int countWords(char str[]) {
    int i = 0;
    int count = 1;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
            count++;
        i++;
    }
    return count;
}

int countChars(char str[]) {
    int i = 0;
    while (str[i] != '\0' && str[i] != ' ')
    {
        i++;
    }
    return i;
}

char** makeArgsArr(char* buf, int wordCount) {
    char** argsArr = (char**) malloc(sizeof(char*) * (wordCount + 1));
    
    int last = 0;
    for (int i = 0; i < wordCount; i++)
    {   
        int start = last;
        int count = 0;
        while (buf[last] != '\0' && buf[last] != ' ')
        {
            count++;
            last++;
        }

        char* str = (char*) malloc(sizeof(char) * (count + 2));        
        for (int j = 0; j < count; j++)
        {
            str[j] = buf[start + j];
        }
        str[start + count] = '\0';



        argsArr[i] = str;
        last += 1;  
    }
    argsArr[wordCount] = NULL;
    return argsArr;
    
}


void handle_request(int client_fifo, char* data) {
    // TODO toupper and write to client_fifo
}


int main(int argc, char *argv[]) {
    // TIP: serwer powinnien odczytywać dane z kolejki FIFO w nieskończonej pętli.
    // TODO: create FIFO

    // while (1) {
    //     // TODO read and parse request
    //     handle_request(client_fifo, data);
    // }


    char serverPath[] = "SERVER_FIFO";
    char buf[BUF_SIZE];

    if (mkfifo(serverPath, 0777) == -1) {
        fprintf(stderr, "Błąd tworzenia kolejki FIFO");
        exit(1);
    }

    printf("SERVER LISTENING ON: %s\n", serverPath);
    printf("WAITING: FOR CLIENT...\n");

    
    
    while (1)
    {
        int server = open(serverPath, O_RDONLY);
        printf("CONNECTED\n");

        int bytes = read(server, buf, sizeof(buf));

        printf("BYTES READ FROM CLIENT: %d\n", bytes);

        // connect to client
        if (bytes > 0) {
            close(server);

            printf("CONNECTING TO CLIENT PATH: %s\n", buf);

            do {
            int client = open(buf, O_RDONLY);
            if (client == -1)
                printf("ERROR\n");
            printf("CONNECTED TO CLIENT\n");

            int bytes = read(client, buf, sizeof(buf));

            printf("READED\n");
            printf("COMMAND TO EXC: %s\n", buf);

            if (bytes == BUF_SIZE)
                    printf("ERROR: BUF OVERFLOW\n");

            close(client);
            } while (bytes == 0);

            int words = countWords(buf);

            char** agrsArr = makeArgsArr(buf, words);

            //sleep(1);
            
            int pdesk;
            switch (fork())
            {
            case -1:
                printf("ERROR\n");
                break;
            case 0:
                close(1);
                pdesk = open("CLIENT_FIFO", O_WRONLY);
                if (pdesk == -1) {
                    fprintf(stderr, "Otwarcie potoku do zapisu");
                    exit(1);
                }
                else if (pdesk != 1) {
                fprintf(stderr, "Niewlasciwy deskryptor do zapisu\n");
                exit(1);
                }
                execvp(agrsArr[0], agrsArr);
                break;
            default:
                printf("SENDING TO CLIENT...\n");
                unlink(serverPath);
                exit(0);
                break;
            }


        }

    }

}