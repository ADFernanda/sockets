#include "lib.h"

void EnviaArquivo(){}

int main (int argc, char *argv[]){

    //verifica quantidade de entradas
    if (argc != NUM_PARAMS_SERVIDOR)
    {
        printf("Erro. Verrifique os arqumentos passados para servidorFTP.\n");
        return 1;
    }
    //verifica se as entradas numéricas são válidas
    if (VerificaEntradas(argv[1], argv[2]) == 1)
    {
        return 1;
    }

    struct sockaddr_in servidor, cliente;
    int tamBuffer = atoi(argv[2]), portoServidor = atoi(argv[1]), servidorfd, clientefd,
        sizeCliente = sizeof(cliente), slen;
    char *buffer = (char*) calloc (tamBuffer ,sizeof(char));

    struct timeval tvInicial, tvFinal;
    int tempo = gettimeofday(&tvInicial, NULL);
    //cria um socket e armazena em servidorfd um file descriptor para tal socket
    servidorfd = socket(AF_INET, SOCK_STREAM, 0);
    if((servidorfd) == -1){
        perror("socket");
        return 1;
    }

    // preenche estrutura de dados do servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(portoServidor);
    memset(servidor.sin_zero, 0x0, 8);

    //atribui o endereço especificado pelo addr ao socket referido pelo arquivo descritor servidorfd
    if(bind(servidorfd, (struct sockaddr*)&servidor, sizeof(servidor)) == -1){
        perror("bind");
        close(servidorfd);
        return 1;       
    }

    //torna servidorfd capaz de aceitar uma solicitação de conexão por meio do accept
    listen(servidorfd, 1);

    /* pega a primeira solicitação da fila de conexões pendentes para servidorfd 
    e retorna o socket (clientefd) conectado à servidorfd */
    clientefd = accept (servidorfd, (struct sockaddr*)&cliente, &sizeCliente);
    if(clientefd == -1){
        perror("accept");
        close(servidorfd);
        return 1;
    }

    memset(buffer, 0x0, tamBuffer);
    
    //recebe nome do arquivo
    if((slen = recv(clientefd, buffer, tamBuffer, 0)) > 0){
        buffer[slen] = '\0';
        printf("nome do arquivo: %s\n", buffer);
        close(clientefd);
    }

    //abre o arquivo

    //loop para transferência de dados entre servidor e cliente
    // while(1){
        //envia o arquivo => send(clientefd, buffer, strlen(buffer), 0)  
    // }    

    close(servidorfd);
    tempo = gettimeofday(&tvFinal, NULL);
    printf("tempo gasto: %lu\n", tvFinal.tv_sec - tvInicial.tv_sec);
    return 0;
}