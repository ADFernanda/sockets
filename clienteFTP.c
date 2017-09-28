#include "lib.h"
#include <arpa/inet.h>


// void RecebeArquivo(){}

int main (int argc, char *argv[]){

    //verifica quantidade de entradas
    if (argc != NUM_PARAMS_CLIENTE)
    {
        printf("Erro. Verrifique os arqumentos passados para servidorFTP.\n");
        return 1;
    }
    //verifica se as entradas numéricas são válidas
    if (VerificaEntradas(argv[2], argv[4]) == 1)
    {
        return 1;
    }

    struct sockaddr_in remoto;
    int tamBuffer = atoi(argv[4]), portoServidor = atoi(argv[2]), clientefd, len = sizeof(remoto), slen, i=0;
    char *buffer = (char*) calloc (tamBuffer ,sizeof(char)), *hostServidor = argv[1], *nomeArquivo = argv[3];
    FILE *arquivoRecebido = fopen("novo.txt", "w+");

    double taxa = 0;
    unsigned int tempoGastoMs = 0, tempoGastoSeg = 0, numBytes = 0;
    double numKbytes = 0;

    struct timeval  tvInicial, tvFinal;
    gettimeofday(&tvInicial, NULL);
    

    //cria um socket e armazena em clientefd um file descriptor para tal socket
    clientefd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientefd == -1){
        perror("socket");
    }

    //preenche estrutura de dados do cliente
    remoto.sin_family = AF_INET;
    remoto.sin_port = htons(portoServidor);
    remoto.sin_addr.s_addr = inet_addr(hostServidor);    
    memset(remoto.sin_zero, 0x0, 8);
    
    //conecta clientefd ao socket referenciado em addr
    if((connect (clientefd, (struct sockaddr*)&remoto, len)) == -1){
        perror("connect");
        close(clientefd);
        exit(1);
    }
    
    if((send(clientefd, nomeArquivo, strlen(nomeArquivo), 0)) == -1){
        printf("Erro no envio do nome do arquivo\n");
        perror("send");
        close(clientefd);
        exit(1);
    }
    
    //loop para transferência de dados entre servidor e cliente
    while(1){
        memset(buffer, 0x0, tamBuffer);
        if(slen = recv(clientefd, buffer, tamBuffer, 0) > 0){
            fputs (buffer, arquivoRecebido);
            i++;
        }else{            
            break;
        }           
    }
    
    
    fclose(arquivoRecebido);
    close(clientefd);

    numBytes = i * tamBuffer;
    numKbytes = numBytes/1000;
    

    gettimeofday(&tvFinal, NULL);
    unsigned int time_in_sec = (tvFinal.tv_sec) -  (tvInicial.tv_sec);
    unsigned int time_in_mill = (tvFinal.tv_usec / 1000) - (tvInicial.tv_usec / 1000); // convert tv_sec & tv_usec to millisecond

    taxa = (double)numKbytes/time_in_sec;

    printf("Buffer = \%5u byte(s), \%10.2f kbps (\%u bytes em \%3u.\%06u s)\n", tamBuffer, taxa, numBytes, time_in_sec, time_in_mill);

    return 0;
}