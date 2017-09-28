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
    char *buffer = (char*) calloc (tamBuffer ,sizeof(char)), nomeArquivo[256];

    FILE *arquivo;

    //cria um socket e armazena em servidorfd um file descriptor para tal socket
    servidorfd = socket(AF_INET, SOCK_STREAM, 0);
    if((servidorfd) == -1){
        perror("socket");
        free(buffer);
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
        free(buffer);
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
        free(buffer);
        return 1;
    }
    
    memset(buffer, 0x0, tamBuffer);
    
    //recebe nome do arquivo
    if((slen = recv(clientefd, nomeArquivo, tamBuffer, 0)) > 0){
        nomeArquivo[slen] = '\0';
        arquivo = fopen(nomeArquivo, "r");
    }else{
        fclose(arquivo);
        free(buffer);
        close(servidorfd);
        close(clientefd);
    }
    //loop para transferência de dados entre servidor e cliente
    while (!feof(arquivo)) {
        memset(buffer, 0x0, tamBuffer);
        fread(buffer, tamBuffer, 1, arquivo);
        send(clientefd, buffer, tamBuffer, 0);
    }   

    fclose(arquivo);
    free(buffer);
    close(servidorfd);
    close(clientefd);
    return 0;
}