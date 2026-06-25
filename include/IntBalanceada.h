#ifndef INTBALANCEADA_H
#define INTBALANCEADA_H
#include <stdio.h>

#include "struct.h"

void geraBlocos(FILE* arqBin, int tam, Fitas *fitas);
void intercalarBlocos(FILE* arqBin, Fitas* fitas);
void intBalanceada(FILE* arqBin, int tam);

void merge(Registro *v, Registro *aux, int l, int m, int r);
void mergerec(Registro *v, Registro *aux, int l, int r);
void mergeSort(Registro *v, int n);


void trocaHeap(Heap *a, Heap *b);
void refazHeap(Heap *h,int n, int i);
void constroiHeap(Heap h[], int n);

#endif