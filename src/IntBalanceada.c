#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IntBalanceada.h"
#include "struct.h"
#include "arquivos.h"


#ifdef _WIN32
    #include <io.h>
    #define truncate(arq) _chsize(_fileno(arq), 0)
#else
    #include <unistd.h>
    #define truncate(arq) ftruncate(fileno(arq), 0)
#endif




void mergerec(Registro *v, int l, int r) {
	if (l < r) {
		int m = (l + r) / 2;
		mergerec(v, l, m);
		mergerec(v, m + 1, r);
		merge(v, l, m, r);
	}
}


void merge(Registro *v, int l, int m, int r) {
	int sizeL = m - l + 1;
	int sizeR = r - m;
	
	Registro *vl = (Registro*) malloc(sizeof(Registro) * sizeL);
	Registro *vr = (Registro*) malloc(sizeof(Registro) * sizeR);

	int i, j, k;
	for (i = 0; i < sizeL; i++)
		vl[i] = v[i + l];
	for (j = 0; j < sizeR; j++)
		vr[j] = v[j + m + 1];
		
	i = j = 0;
	k = l;
	
	while (i < sizeL && j < sizeR) {
		if (vl[i].nota <= vr[j].nota)
			v[k++] = vl[i++];
		else
			v[k++] = vr[j++];
	}
	
	while (i < sizeL)
		v[k++] = vl[i++];
		
	while (j < sizeR)
		v[k++] = vr[j++];
		
	free(vl);
	free(vr);
}

void mergeSort(Registro *v, int n) {
	mergerec(v, 0, n - 1);
}


void geraBlocos(FILE* arqBin, int tam, Fitas *fitas) {
    Registro buffer[BLOCK_SIZE];
    Registro vec[TAMAREA];

    int bufferN;
    int qtdRestante = tam;
    int idxBuffer = 0;
    int fitaAtual = 0;
    int idxVec;
	
    bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
    fread(buffer, sizeof(Registro), bufferN, arqBin); 
    qtdRestante -= bufferN;

    while(bufferN > 0) {
        idxVec = 0;
        while (idxVec < TAMAREA && bufferN > 0) {
            vec[idxVec++] = buffer[idxBuffer++];
            bufferN--;

            if (bufferN == 0 && qtdRestante > 0) {
                bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
                fread(buffer, sizeof(Registro), bufferN, arqBin); 
                qtdRestante -= bufferN;
                idxBuffer = 0;
            }
        } 
        mergeSort(vec, idxVec);
        fwrite(vec, sizeof(Registro), idxVec, fitas->vArq[fitaAtual]);
        fitas->qtdBlocos[fitaAtual]++;
        fitaAtual = (fitaAtual + 1) % QTDFITAS;
    }
}

int verificaFitasAtivas(int *vec, int n) {
    for (int i = 0; i < n; i++)
        if (vec[i] > 0)
            return 1;
    return 0;
}

void intercalarBlocos(FILE* arqBin, Fitas* fitas) {
    int idxFitas[QTDFITAS] = {0}; // Controla quantos registros foram lidos do bloco atual de cada fita
    bool parteSaida = true; // true -> fitas de saida = 20-39 / false -> fitas de saida = 0-19
    Heap h[QTDFITAS];
    Registro reg;
    int baseE = 0;
    int baseS = QTDFITAS;
    int tamHeap;
    long int tamBloco = TAMAREA; 

    while (1) {
        baseE = parteSaida ? 0 : QTDFITAS;
        baseS = parteSaida ? QTDFITAS : 0;
        int totalBlocos = 0;

        // Reposiciona os ponteiros e conta blocos ativos
        for (int i = 0; i < QTDFITAS; i++) {
            totalBlocos += fitas->qtdBlocos[baseE + i];
            rewind(fitas->vArq[baseE + i]);
            rewind(fitas->vArq[baseS + i]);
            fitas->qtdBlocos[baseS + i] = 0;
        }
            
        // Condição de parada, ocorre quando apenas 1 bloco ordenado no total
        if (totalBlocos <= 1) 
            break;

        int idxSaidaAtual = baseS; 

        while (verificaFitasAtivas(&fitas->qtdBlocos[baseE], QTDFITAS)) {
            tamHeap = 0;    
            
            for (int i = 0; i < QTDFITAS; i++) 
                idxFitas[i] = 0;
            for (int i = 0; i < QTDFITAS; i++){    
                if (fitas->qtdBlocos[baseE + i] > 0) {
                    if(fread(&reg, sizeof(Registro), 1, fitas->vArq[baseE + i]) == 1) {
                        h[tamHeap].reg = reg;
                        h[tamHeap++].fitaOrigem = i;
                        idxFitas[i]++;    
                    }
                }
            }
            
            constroiHeap(h, tamHeap);

            // Processa o Heap até esvaziar o bloco atual
            while (tamHeap > 0) {
                // Grava o menor elemento na fita de saída atual
                fwrite(&(h[0].reg), sizeof(Registro), 1, fitas->vArq[idxSaidaAtual]);   
                int origem = h[0].fitaOrigem;
                if (idxFitas[origem] < tamBloco) {
                    Registro temp;
                    if (fread(&temp, sizeof(Registro), 1, fitas->vArq[baseE + origem]) == 1) {
                        idxFitas[origem]++;
                        h[0].reg = temp;
                        h[0].fitaOrigem = origem;
                    }
                    else {
                        h[0] = h[--tamHeap]; // Fim físico da fita
                    }
                }
                else {
                    h[0] = h[--tamHeap]; // Fim lógico do bloco
                }
                
                refazHeap(h, tamHeap, 0);
            }
            
            fitas->qtdBlocos[idxSaidaAtual]++; // Registra que um bloco inteiro foi gerado na saída

            for(int i = 0; i < QTDFITAS; i++) {
                if (idxFitas[i] > 0 && fitas->qtdBlocos[baseE + i] > 0) 
                    fitas->qtdBlocos[baseE + i]--;
            }
            
            idxSaidaAtual = baseS + ((idxSaidaAtual - baseS + 1) % QTDFITAS);
        }    
        
        for(int i = 0; i < QTDFITAS; i++) 
            fflush(fitas->vArq[baseS + i]);
        
        parteSaida = !parteSaida;
        for (int i = 0; i < QTDFITAS; i++) {
            rewind(fitas->vArq[baseE + i]);
			truncate(fitas->vArq[baseE + i]);
        }
        tamBloco *= QTDFITAS; 
    }

    int fitaFinal = -1;
    for (int i = 0; i < 2 * QTDFITAS; i++) {
        if (fitas->qtdBlocos[i] > 0) {
            fitaFinal = i;
            break;
        }
    }
    
    if (fitaFinal != -1) {
        rewind(fitas->vArq[fitaFinal]);
        rewind(arqBin);
        while (fread(&reg, sizeof(Registro), 1, fitas->vArq[fitaFinal]) == 1)
            fwrite(&reg, sizeof(Registro), 1, arqBin);
            
        fflush(arqBin);
    }

}


void constroiHeap(Heap h[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		refazHeap(h, n, i);
}

void refazHeap(Heap h[],int n, int i)
{
	int menor = i;
	int esq = 2*i + 1;

	int dir = 2*i + 2;

	if(esq < n && h[esq].reg.nota < h[menor].reg.nota)
		menor = esq;

	if (dir < n && h[dir].reg.nota < h[menor].reg.nota)
		menor = dir;

	if (menor != i){
		trocaHeap(&h[i], &h[menor]);
		refazHeap(h, n, menor);
	}

}

void trocaHeap(Heap *a, Heap *b) {
	Heap temp = *a;
	*a = *b;
	*b = temp;
}

void intBalanceada(FILE* arqBin, int tam) {
	Fitas *fitas = criaFitas();
	geraBlocos(arqBin, tam, fitas);
	intercalarBlocos(arqBin, fitas);
	liberaFitas(fitas);
}