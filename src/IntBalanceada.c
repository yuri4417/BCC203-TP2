#include <stdlib.h>
#include "IntBalanceada.h"
#include "struct.h"
#include "arquivos.h"

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


/*
**1. Configuração Inicial**

* Defina quais são as fitas de entrada (ex: índices `0` a `QTDFITAS-1`) e as fitas de saída (ex: índices `QTDFITAS` a `2*QTDFITAS - 1`).
* Crie uma variável para rastrear a **fita de saída atual** (iniciando no primeiro índice do grupo de saída).
* Crie um array (ex: `fitaAtiva[QTDFITAS]`) inicializado com `1` (verdadeiro) para controlar se cada fita de entrada ainda possui dados ou se chegou ao Fim de Arquivo (EOF).

**2. Loop Principal (Executa enquanto houver dados nas fitas de entrada)**

* Zere o array que conta os registros lidos por fita no bloco atual (o seu `idxFitas`).
* Zere o tamanho do Heap.
* **Repovoamento do Heap:** Para cada fita de entrada que ainda está ativa (`fitaAtiva[i] == 1`), tente ler **um** registro.
* Se a leitura for bem-sucedida: coloque o registro no Heap, guarde a origem dele (`fitaOrigem`), incremente o contador de registros lidos dessa fita (`idxFitas[i] = 1`) e aumente o tamanho do Heap.
* Se a leitura falhar (EOF): marque `fitaAtiva[i] = 0`.


* Após verificar todas as fitas, chame a função para construir o Min-Heap (`constroiHeap`).
* **Condição de Parada:** Se o tamanho do Heap for `0` após essa etapa, significa que todas as fitas de entrada acabaram. Interrompa o Loop Principal (saia do loop).

**3. Loop de Intercalação do Bloco (Executa enquanto o tamanho do Heap > 0)**

* Pegue a raiz do Heap (o menor elemento) e descubra de qual fita ele veio (`fitaOrigem`).
* Grave este registro na **fita de saída atual**.
* **Verificação de limite do bloco:** Verifique se a fita de origem ainda não esgotou o seu bloco (ou seja, se `idxFitas[fitaOrigem] < TAMAREA`).
* **Se o bloco não acabou:** Tente ler o próximo registro da `fitaOrigem`.
* Se ler com sucesso: Substitua a raiz do Heap por esse novo registro, incremente `idxFitas[fitaOrigem]` e chame `refazHeap`.
* Se falhar (EOF no meio do bloco): Marque `fitaAtiva[fitaOrigem] = 0`, mova o último elemento do Heap para a raiz, diminua o tamanho do Heap em 1 e chame `refazHeap`.


* **Se o bloco acabou:** Não faça leitura. Apenas mova o último elemento do Heap para a raiz, diminua o tamanho do Heap em 1 e chame `refazHeap`.



**4. Transição de Bloco (Após o Heap esvaziar)**

* O loop interno (passo 3) terminou, o que significa que um bloco gigante foi totalmente formado e gravado na fita de saída atual.
* Avance a **fita de saída atual** para a próxima. Use aritmética modular para que o índice volte ao início do grupo de saída caso ultrapasse o limite (ex: `fitaSaida = QTDFITAS + ((fitaSaida - QTDFITAS + 1) % QTDFITAS)`).
* O Loop Principal recomeça do Passo 2 para processar os próximos blocos das fitas de entrada.
*/


int verificaFitasAtivas(int *vec, int n) {
	for (int i = 0; i < n; i++)
		if (vec[i])QTDFITAS
			return 1;

	return 0;
}

void intercalarBlocos(FILE** arqBin, int tam, Fitas* fitas) {
	int idxSaidaAtual = QTDFITAS;
	int idxFitas[2 * QTDFITAS] = {0};
	int fitaAtiva[QTDFITAS];
	Heap h[QTDFITAS];
	Registro reg;
	int tamHeap;
	for (int i = 0; i < QTDFITAS; i++)
		fitaAtiva[i] = 1;
	while (verificaFitasAtivas(fitaAtiva, QTDFITAS)) {
		for (int i = 0; i < 2 * QTDFITAS; i++)
			idxFitas[i] = 0;
		tamHeap = 0;
		for (int i = 0; i < QTDFITAS; i++)	
			if(fread(&reg, sizeof(Registro), 1, arqBin) == 1) {
				h[tamHeap++] = reg;
				constroiHeap(h, tamHeap);
				idxFitas[i]++;	
			}
			else
				fitaAtiva[i] = 0;
	
	}
	
}

/*
void intercalarBlocos(FILE** arqBin, int tam, Fitas* fitas){
	Heap h[QTDFITAS];
    int blocoValido[2 * QTDFITAS] = {1};
    int idxFitas[2 * QTDFITAS] = {0};

	int tamHeap = 0;
	Registro reg;
    int BlocoAtual = 0;
    int qtdBlocosMax = fitas->qtdBlocos[0];
    int i = 0;

	for (int i =0;i < QTDFITAS;i++){
		rewind(fitas->vArq[i]);
		if(fread(&reg, sizeof(Registro), 1, fitas->vArq[i]) == 1){
            idxFitas[i]++;
			h[tamHeap].reg = reg;
			h[tamHeap].fitaOrigem = i;
			tamHeap++;
		}
	}
	constroiHeap(h, tamHeap);

    for (int i = 1; i < QTDFITAS; i++)
        if (fitas->qtdBlocos[i] > qtdBlocosMax)
            qtdBlocosMax = fitas->qtdBlocos[i];

    for (int i = 0; i < qtdBlocosMax; i++) {
		FILE *arqSaida = fitas->vArq[QTDFITAS + i];
        while(tamHeap > 0) {
            Heap menor = h[0];
            // Fazer buffer para escrever na fita de saida
            fwrite(&menor.reg, sizeof(Registro), 1, arqSaida);

            // Atualizar a qtd de blocos direitinho, consertar uso de idxFitas para funcionar com os proximos blocos
            if(blocoValido[menor.fitaOrigem] && (fread(&reg, sizeof(Registro), 1, fitas->vArq[menor.fitaOrigem]) == 1)) {
                idxFitas[menor.fitaOrigem]++;
                h[0].reg = reg;
                h[0].fitaOrigem = menor.fitaOrigem;
            } else {
                if (idxFitas[menor.fitaOrigem] == TAMAREA)
                    fitas->qtdBlocos[menor.fitaOrigem]--;
                h[0] = h[tamHeap - 1];
                tamHeap--;
            }
            refazHeap(h, tamHeap, 0);
        }

	}
}
*/

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
	intercalarBlocos(&arqBin, tam,fitas);
	liberaFitas(fitas);
}