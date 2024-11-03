// Systemy Operacyjne, Laboratorium 2023
// Uniwersytet Jagielloński w Krakowie
// Piotr Bartman

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include <fcntl.h>
// #include <unistd.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>



/**
 * (i) Zaimplementować program tworzący proces klienta.
 *     Klient tworzy własną kolejkę, poprzez którą będą przychodzić odpowiedzi serwera.
 *     Zadaniem klienta jest przesłanie nazwy stworzonej przez siebie kolejki,
 *     a serwera odesłaniem poprzez kolejkę stworzoną przez klienta wyniku działania.
 *
 * (ii) Zmodyfikować poprzedni program, tak, by kolejka utworzona przez klienta była "dwukierunkowa",
 *      klient publiczną kolejką powinien przesyłać nazwę stworzonej przez siebie kolejki.
 *      Dalsza wymiana komunikatów powinna odbywać się poprzez kolejkę stworzoną przez klienta.
 *      Klient kolejką tą powinien wysyłać polecenia, zadaniem serwera jest wykonywanie tych
 *      poleceń i odsyłanie wyników.
 *
 */


int main(int argc, char *argv[]) {

    char clientPath[] = "CLIENT_FIFO";
    char colon = ':';
    char buf[1024];


    if (mkfifo(clientPath, 0777) == -1) {
        fprintf(stderr, "Błąd tworzenia kolejki FIFO");
        exit(1);
    }

    int server = open("SERVER_FIFO", O_WRONLY);

    write(server, clientPath, sizeof(clientPath));
    close(server);
    printf("SEND PATH TO SERVER\n");


    int clinet = open(clientPath, O_WRONLY);
    if (clinet == -1)
     printf("ERROR\n");

    printf("OPENED CONNECTION\n");
    char command[] = "ls -l";
    //sleep(2);
    write(server, command, sizeof(command));
    close(clinet);

    int bytes;

    printf("TRYING TO READ\n");
    do
    {
        int client  = open(clientPath, O_RDONLY);
        
        bytes = read(client, buf, sizeof(buf));
        close(client);

    } while (bytes == 0);
    
    printf("%s\n", buf);
    unlink(clientPath);

}
