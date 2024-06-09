/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: Individual class that is used by the MA-EDM solver. It is adapted to the 
Crossing Minimization Problem and mapped to the Linear Ordering Problem in the small cases.
********************************************************************************************/

#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include <bits/stdc++.h>
#include <limits.h>
#include "Problem.h"
#include "ProblemLL.h"
#include "Instance.h"
#define TFitness long long 
#define TDistance long long
#define MAX_DISTANCE LLONG_MAX

class Individual {
	public:
		Individual();
		~Individual(){};
		void initialize_heuristic();
		void initialize_Random();
		void initialize_byMean();
		void initialize_byScore();
		TDistance getDistance(Individual &ind);
		//Crossover Operator
		void crossoverCX(Individual &ind);//Cross both individuals (the result is saved in the individuals) with cycle Crossover
		void crossoverCXone(Individual &ind, int *valueToPositionI1, int *valueToPositionI2);
		//Mutation Operator
		void local_search(int cuttingMult);
		void local_search_ll(int cuttingMult);//using long long variables
		void local_search_edges(double time_limit, int cuttingMult);
		void intensify(const string &mutationType, double ils_time, int cuttingMult, int swaps, bool reqll);
		void ils_edges(double ils_time, int cuttingMult, int swaps);
		
		void evaluate();
		void evaluateWithMatrix();
		void evaluateWithEdges();
		void calculateInvertedIndexes(int *indexes);
		Individual internalClone();
		TFitness getCost();
	
		//Static members (shared by all individuals)
		static Problem *problem;
		static ProblemLL *problemll;
		static Instance *instance;
		static int permBlock;
		static int dim;
		static string initialization;
		
		//Normal members
		vector<int> S;
		TDistance distance;
		TFitness cost;
};

#endif
