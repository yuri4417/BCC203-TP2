#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IntBalanceada.h"
#include "struct.h"
#include "arquivos.h"

//Funcao Responsavel por Refazer o Heap, consertando a ordem de prioridade
void refazHeap(Heap h[], int n, int i, Bench *bench) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n) {
        bench->comp++;
        if (h[esq].marcado == h[menor].marcado) {
            if (h[esq].reg.nota < h[menor].reg.nota) 
                menor = esq;
        }
            
        else if (h[esq].marcado == false && h[menor].marcado == true) 
            menor = esq;
    }

    if (dir < n) {
        bench->comp++;
        if (h[dir].marcado == h[menor].marcado) {
            if (h[dir].reg.nota < h[menor].reg.nota) 
                menor = dir;
        }
            
        else if (h[dir].marcado == false && h[menor].marcado == true) 
            menor = dir;
    }

    if (menor != i) {
        Heap temp = h[i];
        h[i] = h[menor];
        h[menor] = temp;
        refazHeap(h, n, menor, bench);
    }
}

//Funcao responsavel por Construir o Heap
void constroiHeap(Heap h[], int n, Bench *bench) {
    for (int i = n / 2 - 1; i >= 0; i--)
        refazHeap(h, n, i, bench);
}

//Verifica se ha fitas ativas
int verificaFitasAtivas(int *vec, int n) {
    for (int i = 0; i < n; i++)
        if (vec[i] > 0)
            return 1;
    return 0;
}

//Funcao comparativa do Merge Sort
void merge(Registro *v, Registro *aux, int l, int m, int r, Bench *bench) {
    for (int k = l; k <= r; k++)
        aux[k] = v[k];

    int i = l;
    int j = m + 1;

    for (int k = l; k <= r; k++) {
        if (i > m) 
            v[k] = aux[j++];
        else if (j > r) 
            v[k] = aux[i++];
        else {
            bench->comp++;
            if (aux[i].nota <= aux[j].nota) 
                v[k] = aux[i++];
            else 
                v[k] = aux[j++];
        }    
    }
}

//Funcao recursiva do merge sort
void mergerec(Registro *v, Registro *aux, int l, int r, Bench *bench) {
    if (l < r) {
        int m = (l + r) / 2;
        mergerec(v, aux, l, m, bench);
        mergerec(v, aux, m + 1, r, bench);
        merge(v, aux, l, m, r, bench);
    }
}

//Funcao principal do merge sort
void mergeSort(Registro *v, int n, Bench *bench) {
    Registro *aux = (Registro*) calloc(n, sizeof(Registro));
    mergerec(v, aux, 0, n - 1, bench);
    free(aux);
}

//Funcao fundamental da geracao de blocos de acordo com o arquivo base com Merge Sort
void geraBlocos(FILE* arqBin, int tam, Fitas *fitas, Bench *bench) {
    Registro buffer[BLOCK_SIZE] = {0};
    Registro vec[TAMAREA] = {0};

    int bufferN;
    int qtdRestante = tam;
    int idxBuffer = 0;
    int fitaAtual = 0;
    int idxVec; //ultima posicao valida do buffer
	
    bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
    fread(buffer, sizeof(Registro), bufferN, arqBin); 
    bench->transfLeit++;
    qtdRestante -= bufferN;

    while (bufferN > 0) {
        idxVec = 0;
        while (idxVec < TAMAREA && bufferN > 0) {
            vec[idxVec++] = buffer[idxBuffer++];
            bufferN--;

            if (bufferN == 0 && qtdRestante > 0) {
                bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
                fread(buffer, sizeof(Registro), bufferN, arqBin); 
                bench->transfLeit++;
                qtdRestante -= bufferN;
                idxBuffer = 0;
            }
        } 
        
        mergeSort(vec, idxVec, bench);
        
        //Escreve o bloco na fita
        fwrite(vec, sizeof(Registro), idxVec, fitas->vArq[fitaAtual]);
        bench->transfEsc++;

        //Escreve -1 para sinalizar o fim do bloco
        Registro EOB = {0};
        EOB.nota = -1;
        fwrite(&EOB, sizeof(Registro), 1, fitas->vArq[fitaAtual]);
        bench->transfEsc++;

        fitas->qtdBlocos[fitaAtual]++;
        fitaAtual = (fitaAtual + 1) % QTDFITAS; //vai para proxima fita
    }
}

//Funcao fundamental da geracao de blocos de acordo com o arquivo base com Selecao por Substituicao
void geraBlocosSub(FILE* arqBin, int tam, Fitas *fitas, Bench *bench) {
    Registro buffer[BLOCK_SIZE] = {0};
    Heap vec[TAMAREA] = {0};

    int bufferN, qtdRestante = tam, idxBuffer = 0, fitaAtual = 0;

    bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
    fread(buffer, sizeof(Registro), bufferN, arqBin); 
    bench->transfLeit++;
    qtdRestante -= bufferN;
    
    //Preenche o heap
    int tamHeap = 0;
    while (tamHeap < TAMAREA && bufferN > 0) {
        vec[tamHeap].reg = buffer[idxBuffer++];
        vec[tamHeap].marcado = false;
        tamHeap++;
        bufferN--;
        
        if (bufferN == 0 && qtdRestante > 0) {
            bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
            fread(buffer, sizeof(Registro), bufferN, arqBin); 
            bench->transfLeit++;
            qtdRestante -= bufferN;
            idxBuffer = 0;
        }
    }
    constroiHeap(vec, tamHeap, bench);

    while (tamHeap > 0) {
        //Tira o primeiro (menor) do heap
        Registro temp = vec[0].reg;
        fwrite(&temp, sizeof(Registro), 1, fitas->vArq[fitaAtual]);
        bench->transfEsc++;
        
        if (bufferN > 0) {
            vec[0].reg = buffer[idxBuffer++];
            bufferN--;
            
            if (bufferN == 0 && qtdRestante > 0) { // Recarrega o buffer se necessario
                bufferN = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
                fread(buffer, sizeof(Registro), bufferN, arqBin); 
                bench->transfLeit++;
                qtdRestante -= bufferN;
                idxBuffer = 0;
            }    
            //entra o proximo elemento da fita que saiu
            if (vec[0].reg.nota < temp.nota)
                vec[0].marcado = true;
            else 
                vec[0].marcado = false;
        } 
        else 
            //Caso acabe o arquivo, o heap diminui de tamanho
            vec[0] = vec[--tamHeap];
        
        
        if (tamHeap > 0) {
            refazHeap(vec, tamHeap, 0, bench);
            
            //Verifica se todos estao marcados
            bool todosMarcados = true;
            for (int i = 0; i < tamHeap; i++) {
                if (!vec[i].marcado) {
                    todosMarcados = false;
                    break;
                }
            }
            
            //Se todos estiverem marcados, escreve o EOB e vai para a proxima fita
            if (todosMarcados) {
                Registro EOB = {0};
                EOB.nota = -1;
                fwrite(&EOB, sizeof(Registro), 1, fitas->vArq[fitaAtual]);
                bench->transfEsc++;

                fitas->qtdBlocos[fitaAtual]++;
                fitaAtual = (fitaAtual + 1) % QTDFITAS;
                
                for (int i = 0; i < tamHeap; i++)
                    vec[i].marcado = false;
                
                constroiHeap(vec, tamHeap, bench);
            }
        }
    }
    //Escreve o EOB na ultima fita
    Registro EOB = {0};
    EOB.nota = -1;
    fwrite(&EOB, sizeof(Registro), 1, fitas->vArq[fitaAtual]);
    bench->transfEsc++;
    fitas->qtdBlocos[fitaAtual]++;
}

//Funcao principal de Intercalacao dos blocos gerados
void intercalarBlocos(FILE* arqBin, Fitas* fitas, Bench *bench) {
    bool parteSaida = true; 
    Heap h[QTDFITAS] = {0};
    Registro reg = {0};
    int baseE = 0;
    int baseS = QTDFITAS;
    int tamHeap;
    
    while (1) {
        //Base das fitas de Entrada e Saida alternando entre as INTERCALACOES
        baseE = parteSaida ? 0 : QTDFITAS;
        baseS = parteSaida ? QTDFITAS : 0;
        int totalBlocos = 0;

        //Obtem o num total de blocos e da rewind para voltar cada fita pro inicio
        for (int i = 0; i < QTDFITAS; i++) {
            totalBlocos += fitas->qtdBlocos[baseE + i];
            rewind(fitas->vArq[baseE + i]);
            rewind(fitas->vArq[baseS + i]);
            fitas->qtdBlocos[baseS + i] = 0;
        }
        
        //Se tiver so um bloco ele ja eh o arquivo ordenado
        if (totalBlocos <= 1) 
            break;
            
        int idxSaidaAtual = baseS; 
        
        //Enquanto tiver fitas com blocos ele faz o heap
        while (verificaFitasAtivas(&fitas->qtdBlocos[baseE], QTDFITAS)) {
            tamHeap = 0;    
            
            for (int i = 0; i < QTDFITAS; i++) {
                if (fitas->qtdBlocos[baseE + i] > 0) {
                    if (fread(&reg, sizeof(Registro), 1, fitas->vArq[baseE + i]) == 1) {
                        bench->transfLeit++;
                        if (reg.nota != -1.0) {
                            h[tamHeap].reg = reg;
                            h[tamHeap].fitaOrigem = i;
                            h[tamHeap++].marcado = false; 
                        } else {
                            //Fim do bloco
                            fitas->qtdBlocos[baseE + i]--; 
                        }
                    }
                }
            }
            constroiHeap(h, tamHeap, bench);
            
            //Escreve o menor na fita e coloca o prox da fita de origem no heap
            while (tamHeap > 0) { 
                fwrite(&(h[0].reg), sizeof(Registro), 1, fitas->vArq[idxSaidaAtual]);   
                bench->transfEsc++;

                int origem = h[0].fitaOrigem;
                Registro temp;
                
                if (fread(&temp, sizeof(Registro), 1, fitas->vArq[baseE + origem]) == 1) {
                    bench->transfLeit++;
                    
                    //Caso fim do bloco
                    if (temp.nota == -1.0) {
                        fitas->qtdBlocos[baseE + origem]--; 
                        h[0] = h[--tamHeap];
                    } else {
                        h[0].reg = temp;
                        h[0].fitaOrigem = origem;
                        h[0].marcado = false;
                    }
                } else {
                    h[0] = h[--tamHeap]; 
                }
                    
                if (tamHeap > 0)
                    refazHeap(h, tamHeap, 0, bench);
            }


            //Escreve o EOB no fim do bloco
            Registro EOB = {0};
            EOB.nota = -1;
            fwrite(&EOB, sizeof(Registro), 1, fitas->vArq[idxSaidaAtual]);
            bench->transfEsc++;
            
            //Vai pra prox fita
            fitas->qtdBlocos[idxSaidaAtual]++; 
            idxSaidaAtual = baseS + ((idxSaidaAtual - baseS + 1) % QTDFITAS);
        }
        //Todos os blocos intercalados, troca entrada pela saida    
        for (int i = 0; i < QTDFITAS; i++) {
            fflush(fitas->vArq[baseS + i]); 
            rewind(fitas->vArq[baseE + i]);
        }    
        parteSaida = !parteSaida;
    }

    //Descobre qual fita ficou o resultado
    int fitaFinal = -1;
    for (int i = 0; i < 2 * QTDFITAS; i++) {
        if (fitas->qtdBlocos[i] > 0) {
            fitaFinal = i;
            break;
        }
    }
    
    //Volta ao inicio da fita e le ela toda
    if (fitaFinal != -1) {
        rewind(fitas->vArq[fitaFinal]);
        rewind(arqBin);
        Registro buffer[BLOCK_SIZE] = {0};
        Registro bufferLimpo[BLOCK_SIZE] = {0};
        int lidos;
        
        while ((lidos = fread(buffer, sizeof(Registro), BLOCK_SIZE, fitas->vArq[fitaFinal])) > 0) {
            bench->transfLeit++;
            int validos = 0;
            
            for (int k = 0; k < lidos; k++) {
                //Copia apenas os registros validos, pois em uma fita final vai ter varios EOB
                if (buffer[k].nota != -1.0) {
                    bufferLimpo[validos++] = buffer[k];
                }
            }
            
            if (validos > 0) {
                fwrite(bufferLimpo, sizeof(Registro), validos, arqBin);
                bench->transfEsc++;
            }
        }
        fflush(arqBin);
    }
}

//Funcao principal da Intercalacao Balanceada com Merge Sort
void intBalanceada(FILE* arqBin, int tam, Bench *bench) {
    Fitas *fitas = criaFitas();
    geraBlocos(arqBin, tam, fitas, bench);
    intercalarBlocos(arqBin, fitas, bench);
    liberaFitas(fitas);
}

//Funcao principal da Intercalacao Balanceada com Selecao por Substituicao
void intBalanceadaSub(FILE* arqBin, int tam, Bench *bench) {
    Fitas *fitas = criaFitas();
    geraBlocosSub(arqBin, tam, fitas, bench);
    intercalarBlocos(arqBin, fitas, bench);
    liberaFitas(fitas);
}