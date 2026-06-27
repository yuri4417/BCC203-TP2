#ifndef INTBALANCEADA_H
#define INTBALANCEADA_H
#include <stdio.h>

#include "struct.h"
//Funcao responsavel pela geracao do blocos que serao intercalados
void geraBlocos(FILE* arqBin, int tam, Fitas *fitas,Bench *bench);
void geraBlocosSub(FILE* arqBin, int tam, Fitas *fitas, Bench *bench) ;

//Funcao responsavel pela intercalacao dos blocos
void intercalarBlocos(FILE* arqBin, Fitas* fitas,Bench *bench);

//Funcao principal de execucao do Intercalacao Balanceada com Merge Sort / Intercalacao Balanceada com substituicao por selecao
void intBalanceada(FILE* arqBin, int tam,Bench *bench);
void intBalanceadaSub(FILE* arqBin, int tam, Bench *bench) ;

//Funcoes complementares e a Funcao principal do Merge Sort
void merge(Registro *v, Registro *aux, int l, int m, int r,Bench *bench);
void mergerec(Registro *v, Registro *aux, int l, int r,Bench *bench);
void mergeSort(Registro *v, int n,Bench *bench);

//Funcoes para operacoes do Heap durante geracao dos blocos
void trocaHeap(Heap *a, Heap *b);
void refazHeap(Heap *h,int n, int i,Bench *bench);
void constroiHeap(Heap h[], int n,Bench *bench);

#endif