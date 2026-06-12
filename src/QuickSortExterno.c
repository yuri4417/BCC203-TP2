#include "QuickSortExterno.h"
#include "struct.h"
#include <stdio.h>
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
void InsereItem(Registro R, TipoArea *Area) {
    if (Area->ocupadas >= TAMAREA) {
        return;
    }

    int i = Area->ocupadas - 1;

    // Desloca os elementos maiores para a direita para abrir espaço
    // Altere 'nota' para o campo correto se o critério de ordenação for outro
    while (i >= 0 && Area->itens[i].nota > R.nota) {
        Area->itens[i + 1] = Area->itens[i];
        i--;
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
void LeSup(FILE **ArqLEs, Registro *UltLido, int *Ls, bool *OndeLer) {
    fseek(*ArqLEs, (*Ls - 1) * sizeof(Registro), SEEK_SET);
    fread(UltLido, sizeof(Registro), 1, *ArqLEs);
    (*Ls)--; 
    *OndeLer = false;
}

//Funcao que simula o ponteiro de leitura Esquerdo do QuickSort
void Lelnf(FILE **ArqLi, Registro *UltLido, int *Li, bool *OndeLer) {
    fseek(*ArqLi, (*Li - 1) * sizeof(Registro), SEEK_SET);
    fread(UltLido, sizeof(Registro), 1, *ArqLi);
    (*Li)++; 
    *OndeLer = true;
}

void InserirArea(TipoArea *Area, Registro *UltLido, int *NRArea) {
    /* Insere UltLido de forma ordenada na Area */
    InsereItem(*UltLido, Area); 
    *NRArea = ObterNumCelOcupadas(Area);
}

//Funcao de Escrita na parte direita do Quicksort
void EscreveMax(FILE **ArqLEs, Registro R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(Registro), SEEK_SET);
    fwrite(&R, sizeof(Registro), 1, *ArqLEs); 
    fflush(*ArqLEs);
    (*Es)--;
}

//Funcao de Escrita na parte Esquerda do Quicksort
void EscreveMin(FILE **ArqEi, Registro R, int *Ei) {
    fseek(*ArqEi, (*Ei - 1) * sizeof(Registro), SEEK_SET);
    fwrite(&R, sizeof(Registro), 1, *ArqEi); 
    fflush(*ArqEi);
    (*Ei)++; 
}


void RetiraMax(TipoArea *Area, Registro *R, int *NRArea) {
    RetiraUltimo(Area, R); 
    *NRArea = ObterNumCelOcupadas(Area); 
}

void RetiraMin(TipoArea *Area, Registro *R, int *NRArea) {
    RetiraPrimeiro(Area, R); 
    *NRArea = ObterNumCelOcupadas(Area); 
}

//Funcao Recursiva reponsavel pela realizacao da criacoes das particoes durante o quicksort
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j) {
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
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            else 
                Lelnf(ArqLi, &UltLido, &Li, &OndeLer);
            
            InserirArea(Area, &UltLido, &NRArea);
            continue;
        }

        if (Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei) 
            Lelnf(ArqLi, &UltLido, &Li, &OndeLer);
        else if (OndeLer) 
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else 
            Lelnf(ArqLi, &UltLido, &Li, &OndeLer);

        if (UltLido.nota > Lsup) {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }
        if (UltLido.nota < Linf) {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        InserirArea(Area, &UltLido, &NRArea);

        if (Ei - Esq < Dir - Es) { 
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei); 
            Linf = R.nota;
        } else { 
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es); 
            Lsup = R.nota;
        }
    }

    while (Ei <= Es && NRArea > 0) { 
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
    //printf("Ei=%d Es=%d NRArea=%d\n", Ei, Es, NRArea);
}

//Funcao principal de QuickSort Externo
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) { 
    int i, j;
    TipoArea Area; // Area de armazenamento interna
    
    //printf("Esq=%d Dir=%d\n", Esq, Dir);
    if (Dir - Esq < 1) return;
    
    FAVazia(&Area);
    Particao(ArqLi, ArqEi, ArqLEs, &Area, Esq, Dir, &i, &j);
    //printf("i=%d j=%d\n", i, j);
    if (i - Esq < Dir - j) { 
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else { 
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}