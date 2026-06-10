
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "arquivos.h"
#include "struct.h"


#define crescFULL "./data/arqCresc-FULL.bin"
#define descFULL "./data/arqDesc-FULL.bin"
#define randFULL "./data/PROVAO.TXT"


void parseReg(FILE* pArq, Registro *item) {
    char linha[104];
    char temp[50];

    fgets(linha, sizeof(Registro), pArq);
    printf("%s", linha);
    memcpy(temp, linha, 8);
    temp[8] = '\0';
    item->inscricao = atol(temp);


    memcpy(temp, linha + 9, 5);
    temp[5] = '\0';
    printf("%s\n", temp);
    item->nota = atof(temp);
    

    memcpy(item->estado, linha + 15, 2);
    item->estado[2] = '\0';

    memcpy(item->cidade, linha + 18, 50);
    item->cidade[50] = '\0';
    for (int i = 49; item->cidade[i] >= 0; i--) {
        if (item->cidade[i] != 32) {
            item->cidade[i+1] = '\0';
            break;
        }

    }
    memcpy(item->curso, linha + 69, 30);
    item->curso[30] = '\0';
    for (int i = 29; item->curso[i] >=0; i--) {
        if (item->curso[i] != 32) {
            item->curso[i+1] = '\0';
            break;
        }
    }   
}


int cmpCresc(const void *a, const void *b) {
    const Registro *r1 = a;
    const Registro *r2 = b;

    if (r1->nota < r2->nota)
        return -1;
    if (r1->nota > r2->nota)
        return 1;
    return 0;
}

int cmpDecresc(const void *a, const void *b) {
    const Registro *r1 = a;
    const Registro *r2 = b;

    if (r1->nota > r2->nota)
        return -1;
    if (r1->nota < r2->nota)
        return 1;
    return 0;
}

void geraOrdenado(FILE** pArq, char* filePath, int printFlag, int tam, int Cresc) {
    FILE *pArq = fopen(randFULL, "r");
    if (!pArq) return;
    FILE *pDest = fopen(filePath, "w");
    if (!pDest) return;
    Registro *vec = malloc(sizeof(Registro) * tam);
    if (!vec) return;

    int i = 0;
    while (i < tam) parseReg(pArq, &vec[i++]);

    // Verificar se pode usar qsort direto
    if (Cresc)
        qsort(vec, tam, sizeof(Registro), cmpCresc);
    else
        qsort(vec, tam, sizeof(Registro), cmpDecresc);

    i = 0;
    while (i < tam) {
        fprintf(pDest, "%8ld %5.1f %-2s %-50s %-30s\n", vec[tam].inscricao, vec[tam].nota, vec[tam].estado, vec[tam].cidade, vec[tam].curso);
        i++;
    }
}


FILE* criaArquivos(int situacao, int printFlag, int tam) {
    FILE *pArq = NULL;
    //Decide qual arquivo gerar
    switch (situacao) {
        case ARQCRESC:
            geraOrdenado(&pArq, crescFULL, printFlag, tam, 1);
            break;
        case ARQDESC:
            geraOrdenado(&pArq, descFULL, printFlag, tam, 0);
            break;
        case ARQRAND:
            fopen(crescFULL, "r");
            break;
    }
    return pArq;
}


FILE **criaFitas(){
    FILE** matrizFile = malloc(sizeof(FILE*)*QTDFITAS);
    char filePath[50];
    
    for(int i =0; i<QTDFITAS; i++){
        
        sprintf(filePath, "./data/FEntrada/Fita_%d.txt", i);
        matrizFile[i] = fopen(filePath, "w");
    }
    return matrizFile;
}
