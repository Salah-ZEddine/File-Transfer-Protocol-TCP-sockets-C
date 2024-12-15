#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT 3490
#define IPADD "127.0.0.1"
#define BACKLOG 5

void send_file(FILE *file, int new_fd){

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(new_fd, buffer, bytes_read, 0) == -1)
        {
            perror("Envoie de fichier non réussie");
            break;
        }
    }

    printf("File transfer complete\n");

    fclose(file);
}

int main()
{

    int sockfd, new_fd;            /* Ecouter sock_fd, nouvelle connection sur new_fd */
    struct sockaddr_in my_addr;    /* Adresse */
    struct sockaddr_in their_addr; /* Adresse du connecté  */
    int sin_size;

    char buffer[1024];
    FILE *file;

    if (sockfd = socket(AF_INET, SOCK_STREAM, 0) == -1)
    {
        perror("Erreur de creation de socket");
        exit(1);
    }
    printf("Socket cree avec succes \n");

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
    bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Erreur de binding");
        exit(1);
    }

    printf("BInding succesfull");

    /*Attendant la connection au niveau client */

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("Erreur de listening");
        exit(1);
    }
    printf("Entrain d'entendre les connections\n");

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);

        if (new_fd = accept(sockfd, (struct sockaddr *)&their_addr, sin_size) == -1)
        {
            perror("Erreur de connection");
            continue;
        }

        printf("Connection Reussite!!\n");

        file = fopen("FILE", "rb");
        if (file == NULL)
        {
            perror("Erreur d ouverture de fichier");
            close(new_fd);
            continue;
        }

        printf("Fichier ouvert \n");
        send_file(file, new_fd);
        fclose(file);
        close(new_fd);
    }

    close(sockfd);

    return 0;
}