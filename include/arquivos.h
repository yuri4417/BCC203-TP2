#ifndef ARQUIVOS_H
#define ARQUIVOS_H
#include <stdio.h>
#include <struct.h>


FILE* criaArquivos(int situacao, int printFlag, int tam); //Funcao responsavel por criar arquivo respectivo ao tipo de ordenacao
void converteArquivo(int tam, int printFlag); //Funcao responsavel por converter um arquivo binario para txt
void geraArqMetodo(FILE **pArq, char *filePath, int ord, int printFlag, int tam); //Funcao que cria um arquivo respectivo ao metodo de ordenacao

//Funcoes de comparacao entre itens
int cmpDecresc(const void *a,const void *b);
int cmpCresc(const void *a, const void *b);

//Funcoes de leitura de arquivos
int parseRegBin(FILE *arq, Registro *r) ;
int parseReg(FILE* pArq, Registro *item) ;

Fitas *criaFitas(); //Funcao responsavel por alocar a estrutura das Fitas
void liberaFitas(Fitas *fitas); // Funcao responsavel pela desalocacao das fitas
#endif