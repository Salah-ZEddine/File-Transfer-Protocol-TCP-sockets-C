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
#define MAXDATASIZE 1024


int main(){

    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct sockaddr_in their_addr;
    FILE *file;
    char mode;
    char fileName[256];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[-]Erreur de creation de socket");
        exit(1);
    }
    printf("[+]Socket cree avec succes \n");

    their_addr.sin_family = AF_INET;   /* host byte order */
    their_addr.sin_port = htons(MYPORT); /* short, network byte order */
    bzero(&(their_addr.sin_zero), 8);

    if (inet_aton(IPADD, &their_addr.sin_addr) == 0) {  
        fprintf(stderr, "Invalid IP address\n");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1){
        perror("[-]erreur de connection");
        exit(1);
    }

    printf("[+]Demande de connection a ete cree\n");

    printf("Entrer le mode (r pour recevoir, u pour envoyer)\n");
    scanf("%c",&mode);

    switch (mode){
    case 'u':
        openFile(file,"received_file","wb");
        receive_file(file,sockfd);
        break;
    
    case 'r':
        printf("[+]Entre le nom de fichier a envoyé\n");
        scanf("%s", fileName);
        openFile(file,fileName,"rb");
        send_file(file,sockfd);
        break;
    
    default:
        printf("Entre un mode convennable\n");
        break;
    }
    close(sockfd);
    return 0;
}

void openFile(FILE *file,char fileName[256], char permission[25]){
    file = fopen(fileName, permission);
        if (file == NULL) {
            perror("[-]Erreur d'ouverture du fichier");
            exit(1);
        }
        printf("[+]Fichier ouvert sans prob\n");
}

void send_file(FILE *file, int new_fd){

    char buffer[MAXDATASIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(new_fd, buffer, bytes_read, 0) == -1)
        {
            perror("[-]Envoie de fichier non réussie");
            break;
        }
    }

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
    printf("[+]Reception complete du fichier\n");
}