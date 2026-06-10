#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#include "Executavel.h"
#include "struct.h"
#include "IntBalanceada.h"
#include "IntBalanceadaSub.h"
#include <time.h>

typedef struct timespec Timer;
void timerStart(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, t);
}
double timerStop(Timer *pIni) {
    struct timespec fim;
    clock_gettime(CLOCK_MONOTONIC, &fim);
    return (fim.tv_sec - pIni->tv_sec) + (fim.tv_nsec - pIni->tv_nsec) / 1e9;
}


int valida(int argc, char *argv[], Config *cfg){
    //Verifica se o arquivo tem o tamanho certo
    if (argc < 4)
        return 0;

    //Valida qual metodo usar
    cfg->metodo = atoi(argv[1]);
    if(cfg->metodo < 1 || cfg->metodo > 3){ 
        printf("Metodo invalido. Use 1 para Intercalação Balanceada com Counting Sort, 2 para Intercalação Balanceada com Seleção do Substuição, 3 para QuickSort Externo.\n");
        return 0;
    }
    
    //Valida a quantidade do arquivo
    cfg->quantidade = atoi(argv[2]);
    if(cfg->quantidade <= 0){
        printf("Digite uma quantidade válida.\n");
        return 0;
    }

    //Valida qual arquivo utilizar
    cfg->situacao = atoi(argv[3]);
    if(cfg->situacao < 1 || cfg->situacao > 3){
        printf("Situacao invalida. Use 1 para arquivo ascendente, 2 para arquivo descendente e 3 para arquivo aleatorio.\n");
        return 0;
    }
    
    //Valida se tem [-P] para imprimir as chaves do arquivo
    if (argc == 5 && strcmp(argv[4], "[-P]") == 0) 
        cfg->imprimir = 1;

    //Verifica se tem o tamanho correto do comando mas algo escrito diferente
    else if(argc >= 5) {
        printf("Digite o comando correto: ordena <método> <quantidade> <situação> [-P]\n");
        return 0;
    }
    return 1;
}

void executar(Config *cfg, Bench *bench) {
    //Declara variaveis que sao usadas nos metodos
    Timer timer;  
    bench->comp = 0;
    bench->transf= 0;

    FILE *pArq =  criaArquivos(cfg->situacao, cfg->imprimir, cfg->quantidade);
    if (!pArq) {
        printf("Erro ao abrir o pArquivo.\n");
        return;
    } 
    timerStart(&timer); //Inicia o contador do tempo
    //Chama qual metodo usar
    switch (cfg->metodo) {
        case IB2F:
            printf("Faz 2f com algum metodo\n");
            break;
        case IB2FSUB:
            printf("Faz 2f com heap\n");
            break;
        case QS:
            printf("Faz quicksort externo\n");
            break;
    }
    //Finaliza a contagem do tempo e printa as transf, comp e o tempo de execucao
    bench->tempoExec = timerStop(&timer);
    printf("Numero de transferencias: %ld\n", bench->transf);
    printf("Numero de comparacoes: %ld\n", bench->comp);
    printf("Tempo de execucao: %lf segundos\n", bench->tempoExec);
}