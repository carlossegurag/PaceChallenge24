/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: class for the Memetic Algorithm with Explicit Diversity Management (MA-EDM)
published in the paper "A Diversity-aware Memetic Algorithm for the Linear Ordering Problem".
It was modified for the Pace Challenge 2024 with the following features:
- More complex perturbations
- More efficient local search with the use of gain matrices and fast break
- Use of long long only when required
********************************************************************************************/

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <string>

#include "MA.h"
#include "utils.h"

using namespace std;

extern volatile bool finished;

MA::MA(int N_, double pc_, const string &crossType_ , const string &perturbationType_, double DIfactor_, double ils_time_,double finalTime_, const string &outputFile_, int cuttingMult_, int swaps_, bool reqLongLong_){
	N = N_;
	pc = pc_;
	crossType= crossType_;
	perturbationType=perturbationType_;
	DIfactor= DIfactor_;
	ils_time=ils_time_;
	finalTime = finalTime_;
	outputFile = outputFile_;
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	cuttingMult = cuttingMult_;
	swaps = swaps_;
	reqLongLong = reqLongLong_;
}

//Initialize and apply intensification to each individual
void MA::initPopulation(){
	for (int i = 0; i < N; i++){
		Individual *ei = new Individual();
		ei->initialize_heuristic();
		ei->intensify(perturbationType,ils_time, cuttingMult, swaps, reqLongLong);
		population.push_back(ei);
	}
}

//Select two parents with binary selection
void MA::selectParents(){
	parents.clear();
	for (int i = 0; i < population.size(); i++){
		int first = getRandomInteger0_N(N - 1);
		int second = getRandomInteger0_N(N - 1);
		if (population[first]->getCost() <= population[second]->getCost()){
			parents.push_back(population[first]);
		} else {
			parents.push_back(population[second]);
		}
	}
}

//Apply crossover: two crossover operators were implemented cx and bx
void MA::crossover(){
	for (int i = 0; i < parents.size(); i++){
		Individual *ni = new Individual();
		*ni = *parents[i];
		offspring.push_back(ni);
	}

	for (int i = 0; i < offspring.size() - N%2; i+=2) {
		if (generateRandomDouble0_Max(1) <= pc) {
			offspring[i]->crossoverCX(*offspring[i + 1]);
		}
	}
}

void MA::intensify(){
	for (int i = 0; i < offspring.size(); i++){
		if (finished) return;
		offspring[i]->intensify(perturbationType,ils_time, cuttingMult, swaps, reqLongLong);
	}
}

void MA::replacement(){
	vector < Individual* > all;
	
	//Join population and offspring
	for (int i = 0; i < population.size(); i++){
		all.push_back(population[i]);
		all.back()->distance = MAX_DISTANCE;
	}
	population.clear();

	for (int i = 0; i < offspring.size(); i++){
		all.push_back(offspring[i]);
		all.back()->distance = MAX_DISTANCE;
	}
	offspring.clear();
	
	//Select best solution
	int indexBest = 0;
	for (int i = 1; i < all.size(); i++){
		if (all[i]->getCost() < all[indexBest]->getCost()){
			indexBest = i;
		}
	}
	population.push_back(all[indexBest]);
	all[indexBest] = all.back();
	all.pop_back();

	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	elapsedTime -= initialTime;

	//Select next N - 1 solution
	double D = DI - DI * elapsedTime / finalTime;
	while(population.size() != N){
		//Update distances
		for (int i = 0; i < all.size(); i++){
			all[i]->distance = min(all[i]->distance, all[i]->getDistance(*population.back()));
		}
		//Select best option
		indexBest = 0;
		for (int i = 1; i < all.size(); i++){
			bool betterInDist =	(all[i]->distance > all[indexBest]->distance);
			bool eqInDist = (all[i]->distance == all[indexBest]->distance);
			bool betterInFit = (all[i]->getCost() < all[indexBest]->getCost());
			bool eqInFit = (all[i]->getCost() == all[indexBest]->getCost());

			if (all[indexBest]->distance < D){//Do not fulfill distance requirement
				if ((betterInDist) || (eqInDist && betterInFit)){
					indexBest = i;
				}
			} else {
				if (all[i]->distance >= D){
					if ((betterInFit) || (eqInFit && betterInDist)){
						indexBest = i;
					}
				}
			}
		}
		//Insert best option
		population.push_back(all[indexBest]);
		all[indexBest] = all.back();
		all.pop_back();
	}
	//Release memory
	for (int i = 0; i < all.size(); i++){
		delete(all[i]);
	}
}

void MA::initDI(){
	double meanDistance = 0;
	for (int i = 0; i < population.size(); i++){
		for (int j = i + 1; j < population.size(); j++){
			meanDistance += population[i]->getDistance(*population[j]);
		}
	}
	meanDistance /= (population.size() * (population.size() - 1)) / 2;
	DI = meanDistance*DIfactor;
}

void MA::run() {
	ofstream f;
	f.open(outputFile, ios::app);
	initPopulation();
	initDI();
	double cTime;
	
	do {
		//Iteration of the MA: selection, crossover, intensification, replacement
		selectParents();
		crossover();
		intensify();
		replacement();
		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);
		cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec) / 1.0e6;
		elapsedTime = cTime - initialTime;
	}while ((cTime - initialTime < finalTime) && (!finished));
}
