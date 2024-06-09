/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 
Description: implementation of the cycle-based crossover (CX)
********************************************************************************************/

#include "Individual.h"
#include "utils.h"

//At exit, both individuls must have been evaluated 
void Individual::crossoverCX(Individual &ind) {
	//Calculate inverted indexes
	int valueToPositionI1[dim];
	calculateInvertedIndexes(valueToPositionI1);
	int valueToPositionI2[dim];
	ind.calculateInvertedIndexes(valueToPositionI2);
	//clone this because is going to change
	Individual copyI1 = internalClone();
	crossoverCXone(ind, valueToPositionI1, valueToPositionI2);
	ind.crossoverCXone(copyI1, valueToPositionI2, valueToPositionI1);
}

void Individual::crossoverCXone(Individual &ind, int *valueToPositionI1, int *valueToPositionI2) {
	long int i, si, vi;
	int EMPTY_VAL = (-1);
	int NewInd[dim];
	bzero(NewInd, dim * sizeof(int));

	//Copy matching positions
	for (i = 0; i < dim; i++) {
		if (S[i] == ind.S[i]) 
			NewInd[i] = ind.S[i];
		else 
			NewInd[i] = EMPTY_VAL;
	}

	si = getRandomInteger0_N(dim);
	for (vi = si; vi < si + dim; vi++) {
		i = vi % dim;
		if (NewInd[i] < 0) {
			//Select one of the cycles randomly
			int j, p, k;
			p = getRandomInteger0_N(2);
			j = i;
			do {//Copy cycle
				if (p == 0) {
					NewInd[j] = S[j];
					k = ind.S[j];
					j = valueToPositionI1[k];
				} else {
					NewInd[j] = ind.S[j];
					k = S[j];
					j = valueToPositionI2[k];
				}
			} while (j != i);
		}
	}

	//Apply to the individual and evaluate
	for(i=0 ; i< dim ; i++){
		S[i]=NewInd[i];
	}
	evaluate();
}

void Individual::calculateInvertedIndexes(int *indexes){
	for (unsigned int i = 0; i < dim; i++){
		indexes[S[i]] = i;
	}
}
