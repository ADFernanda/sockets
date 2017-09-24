#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int VerificaEntradas(char *tamBuffer)
{
    char *p;
    int num;
    errno = 0;
    long conv1 = strtol(tamBuffer, &p, 10);
    long conv2 = strtol(tamBuffer, &p, 10);

    if (errno != 0 || *p != '\0' || conv1 > INT_MAX || conv2 > INT_MAX) 
    {
        printf("Erro. Entre com um número inteiro para o tamanho do buffer.\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
