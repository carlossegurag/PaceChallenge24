/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: some internal operations required by MA:
- getCost as the negative of the cost because it is a maximization problem (LOP)
- clone individuals
********************************************************************************************/

#include "Individual.h"
#include <sys/time.h>

Individual::Individual(){
	S.resize(dim);
}

TFitness Individual::getCost(){
	return -cost;
}

Individual Individual::internalClone() {
	Individual tempInd;
	tempInd.S=S;
	tempInd.cost=cost;
	tempInd.distance=distance;
	return tempInd;
}
