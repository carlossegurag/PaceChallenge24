/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 
Description: function to evaluate the cost associated to a permutation in the PaceChallenge
Depending on the size the LOP matrix (small cases) or the edges are used (big)
********************************************************************************************/

#include<ext/pb_ds/assoc_container.hpp>
#include "Individual.h"

extern bool isBig;

void Individual::evaluateWithMatrix(){
	cost = 0;
	for (unsigned int j = 0; j < dim - 1; j++){
		for (unsigned int i = j+1; i < dim; i++){
			if (problem){
				cost += problem->instance[S[j]][S[i]];
			} else {
				cost += problemll->instance[S[j]][S[i]];
			}
		}
	}
}

using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

void Individual::evaluateWithEdges(){
	cost = 0;
	ordered_set< pair<int, pair<int, int> > > passedEdges;
	for (int i = dim - 1; i >= 0; i--){
		for (int v : instance->graph_adjList[S[i]]){
			cost -= passedEdges.order_of_key(make_pair(v, make_pair(-1, -1)));
		}
		int c = 0;
		for (int v : instance->graph_adjList[S[i]]){
			passedEdges.insert(make_pair(v, make_pair(S[i], c)));
			c++;
		}
	}
}

void Individual::evaluate() {
	if (!isBig){
		evaluateWithMatrix();
	} else {
		evaluateWithEdges();
	}
}
