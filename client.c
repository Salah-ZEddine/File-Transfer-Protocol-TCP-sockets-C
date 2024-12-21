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

    file = fopen("received_file", "wb");
    if (file == NULL) {
        perror("[-]Erreur d'ouverture du fichier");
        exit(1);
    }

    while ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) > 0) {
        fwrite(buf, sizeof(char), numbytes, file);
    }

    if (numbytes == -1) {
        perror("[-]Erreur de réception");
    }

    printf("[+]Transfert de fichier terminé\n");

    
    fclose(file);
    close(sockfd);
    return 0;
}