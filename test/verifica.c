#include <stdio.h>
#include <stdlib.h>
#include "../include/struct.h" 

int cmpInscricao(const void *a, const void *b) {
    long int ia = *(const long int *)a;
    long int ib = *(const long int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: ./verifica <situacao> <quantidade>\n");
        return -1;
    }
    
    int qtd = atoi(argv[2]);

    FILE *f = fopen(ARQMETODO, "rb");
    if (!f) {
        printf("Arquivo %s não encontrado.\n", ARQMETODO);
        return -1;
    }
    
    long int *inscricoes = (long int *)malloc(qtd * sizeof(long int));
    if (!inscricoes) {
        printf("Erro de alocação de memória.\n");
        fclose(f);
        return -1;
    }

    Registro atual, anterior;
    int lidos = 0;
    int falhasOrdem = 0;

    if (fread(&anterior, sizeof(Registro), 1, f) == 1) {
        inscricoes[lidos] = anterior.inscricao;
        lidos++;
        
        while (fread(&atual, sizeof(Registro), 1, f) == 1) {
            if (atual.nota < anterior.nota) { 
                falhasOrdem++;
            }
            if (lidos < qtd) 
                inscricoes[lidos] = atual.inscricao;
            
            anterior = atual;
            lidos++;
        }
    }
    fclose(f);

    int duplicatas = 0;
    if (lidos <= qtd) {
        qsort(inscricoes, lidos, sizeof(long int), cmpInscricao);
        for (int i = 1; i < lidos; i++) {
            if (inscricoes[i] == inscricoes[i - 1]) {
                duplicatas++;
            }
        }
    }
    free(inscricoes);

    printf("Leitura de Registros: ");
    if (lidos != qtd)
        printf("ERRO (Lidos: %d / Esperado: %d)\n", lidos, qtd);
    else
        printf("OK\n");
        
    printf("Ordenacao final: ");
    if (falhasOrdem > 0) 
        printf("ERRO (%d registros desordenados)\n", falhasOrdem);
    else
        printf("OK\n");

    printf("Unicidade: ");
    if (duplicatas > 0)
        printf("ERRO (%d registros duplicados)\n", duplicatas);
    else
        printf("OK\n");

    return 0;
}