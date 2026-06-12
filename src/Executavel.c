#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include "Executavel.h"
#include "IntBalanceada.h"
#include "IntBalanceadaSub.h"
#include "QuickSortExterno.h"
#include "arquivos.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Funcoes relacionadas a utilizacao de timer para calculo de eficiencia do programa
void timerStart(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, t);
}
double timerStop(Timer *pIni) {
    struct timespec fim;
    clock_gettime(CLOCK_MONOTONIC, &fim);
    return (fim.tv_sec - pIni->tv_sec) + (fim.tv_nsec - pIni->tv_nsec) / 1e9;
}


//Funcao responsavel pela validacao das variaveis passadas na execucao do ./ordena
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


//Funcao responsavel por inicializar o time, criacao de arquivos e execucao do metodo pedido
void executar(Config *cfg, Bench *bench) {
    //Declara variaveis que sao usadas nos metodos
    Timer timer;  
    bench->comp = 0;
    bench->transfLeit = 0;
    bench->transfEsc = 0;

    FILE *pArq =  criaArquivos(cfg->situacao, cfg->imprimir, cfg->quantidade);
    if (!pArq) {
        printf("Erro ao abrir o pArquivo.\n");
        return;
    } 
    timerStart(&timer); //Inicia o contador do tempo
    //Chama qual metodo usar
    switch (cfg->metodo) {

        //Intercalacao Balanceada
        case IB2F:
            printf("Faz 2f com algum metodo\n");
            break;

        //Intercalacao Balanceada com Substituicao por Selecao    
        case IB2FSUB:
            printf("Faz 2f com heap\n");
            break;

        //QuickSort Externo    
        case QS:
            QuicksortExterno(&pArq, &pArq, &pArq, 1, cfg->quantidade);
            break;
    }
    converteArquivo(cfg->quantidade, cfg->imprimir);
    //Finaliza a contagem do tempo e printa as transf, comp e o tempo de execucao
    bench->tempoExec = timerStop(&timer);
    /*printf("Numero de transferencias Leitura: %ld\n", bench->transfLeit);
    printf("Numero de transferencias Escrita: %ld\n", bench->transfEsc);
    printf("Numero de comparacoes: %ld\n", bench->comp);
    printf("Tempo de execucao: %lf segundos\n", bench->tempoExec);
    */
}