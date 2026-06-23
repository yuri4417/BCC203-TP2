#ifndef INTBALANCEADA_H
#define INTBALANCEADA_H
#include <stdio.h>

#include "struct.h"

void geraBlocos(FILE* arqBin, int tam, Fitas *fitas);
void intercalarBlocos(FILE** arqBin, int tam, Fitas* fitas);
void intBalanceada(FILE* arqBin, int tam);

void recarregaBuffer(Registro* buffer, FILE* arqBin, int* qtdRestante, int* bufferN);

void merge(Registro *v, int l, int m, int r);
void mergerec(Registro *v, int l, int r);
void mergeSort(Registro *v, int n);

void trocaHeap(Heap *a, Heap *b);
void refazHeap(Heap *h,int n, int i);
void constroiHeap(Heap h[], int n);

#endif