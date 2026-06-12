#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "arquivos.h"
#include "struct.h"

//Funcao de leitura no arquivo original .txt que armazena os registros
int parseReg(FILE* pArq, Registro *item) {
    char linha[104];
    char temp[50];

    //Leitura de Matricula
    if (fgets(linha, sizeof(linha), pArq) == NULL)   
        return 0;
    memcpy(temp, linha, 8);
    temp[8] = '\0';
    item->inscricao = atol(temp);

    //Leitura da Nota
    memcpy(temp, linha + 9, 5);
    temp[5] = '\0';
    item->nota = atof(temp);
    
    //Leitura do Estado
    memcpy(item->estado, linha + 15, 2);
    item->estado[2] = '\0';
    
    //Leitura da Cidade e Reducao dos ' ' desnecessarios
    memcpy(item->cidade, linha + 18, 50);
    item->cidade[50] = '\0';
    for (int i = 49; i >= 0; i--) {
        if (item->cidade[i] != 32) {
            item->cidade[i+1] = '\0';
            break;
        }
    }

    //Leitura do Curso e Reducao dos ' ' desnecessarios
    memcpy(item->curso, linha + 69, 30);
    item->curso[30] = '\0';
    for (int i = 29; i >= 0; i--) {
        if (item->curso[i] != 32) {
            item->curso[i+1] = '\0';
            break;
        }
    }   
    return 1;
}

//Funcao de leitura do registro binario
int parseRegBin(FILE *arq, Registro *r) {
    return fread(r, sizeof(Registro), 1, arq) == 1;
}

//Funcoes comparativas entre itens
int cmpCresc(const void *a, const void *b) {
    const Registro *r1 = a;
    const Registro *r2 = b;

    if (r1->nota < r2->nota)
        return -1;
    if (r1->nota > r2->nota)
        return 1;
    return 0;
}
int cmpDecresc(const void *a,const void *b) {
    const Registro *r1 = a;
    const Registro *r2 = b;

    if (r1->nota > r2->nota)
        return -1;
    if (r1->nota < r2->nota)
        return 1;
    return 0;
}

//Funcao que cria o arquivo respectivo ao metodo de ordenacao pedido pelo usuario
void geraArqMetodo(FILE **pArq, int ord, int printFlag, int tam) {
    *pArq = fopen(ARQMETODO, "w+b");
    if (!(*pArq)) 
        return;

    FILE *pArqRef = fopen(randFULL, "r");
    if (!pArqRef) {
        fclose(*pArq);
        return;
    }

    Registro *vec = malloc(sizeof(Registro) * tam);
    if (!vec) {
        fclose(*pArq);
        fclose(pArqRef);
        return;
    }
    int i = 0;
    while (i < tam && parseReg(pArqRef, &vec[i]))
        i++;

    if (ord == ARQCRESC)
        qsort(vec, i, sizeof(Registro), cmpCresc);
    else if (ord == ARQDESC)
        qsort(vec, i, sizeof(Registro), cmpDecresc);

    fwrite(vec, sizeof(Registro), i, *pArq);

    if (printFlag) {
        printf("Registros presentes no arquivo-base para o método:\n");
        for (int j = 0; j < i; j++) {
            printf("Matricula: %08ld | Nota: %6.2f | Curso: %-30s | Cidade: %-50s | Estado: %-2s\n", vec[j].inscricao,
                 vec[j].nota, vec[j].curso, vec[j].cidade, vec[j].estado);
        }
    }

    free(vec);
    fclose(pArqRef);

    fflush(*pArq);
    fseek(*pArq, 0, SEEK_SET);
}


//Função responsável por criar um arquivo
FILE* criaArquivos(int situacao, int printFlag, int tam) {
    FILE *pArq = NULL;
    geraArqMetodo(&pArq, situacao, printFlag, tam);
    return pArq;
}


//Função responsável pela criação das fitas magnéticas
FILE **criaFitas(){
    FILE** matrizFile = malloc(2 * sizeof(FILE*)*QTDFITAS);
    if (!matrizFile) 
        return NULL;
    char filePath[50];
    
    //Criação das Fitas de Entrada
    for(int i =0; i<QTDFITAS; i++){
        sprintf(filePath, "./data/FEntrada/Fita_%d.txt", i);
        matrizFile[i] = fopen(filePath, "wb");
        if(!matrizFile[i])
        {
            for(int j = 0; j<i; j++)
                fclose(matrizFile[j]);
            free(matrizFile);
            printf("Erro ao criar os arquivos de fita de entrada na pos: %d.\n", i);
            return NULL;        
        }                
    }
   
    //Criação das Fitas de Saida
    for(int i = QTDFITAS; i< 2*QTDFITAS; i++){
        sprintf(filePath, "./data/FSaida/Fita_%d.txt", i - QTDFITAS);
        matrizFile[i] = fopen(filePath, "wb");
        if(!matrizFile[i]) {
            for(int j = 0; j < i; j++)
                fclose(matrizFile[j]);
            free(matrizFile);
            printf("Erro ao criar os arquivos de fita de saída na pos: %d.\n", i);
            return NULL;        
        }      
    }
    return matrizFile;
}


//Funcao responsavel por converter um arquivo binario para txt
void converteArquivo(int tam, int printFlag) {
    FILE *pArqRef = fopen(ARQMETODO, "rb");
    if (!pArqRef)
        return;
    
    FILE *pDest = fopen(ARQRES, "w");
    if (!pDest)
        return;
    Registro *vec = malloc(sizeof(Registro) * tam);
    if (!vec)
        return;
    fread(vec, sizeof(Registro), tam, pArqRef);
    if (printFlag) 
        printf("Arquivo após o método:\n");

    for (int i = 0; i < tam; i++) {
        if (printFlag)
            printf("Matricula: %08ld | Nota: %6.2f | Curso: %-30s | Cidade: %-50s | Estado: %-2s\n", vec[i].inscricao,
                 vec[i].nota, vec[i].curso, vec[i].cidade, vec[i].estado);
        
        fprintf(pDest, "%08ld %05.1f %-2s %-50s %30s\n", vec[i].inscricao, vec[i].nota, vec[i].estado, vec[i].cidade, vec[i].curso);
    }
    
    free(vec);
}