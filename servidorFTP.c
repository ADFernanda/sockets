#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 2000
#define LEN 4096

struct sockaddr_in local;
struct sockaddr_in remoto;

int main (int argc, char *argv[]){

    int sockfd;
    int cliente;
    int len = sizeof(remoto);
    int slen;
    char buffer[4096];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        perror("socket ");
    }
    else
    printf("Socket criado com sucesso\n");

    local.sin_family = AF_INET;
    local.sin_port = htons(PORTA);

    memset(local.sin_zero, 0x0, 8);

    if(bind(sockfd, (struct sockaddr*)&local, sizeof(local)) == -1){
        perror("bind ");
        exit(1);        
    }

    listen(sockfd, 1);

    if((cliente = accept (sockfd, (struct sockaddr*)&remoto, &len)) == -1){
        perror("bind ");
        exit(1);
    }

    strcpy(buffer, "Welcome!\n\0");

    if(send(cliente, buffer, strlen(buffer), 0)){
        printf("Aguardando resposta do cliente...\n");
        while(1){
            memset(buffer, 0x0, LEN);
            if((slen = recv(cliente, buffer, LEN, 0)) > 0){
                buffer[slen] = '\0';
                printf("Mensagem recebida: %s\n", buffer);
                close(cliente);
                break;
            }
        }
    }
    printf("Servidor encerrado!\n");
    close(sockfd);
    return 0;
}