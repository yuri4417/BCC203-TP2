#ifndef STRUCT_H
#define STRUCT_H
#define MAXTAM 471705
#define QTDFITAS 20 
#define BLOCK_SIZE 10240
#define ARQCRESC 1
#define ARQDESC 2 
#define ARQRAND 3 
#define IB2F 1
#define IB2FSUB 2 
#define QS 3      
#define TAMAREA 20
#define MAX_BLOCOS 50000 
#include <stdbool.h>
#include <stdio.h>

#define ARQMETODO "./data/arqMetodo.bin"
#define ARQRES    "./data/arqResult.txt"
#define randTXT   "./data/PROVAO.TXT"
#define randFULL  "./data/PROVAO_R.bin"
#define crescFULL "./data/PROVAO_C.bin"
#define descFULL  "./data/PROVAO_D.bin"


//Definicao de Apelido para a utilizacao do Timer
typedef struct timespec Timer;

//Struct do Registro do Aluno
typedef struct {
    long int inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Registro;

#define EOB (Registro){0, -1.0, "", "", ""}

//Struct das Fitas Magneticas
typedef struct{
    FILE* vArq[2 * QTDFITAS];
    int qtdBlocos[2 * QTDFITAS];
}Fitas;

//Struct da Configuracao que o usuario define para execucao do programa
typedef struct {
    int metodo;
    int quantidade;
    int situacao;
    int imprimir;
} Config;

//Struct do Bench para avaliacao de eficiencia do programa
typedef struct {
    long comp;
    long transfLeit;
    long transfEsc;
    double tempoExec;
}Bench;

//Struct da Area (Memoria Principal) do QuickSort
typedef struct {
    Registro itens[TAMAREA];
    int ocupadas;
} TipoArea;

//Struct do Heap utilizado na geracao dos blocos
typedef struct{
    Registro reg;
    int fitaOrigem;
    bool marcado;
} Heap;



#endif