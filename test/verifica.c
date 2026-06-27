#include <stdio.h>
#include <stdlib.h>
#include "../include/struct.h" 

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: ./verifica <situacao> <quantidade>\n");
        return 1;
    }

    int situacao = atoi(argv[1]);
    int qtd = atoi(argv[2]);

    FILE *f = fopen(ARQMETODO, "rb");
    if (!f) {
        printf("Arquivo %s não encontrado.\n", ARQMETODO);
        return 1;
    }

    Registro atual, anterior;
    int lidos = 0;
    int falhasOrdem = 0;

    if (fread(&anterior, sizeof(Registro), 1, f) == 1) {
        lidos++;
        while (fread(&atual, sizeof(Registro), 1, f) == 1) {
            if (situacao == ARQCRESC && atual.nota < anterior.nota) falhasOrdem++;
            if (situacao == ARQDESC && atual.nota > anterior.nota) falhasOrdem++;
            
            anterior = atual;
            lidos++;
        }
    }
    fclose(f);
    printf("Leitura de Registros: ");
    
    if (lidos != qtd)
        printf("ERRO: Registros lidos /= Quantidade total\n");
    else
        printf("OK\n");
        
    printf("Ordenacao final: ");
    if (falhasOrdem > 0) 
        printf("ERRO: Arquivo desordenado! %d registros desordenados.\n", falhasOrdem);
    else
        printf("OK\n");
    return 0;
}