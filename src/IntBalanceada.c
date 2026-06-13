#include "IntBalanceada.h"
#include "struct.h"
#include "arquivos.h"

void mergeSort(Registro *v, int n) {
	mergerec(v, 0, n - 1);
}

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
            // FAzer buffer para escrever na fita de saida
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

void constroiHeap(Heap h[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		refazHeap(h, n, i);
}

void refazHeap(Heap h[],int n, int i){
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
	intercalarBlocos(arqBin, tam,fitas);
	liberaFitas(fitas);
}