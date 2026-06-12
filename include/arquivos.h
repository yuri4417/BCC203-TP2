#ifndef ARQUIVOS_H
#define ARQUIVOS_H
#include <stdio.h>
#include <struct.h>
void converteArquivo(int tam, int printFlag);
FILE **criaFitas();
void geraArqMetodo(FILE **pArq, int situacao, int printFlag, int tam) ;
void printFile(FILE *pArq) ;
void printFileBin(FILE *arq);
int cmpDecresc(const void *a,const void *b);
int cmpCresc(const void *a, const void *b);
int parseRegBin(FILE *arq, Registro *r) ;
int parseReg(FILE* pArq, Registro *item) ;
FILE* criaArquivos(int situacao, int printFlag, int tam) ;
#endif