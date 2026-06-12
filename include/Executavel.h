#ifndef EXECUTAVEL_H
#define EXECUTAVEL_H
#include "struct.h"
void timerStart(Timer *t) ;
double timerStop(Timer *pIni);
int valida(int argc, char *argv[], Config *cfg);
void executar(Config *cfg, Bench *bench) ;
#endif