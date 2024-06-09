/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: it is the entry point. It initializes the paramters and decides to use
the ILS of the mememtic algorithm depending on the size of the problem to be solved 
(after reduction). It also incorporates the part related to the management of signals
for the PaceChallenge 2024.
*******************************************************************************************/

#include <fstream>
#include <iostream>

using namespace std;

#include <sys/time.h>
#include <sys/resource.h>
#include <bits/stdc++.h>
#include "utils.h"
#include "Individual.h"
#include "Problem.h"
#include "ProblemLL.h"
#include "Instance.h"
#include "MA.h"

bool volatile finished = false;

void printer(int signal){
	finished = true;
}

bool isBig = false;
long long crossRemoved = 0;
Problem *Individual::problem;
ProblemLL *Individual::problemll;
Instance *Individual::instance;
int Individual::permBlock;
int Individual::dim;
string Individual::initialization;

int main(int argc, char *argv[]){
	signal(SIGTERM, printer);
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	srandom(time(NULL));
	Instance *inst = new Instance(true);
	Problem *p = NULL;
	ProblemLL *pll = new ProblemLL();
	bool reqLongLong = true;
	if (!isBig){
		long long max_gain = 0;
		for (int i = 0; i < inst->numNodesB; i++){
			for (int j = 0; j < inst->numNodesB; j++){
				pll->instance[i][j] = inst->pair_matrix[i][j];
				pll->gain[i][j] = inst->pair_matrix[i][j] - inst->pair_matrix[j][i];
				max_gain = max(max_gain, llabs(pll->gain[i][j]));
			}
		}
		if (max_gain <= ((1ll<<31) - 1)){
			reqLongLong = false;
		}
		/*long long sumInstance = 0;
		for (int i = 0; i < inst->numNodesB; i++){
			for (int j = 0; j < inst->numNodesB; j++){
				sumInstance += min(pll->instance[i][j], pll->instance[j][i])*(-1);
			}
		}
		if (sumInstance <= (2ll<<31) - 1){
			reqLongLong = false;
		}*/
	}
	if (!reqLongLong){
		delete pll; pll = NULL;
		p = new Problem();
		for (int i = 0; i < inst->numNodesB; i++){
			for (int j = 0; j < inst->numNodesB; j++){
				p->instance[i][j] = inst->pair_matrix[i][j];
				p->gain[i][j] = inst->pair_matrix[i][j] - inst->pair_matrix[j][i];
			}
		}
	}

	Individual::dim = inst->numNodesB,
	Individual::problem = p;
	Individual::problemll = pll;
	Individual::instance = inst;

	string alg;
	double ils_time, pc, di;
	int f1Time;
	int cuttingMult = 45, swaps = 6;
	int N = 36; 
	if (isBig){
		alg = "ILS";
		Individual::initialization = "Score";
	} else {
		alg = "MA_matrix";
		ils_time = 0.05;
		pc = 0.85;
		di = 0.4;
		f1Time = 230;
		Individual::initialization = "Random";
	}
	Individual::permBlock = 60;


	gettimeofday(&currentTime, NULL);
	double cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	if (alg == "MA_matrix"){
		MA ma(N, pc, "cx", "SWM", di, ils_time, f1Time, "out.txt", cuttingMult, swaps, reqLongLong);
		ma.run();
		gettimeofday(&currentTime, NULL);
		cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
		if (295-(cTime - initialTime) > 0) ma.population[0]->intensify("SWM",295-(cTime - initialTime), cuttingMult*3, swaps, reqLongLong);
		//cout << "Mejor solucion: " << ma.population[0]->cost - crossRemoved << endl;
		for (int nv : ma.population[0]->S){
			cout << inst->numNodesA + 1 + inst->newToOrig[nv] << endl;
			for (int o : inst->similarInOrig[inst->newToOrig[nv]]){
				cout << inst->numNodesA + 1 + o << endl;
			}
		}
	} else if (alg == "ILS"){
		//Estimate medwin
		long long tot = 0;
		long long times = 1000000;
		vector<long long> values;
		for (int i = 0; i < times; i++){
			int f = 0, s = 0;
			while(f == s){
				f = random() % inst->numNodesB;
				s = random() % inst->numNodesB;
			}
			values.push_back(abs(inst->pair_cost(f, s)));
		}
		sort(values.begin(), values.end());
		inst->medWin = values[values.size() / 2];
		Individual *ei = new Individual();
		ei->initialize_heuristic();
		gettimeofday(&currentTime, NULL);
		cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
		ei->ils_edges(5*60-5-(cTime - initialTime), cuttingMult, swaps);
		//cout << "Mejor solucion: " << ei->cost - crossRemoved << endl;
		for (int nv : ei->S){
			cout << inst->numNodesA + 1 + inst->newToOrig[nv] << endl;
			for (int o : inst->similarInOrig[inst->newToOrig[nv]]){
				cout << inst->numNodesA + 1 + o << endl;
			}
		}
	}
	gettimeofday(&currentTime, NULL);
	cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	//cout << "Tiempo transcurrido " << cTime - initialTime << endl;
	return 0;
} 
