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
#define MAXDATASIZE 1024

void send_file(FILE *file, int new_fd){

    char buffer[MAXDATASIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(new_fd, buffer, bytes_read, 0) == -1)
        {
            perror("[-]Envoie de fichier non réussie");
            perror("[-]Envoie de fichier non réussie");
            break;
        }
    }

    printf("[+]File transfer complete\n");
    printf("[+]File transfer complete\n");

    fclose(file);
}

void receive_file(FILE *file,int sockfd){
    char buffer[MAXDATASIZE];
    size_t bytes_read;
    while ((bytes_read = recv(sockfd, buffer, MAXDATASIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_read, file);
    }

    if (bytes_read == -1) {
        perror("[-]Erreur de réception");
        exit(1);
    }
    printf("[+]Fichier reçu avec succes\n");
}

void openFile(FILE *file,char fileName[256], char permission[25]){
    file = fopen(fileName, permission);
        if (file == NULL) {
            perror("[-]Erreur d'ouverture du fichier");
            exit(1);
        }
        printf("[+]Fichier ouvert sans prob\n");
}

int main()
{

    int sockfd, new_fd;            /* Ecouter sock_fd, nouvelle connection sur new_fd */
    struct sockaddr_in my_addr;    /* Adresse */
    struct sockaddr_in their_addr; /* Adresse du connecté  */
    socklen_t sin_size;
    char mode;

    char buffer[MAXDATASIZE];
    FILE *file;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[-]Erreur de creation de socket");
        perror("[-]Erreur de creation de socket");
        exit(1);
    }
    printf("[+]Socket cree avec succes \n");
    printf("[+]Socket cree avec succes \n");

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
    bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("[-]Erreur de binding");
        perror("[-]Erreur de binding");
        exit(1);
    }

    printf("[+]Binding succesfull\n");
    printf("[+]Binding succesfull\n");

    /*Attendant la connection au niveau client */

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("[-]Erreur de listening");
        perror("[-]Erreur de listening");
        exit(1);
    }
    printf("[+]Entrain d'entendre les connections\n");
    printf("[+]Entrain d'entendre les connections\n");

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);

        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("[-]Erreur de connection");
            perror("[-]Erreur de connection");
            continue;
        }

        printf("[+]Connection Reussite!!\n");
        printf("[+]Connection Reussite!!\n");

        if (recv(new_fd, &mode, sizeof(char), 0) <= 0) {
            perror("[-]Erreur de lecture du mode");
            close(new_fd);
            continue;
        }

        switch(mode){
            case 'u':
                printf("[+] mode reception selectionné");
                openFile(file,"fichier_recue","rb");
                printf("[+]Réception du fichier en cours...\n");
                receive_file(file, new_fd);
                break;
            case 's':
                printf("[+]Mode envoi sélectionné\n");
                openFile(file,"FILE","rb");
                send_file(file,new_fd);
                break;
            default:
                printf("Entre un mode convennable\n");
                break;
        }
        

    close(sockfd);

    return 0;
    }
}