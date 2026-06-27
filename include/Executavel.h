#ifndef EXECUTAVEL_H
#define EXECUTAVEL_H
#include "struct.h"
//Funcoes relacionadas a utilizacao de timer para calculo de eficiencia do programa
void timerStart(Timer *t) ;
double timerStop(Timer *pIni);

//Funcao responsavel pela validacao das variaveis passadas na execucao do ./ordena
int valida(int argc, char *argv[], Config *cfg);

//Funcao responsavel por inicializar o time, criacao de arquivos e execucao do metodo pedido
void executar(Config *cfg, Bench *bench) ;
#endif