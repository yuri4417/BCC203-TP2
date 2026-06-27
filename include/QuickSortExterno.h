#ifndef QUICKSORTEXTERNO_H
#define QUICKSORTEXTERNO_H
#include "struct.h"
#include <stdio.h>

void FAVazia(TipoArea *Area); // Inicializa a área como vazia
int ObterNumCelOcupadas(TipoArea *Area); // Retorna a quantidade de células ocupadas na área

void InserirArea(TipoArea *Area, Registro *UltLido, int *NRArea,Bench *bench); //Funcao de insercao de registro no vetor de Area
void InsereItem(Registro R, TipoArea *Area,Bench *bench); // Insere o registro na área mantendo a ordenação crescente 
void RetiraPrimeiro(TipoArea *Area, Registro *R); //Remove e retorna o primeiro elemento (o menor valor, devido à ordenação) 
void RetiraUltimo(TipoArea *Area, Registro *R); // Remove e retorna o último elemento (o maior valor, devido à ordenação)
void RetiraMax(TipoArea *Area, Registro *R, int *NRArea); //Funcao que retira o ultimo registro da area
void RetiraMin(TipoArea *Area, Registro *R, int *NRArea); //Funcao que retira o primeiro registro da area
void LeSup(FILE **ArqLEs, Registro *UltLido, int *Ls, bool *OndeLer,Bench *bench); //Funcao que simula o ponteiro de leitura Direito do QuickSort
void Lelnf(FILE **ArqLi, Registro *UltLido, int *Li, bool *OndeLer,Bench *bench) ; //Funcao que simula o ponteiro de leitura Esquerdo do QuickSort
void EscreveMax(FILE **ArqLEs, Registro R, int *Es,Bench *bench); //Funcao de Escrita na parte direita do Quicksort
void EscreveMin(FILE **ArqEi, Registro R, int *Ei,Bench *bench); //Funcao de Escrita na parte Esquerda do Quicksort

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j,Bench *bench); //Funcao Recursiva reponsavel pela realizacao da criacoes das particoes durante o quicksort

void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir,Bench *bench);//Funcao principal de QuickSort Externo

#endif