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


#define ARQMETODO "./data/arqMetodo.bin"
#define ARQRES    "./data/arqResult.txt"
#define randFULL  "./data/PROVAO.TXT"



typedef struct timespec Timer;
typedef struct {
    long int inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Registro;

typedef struct {
    int metodo;
    int quantidade;
    int situacao;
    int imprimir;
} Config;

typedef struct {
    long comp;
    long transfLeit;
    long transfEsc;
    double tempoExec;
}Bench;

typedef struct {
    Registro itens[TAMAREA];
    int ocupadas;
} TipoArea;

#endif