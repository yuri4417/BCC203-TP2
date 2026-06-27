#include "QuickSortExterno.h"
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

// Inicializa a área como vazia 
void FAVazia(TipoArea *Area) {
    Area->ocupadas = 0;
}

// Retorna a quantidade de células ocupadas na área 
int ObterNumCelOcupadas(TipoArea *Area) {
    return Area->ocupadas;
}

// Insere o registro na área mantendo a ordenação crescente (por nota) 
void InsereItem(Registro R, TipoArea *Area,Bench *bench) {
    if (Area->ocupadas >= TAMAREA) {
        return;
    }

    int i = Area->ocupadas - 1;

    // Desloca os elementos maiores para a direita para abrir espaço
    // Altere 'nota' para o campo correto se o critério de ordenação for outro
    while (i >= 0){
        bench->comp++;
        if (Area->itens[i].nota > R.nota) {
            Area->itens[i + 1] = Area->itens[i];
            i--;
        } else
            break;

    }

    // Insere o novo registro na posição correta
    Area->itens[i + 1] = R;
    Area->ocupadas++;
}

//Remove e retorna o primeiro elemento (o menor valor, devido à ordenação) 
void RetiraPrimeiro(TipoArea *Area, Registro *R) {
    if (Area->ocupadas == 0) {
        return; // Área vazia
    }

    // O menor elemento está sempre no índice 0
    *R = Area->itens[0];

    // Desloca todos os outros elementos para a esquerda
    for (int i = 0; i < Area->ocupadas - 1; i++) {
        Area->itens[i] = Area->itens[i + 1];
    }

    Area->ocupadas--;
}

// Remove e retorna o último elemento (o maior valor, devido à ordenação)
void RetiraUltimo(TipoArea *Area, Registro *R) {
    if (Area->ocupadas == 0) {
        return; // Área vazia
    }

    // O maior elemento está sempre na última posição ocupada
    *R = Area->itens[Area->ocupadas - 1];
    Area->ocupadas--;
}

//Funcao que simula o ponteiro de leitura Direito do QuickSort
void LeSup(FILE **ArqLEs, Registro *UltLido, int *Ls, bool *OndeLer,Bench *bench) {
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Registro), SEEK_SET);
    fread(UltLido, sizeof(Registro), 1, *ArqLEs);
    bench->transfLeit++;
    (*Ls)--; 
    *OndeLer = false;
}

//Funcao que simula o ponteiro de leitura Esquerdo do QuickSort
void Lelnf(FILE **ArqLi, Registro *UltLido, int *Li, bool *OndeLer,Bench *bench) {
    fseek(*ArqLi, (*Li - 1) * sizeof(Registro), SEEK_SET);
    fread(UltLido, sizeof(Registro), 1, *ArqLi);
    bench->transfLeit++; 
    (*Li)++; 
    *OndeLer = true;
}

//Funcao de insercao de registro no vetor de Area
void InserirArea(TipoArea *Area, Registro *UltLido, int *NRArea,Bench *bench) {
    /* Insere UltLido de forma ordenada na Area */
    InsereItem(*UltLido, Area,bench); 
    *NRArea = ObterNumCelOcupadas(Area);
}

//Funcao de Escrita na parte direita do Quicksort
void EscreveMax(FILE **ArqLEs, Registro R, int *Es,Bench *bench) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(Registro), SEEK_SET);
    fwrite(&R, sizeof(Registro), 1, *ArqLEs); 
    bench->transfEsc++;
    (*Es)--;
}

//Funcao de Escrita na parte Esquerda do Quicksort
void EscreveMin(FILE **ArqEi, Registro R, int *Ei,Bench *bench) {
    fseek(*ArqEi, (*Ei - 1) * sizeof(Registro), SEEK_SET);
    fwrite(&R, sizeof(Registro), 1, *ArqEi); 
    bench->transfEsc++;  
    (*Ei)++; 
}

//Funcao que retira o ultimo registro da area
void RetiraMax(TipoArea *Area, Registro *R, int *NRArea) {
    RetiraUltimo(Area, R); 
    *NRArea = ObterNumCelOcupadas(Area); 
}

//Funcao que retira o primeiro registro da area
void RetiraMin(TipoArea *Area, Registro *R, int *NRArea) {
    RetiraPrimeiro(Area, R); 
    *NRArea = ObterNumCelOcupadas(Area); 
}

//Funcao Recursiva reponsavel pela realizacao da criacoes das particoes durante o quicksort
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j,Bench *bench) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq;
    int NRArea = 0;
    float Linf = FLT_MIN, Lsup = FLT_MAX;
    bool OndeLer = true; 
    Registro UltLido, R;

    fseek(*ArqLi, (Li - 1) * sizeof(Registro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(Registro), SEEK_SET);
    
    *i = Esq - 1; 
    *j = Dir + 1;

    while (Ls >= Li) {
        if (NRArea < TAMAREA -1) {
            if (OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer,bench);
            else 
                Lelnf(ArqLi, &UltLido, &Li, &OndeLer,bench);
            
            InserirArea(Area, &UltLido, &NRArea,bench);
            continue;
        }

        if (Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer,bench);
        else if (Li == Ei) 
            Lelnf(ArqLi, &UltLido, &Li, &OndeLer,bench);
        else if (OndeLer) 
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer,bench);
        else 
            Lelnf(ArqLi, &UltLido, &Li, &OndeLer,bench);

        bench->comp++;
        if (UltLido.nota > Lsup) {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es,bench);
            continue;
        }
        bench->comp++;
        if (UltLido.nota < Linf) {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei,bench);
            continue;
        }

        InserirArea(Area, &UltLido, &NRArea,bench);

        if (Ei - Esq < Dir - Es) { 
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei,bench); 
            Linf = R.nota;
        } else { 
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es,bench); 
            Lsup = R.nota;
        }
    }

    while (Ei <= Es && NRArea > 0) { 
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei,bench);
    }
}


// Funcao para uso do qsort interno, quando o subarquivo cabe em memoria principal
int cmpRegs(const void *a, const void *b) {
    float notaA = ((Registro *)a)->nota;
    float notaB = ((Registro *)b)->nota;
    if (notaA < notaB) return -1;
    if (notaA > notaB) return 1;
    return 0;
}


//Funcao principal de QuickSort Externo
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir,Bench *bench) { 
    int i, j;
    TipoArea Area; // Area de armazenamento interna
    int tamParticao = Dir - Esq + 1;
    if (tamParticao < 1) 
        return;
    
    // Qsort interno caso o tamanho do subarquivo seja menor que o da area
    if (tamParticao <= TAMAREA) {
        Registro vec[TAMAREA];
        fseek(*ArqLi, (Esq - 1) * sizeof(Registro), SEEK_SET);
        fread(vec, sizeof(Registro), tamParticao, *ArqLi);
        bench->transfLeit+= tamParticao;
        qsort(vec, tamParticao, sizeof(Registro), cmpRegs);
        fseek(*ArqLi, (Esq - 1) * sizeof(Registro), SEEK_SET);
        fwrite(vec, sizeof(Registro), tamParticao, *ArqLi);
        bench->transfEsc += tamParticao;
        fflush(*ArqLi);
        return;
    }

    FAVazia(&Area);
    Particao(ArqLi, ArqEi, ArqLEs, &Area, Esq, Dir, &i, &j,bench);

    if (i - Esq < Dir - j) { 
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i,bench);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir,bench);
    } else { 
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir,bench);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i,bench);
    }
}