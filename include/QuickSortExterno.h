#ifndef QUICKSORTEXTERNO_H
#define QUICKSORTEXTERNO_H
#include "struct.h"
#include <stdio.h>

void FAVazia(TipoArea *Area);
int ObterNumCelOcupadas(TipoArea *Area);
void InsereItem(Registro R, TipoArea *Area);
void RetiraPrimeiro(TipoArea *Area, Registro *R);
void RetiraUltimo(TipoArea *Area, Registro *R);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j);
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir);

#endif