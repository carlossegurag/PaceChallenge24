/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: function to calculate the permutation deviation distance of two permutations
********************************************************************************************/

#include "Individual.h"

TDistance Individual::getDistance(Individual &ind) {
	int valueToPositionI2[dim];
	for(int i=0; i < dim; i++){
		valueToPositionI2[ind.S[i]] = i;
	}

	long long contPar = 0;
	for(int i=0; i < (dim); i++){
		contPar += abs(i - valueToPositionI2[S[i]]);
	}
	return contPar;
}
