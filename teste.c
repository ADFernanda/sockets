#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define NUM_PARAMS 3

struct sockaddr_in servidor;
struct sockaddr_in cliente;

int main(int argc, char *argv[])
{
    //verifica quantidade de entradas
    if (argc != NUM_PARAMS)
    {
        printf("Erro. Verrifique os arqumentos passados para servidorFTP.\n");
    }

    //verifica se as entradas são válidas
    // if (VerificaEntradas(argv[1], argv[2]) == 1)
    // {
    //     return 1;
    // }

    int tamBuffer = atoi(argv[2]);
    char *portoServidor = argv[1];

    printf("Entradas: %s %d\n", portoServidor, tamBuffer);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //descritor socket
    int sockCliente = socket(AF_INET, SOCK_STREAM, 0); //descritor socket
    char buffer[4096];
    int slen;

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("socket criado com sucesso\n");
    }

    //preenchendo a estrutura de dados do socket
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(2000);
    memset(servidor.sin_zero, 0x0, 8);

    if(bind(sockfd, (struct sockaddr*)&servidor, sizeof(servidor)) == -1){
        perror("bind ");
        exit(1);
    }

    listen(sockfd, 1);
    int len = sizeof(servidor);
    if(sockCliente = accept(sockfd, (struct sockaddr*)&servidor, &len) == -1){
        perror("accept ");
        exit(1);
    }

    strcpy(buffer, "Welcome! \n\0");

    if(send(sockCliente, buffer, strlen(buffer), 0)){
        printf("aguardando a resposta dp cliente...\n");
        while(1){
            memset(buffer, 0x0, 4096);
            if(slen = recv(sockCliente, buffer, 4096, 0) > 0)
            {
                buffer[slen] = '\0';
                printf("mensagem recebida %s\n", buffer);
                close(sockCliente);
                break;
            }        
        }
    }
    printf("servidor finalizado\n");
    close(sockfd);

    return 0;
}