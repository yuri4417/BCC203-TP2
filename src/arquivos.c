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

//Funcao que cria o arquivo respectivo ao metodo pedido pelo usuario
void geraArqMetodo(FILE **pArq, char *filePath, int ord, int printFlag, int tam) {
    FILE *pArqRef;
    
    *pArq = fopen(ARQMETODO, "w+b");
    if (!(*pArq)) 
        return;

    //Buffer para a leitura dos registros
    Registro *vec = calloc(tam, sizeof(Registro));
    if (!vec) {
        fclose(*pArq);
        return;
    }
    pArqRef = fopen(filePath, "rb"); 
    if (!pArqRef) {
        FILE *pArqTxt = fopen(randTXT, "r");
        if (!pArqTxt) {
            free(vec);
            fclose(*pArq);
            return;
        }
        FILE *newFile = fopen(filePath, "w+b");
        if (!newFile) {
            fclose(pArqTxt);
            free(vec);
            fclose(*pArq);
            return;
        }
        Registro *buffer = calloc(MAXTAM, sizeof(Registro));
        if (!buffer) {
            fclose(newFile);
            fclose(pArqTxt);
            free(vec);
            fclose(*pArq);
            return;
        }

        int i = 0;
        while (i < MAXTAM && parseReg(pArqTxt, &buffer[i]))
            i++;
            
        if (ord == ARQCRESC)
            qsort(buffer, i, sizeof(Registro), cmpCresc);
        else if (ord == ARQDESC)
            qsort(buffer, i, sizeof(Registro), cmpDecresc);
            
        fwrite(buffer, sizeof(Registro), i, newFile);
        
        free(buffer);
        fclose(pArqTxt);
        rewind(newFile); 
        pArqRef = newFile;
    }

    //Cria o arqMetodo
    int i = 0;
    while (i < tam && fread(&vec[i], sizeof(Registro), 1, pArqRef) == 1) 
        i++;
    if (ord == ARQCRESC)
        qsort(vec, i, sizeof(Registro), cmpCresc);
    else if (ord == ARQDESC)
        qsort(vec, i, sizeof(Registro), cmpDecresc);
        
    fwrite(vec, sizeof(Registro), i, *pArq);
    
    if (printFlag) {
        printf("Registros presentes no arquivo-base para o método:\n");
        for (int j = 0; j < i; j++) 
            printf("Matricula: %08ld | Nota: %6.2f | Curso: %-30s | Cidade: %-50s | Estado: %-2s\n", vec[j].inscricao,
                vec[j].nota, vec[j].curso, vec[j].cidade, vec[j].estado);
    }
    
    free(vec);
    fclose(pArqRef);
    fflush(*pArq);
    rewind(*pArq); 
}

//Função responsável por criar um arquivo
FILE* criaArquivos(int situacao, int printFlag, int tam) {
    FILE *pArq = NULL;
    switch (situacao) {
        case ARQCRESC:
            geraArqMetodo(&pArq, crescFULL, 1, printFlag, tam);
            break;
        case ARQDESC:
            geraArqMetodo(&pArq, descFULL, 2, printFlag, tam);
            break;
        case ARQRAND:
            geraArqMetodo(&pArq, randFULL, 3, printFlag, tam);
            break;
    }
    return pArq;
}


//Função responsável pela criação das fitas
Fitas *criaFitas(){
    Fitas *x = calloc(1, sizeof(Fitas));
    char filePath[50];
    struct stat st = {0};
    if (stat("./data/FEntrada", &st) == -1)
        MKDIR("./data/FEntrada");

    if (stat("./data/FSaida", &st) == -1)
        MKDIR("./data/FSaida");
    
    for(int i =0; i<QTDFITAS; i++){
        sprintf(filePath, "./data/FEntrada/Fita_%d.bin", i);
        x->vArq[i] = fopen(filePath, "w+b");
        x->qtdBlocos[i] = 0;
        if (!(x->vArq[i])) {
            for(int j = 0; j<i; j++)
                if (x->vArq[i] != NULL) 
                    fclose(x->vArq[i]);
            printf("Erro ao criar os arquivos de fita de entrada na pos: %d.\n", i);
            free(x);
            return NULL;
        }
        
    }
    //Criação das Fitas de Saida
    for(int i = QTDFITAS; i< 2*QTDFITAS; i++){
        sprintf(filePath, "./data/FSaida/Fita_%d.bin", i - QTDFITAS);
        x->vArq[i] = fopen(filePath, "w+b");
        x->qtdBlocos[i] = 0;
        if(!(x->vArq[i])) {
            for(int j = 0; j < i; j++)
                fclose(x->vArq[j]);
            free(x);
            printf("Erro ao criar os arquivos de fita de saída na pos: %d.\n", i);
            return NULL;        
        }      
    }
    return x;
}

//Função responsável pela desalocação das fitas
void liberaFitas(Fitas *fitas){
    if(fitas == NULL)
        return;

    for(int i = 0; i < 2 * QTDFITAS; i++){
        if(fitas->vArq[i] != NULL){
            fclose(fitas->vArq[i]);
            fitas->vArq[i] = NULL;
        }
    }

    free(fitas);
}


//Funcao responsavel por converter um arquivo binario para txt
void converteArquivo(int tam, int printFlag) {
    FILE *pArqRef = fopen(ARQMETODO, "rb");
    if (!pArqRef)
        return;
    
    FILE *pDest = fopen(ARQRES, "w");
    if (!pDest){
        fclose(pArqRef);
        return;
    }
    Registro *vec = calloc(tam, sizeof(Registro));
    if (!vec){
        fclose(pArqRef);
        fclose(pDest);
        return;
    }
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
    fclose(pArqRef);
    fclose(pDest);
}