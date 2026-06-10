#include "struct.h"
void MergeSort(Registro *v, int n) {
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
	
	Registro *vl = (int*) malloc(sizeof(int) * sizeL);
	Registro *vr = (int*) malloc(sizeof(int) * sizeR);

	int i, j, k;
	for (i = 0; i < sizeL; i++)
		vl[i].nota = v[i + l].nota;
	for (j = 0; j < sizeR; j++)
		vr[j].nota = v[j + m + 1].nota;
		
	i = j = 0;
	k = l;
	
	while (i < sizeL && j < sizeR) {
		if (vl[i].nota <= vr[j].nota)
			v[k++].nota = vl[i++].nota;
		else
			v[k++].nota = vr[j++].nota;
	}
	
	while (i < sizeL)
		v[k++].nota = vl[i++].nota;
		
	while (j < sizeR)
		v[k++].nota = vr[j++].nota;
		
	free(vl);
	free(vr);
}