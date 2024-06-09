/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: 
*Implementation of a stochatic hill-climber with the move operation. Initially the
whole neighborhood was traversed in O(n^2). However for Pace Challenge a
break is done if the current gain is too low (local_search())
*Implementation of iterated local search ils using three types of perturbations (intensify())
*It operates with long long only when required
********************************************************************************************/

#include "Individual.h"
#include "utils.h"
#include <bits/stdc++.h>
#include <random>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <set>

extern volatile bool finished;
extern long long crossRemoved;
long long best = LLONG_MIN;

void Individual::local_search_ll(int cuttingMult){
	//Sort the indexes randomly to make it stochastic
	int vs[dim];
	for (int i = 0; i < dim; i++) {
		vs[i] = i;
	}
	random_shuffle(vs, vs + dim);
	//Identify positions of each value
	int vToPos[dim];
	for (int i = 0; i < S.size(); i++){
		vToPos[S[i]] = i;
	}
	//Establish the worsening limit
  bool changed = true;
	long long limit = (long long)(instance->medWin) * (-cuttingMult);
	while (changed) {
		changed = false;
		for (int indexv = 0; indexv < dim; indexv++) {
			//Select a value to move, and look for its best new position
			long long gain = 0, maxgain = 0;
			vector<int> winners;
			int value = vs[indexv];
			int i = vToPos[value];
			winners.push_back(i);
			for (int j = i - 1; j >= 0; j--) {//Move to the left
				gain += problemll->gain[value][S[j]];
				if (gain < limit + maxgain){
					break;
				}
				if (gain > maxgain) {
					maxgain = gain;
					winners.clear();
					winners.push_back(j);
				} else if (gain == maxgain){
					winners.push_back(j);
				}
			}

			gain = 0;
			for (int j = i + 1; j < dim; j++) {//Move to the right
				gain += problemll->gain[S[j]][value];
				if (gain < limit + maxgain){
					break;
				}
				if (gain > maxgain) {
					maxgain = gain;
					winners.clear();
					winners.push_back(j);
				} else if (gain == maxgain){
					winners.push_back(j);
				}
			}
			if (maxgain > 0) {//If there is a gain, move it
				changed = true;
			}
			int j = winners[random()%winners.size()];
			int t = S[i];
			if (i < j)
				for (int k = i + 1; k <= j; k++) {
					S[k - 1] = S[k];
					vToPos[S[k-1]] = k - 1;
				}
			else
				for (int k = i - 1; k >= j; k--) {
					S[k + 1] = S[k];
					vToPos[S[k+1]] = k + 1;
				}
			S[j] = t;
			vToPos[S[j]] = j;
			cost += maxgain;
			if (finished) return;
		}
	}
}


void Individual::local_search(int cuttingMult){
	//Sort the indexes randomly to make it stochastic
	int vs[dim];
	for (int i = 0; i < dim; i++) {
		vs[i] = i;
	}
	random_shuffle(vs, vs + dim);
	//Identify positions of each value
	int vToPos[dim];
	for (int i = 0; i < S.size(); i++){
		vToPos[S[i]] = i;
	}
	//Establish the worsening limit
  bool changed = true;
	long long limit = (long long)(instance->medWin) * (-cuttingMult);
	while (changed) {
		changed = false;
		for (int indexv = 0; indexv < dim; indexv++) {
			//Select a value to move, and look for its best new position
			long long gain = 0, maxgain = 0;
			vector<int> winners;
			int value = vs[indexv];
			int i = vToPos[value];
			winners.push_back(i);
			for (int j = i - 1; j >= 0; j--) {//Move to the left
				gain += problem->gain[value][S[j]];

				if (gain < limit + maxgain){
					break;
				}
				if (gain > maxgain) {
					maxgain = gain;
					winners.clear();
					winners.push_back(j);
				} else if (gain == maxgain){
					winners.push_back(j);
				}
			}

			gain = 0;
			for (int j = i + 1; j < dim; j++) {//Move to the right
				gain += problem->gain[S[j]][value];
				if (gain < limit + maxgain){
					break;
				}
				if (gain > maxgain) {
					maxgain = gain;
					winners.clear();
					winners.push_back(j);
				} else if (gain == maxgain){
					winners.push_back(j);
				}
			}
			if (maxgain > 0) {//If there is a gain, move it
				changed = true;
			}
			int j = winners[random()%winners.size()];
			int t = S[i];
			if (i < j)
				for (int k = i + 1; k <= j; k++) {
					S[k - 1] = S[k];
					vToPos[S[k-1]] = k - 1;
				}
			else
				for (int k = i - 1; k >= j; k--) {
					S[k + 1] = S[k];
					vToPos[S[k+1]] = k + 1;
				}
			S[j] = t;
			vToPos[S[j]] = j;
			cost += maxgain;
			if (finished) return;
		}
	}
}

void Individual::intensify(const string &perturbationType, double ils_time, int cuttingMult, int swaps, bool reqll){
	//cout << "Inicia ILS en " << cost << endl;
	//Time structures
	if (finished) return;
	struct timeval start_bl,end_bl;
	double stop=0.0;
	gettimeofday(&start_bl,NULL);
	/*P_1 Initial intensification process*/
	if (!reqll){
		local_search(cuttingMult);
	} else {
		local_search_ll(cuttingMult);
	}
	/*Save solution after apply local search*/
	Individual newInd=internalClone();
	/*Loop principal*/
	while((stop<ils_time) && (!finished)){
		//double v = (double)(random()) / RAND_MAX;
		int selected = random() % 3;
		if (selected == 0){
			for (int i = 0; i < swaps; i++){
				int f = random() % dim;
				int s = random() % dim;
				swap(newInd.S[f], newInd.S[s]);
			}
			newInd.evaluate();
		} else if (selected == 1){
			int i1 = max((int)(random() % (dim)) - permBlock/2, 0);
			int i2 = min(dim - 1, i1 + permBlock);
			vector<long long> all;
			for (int i = i1; i <= i2; i++){
				all.push_back(newInd.S[i]);
			}
			random_shuffle(all.begin(), all.end());
			for (int i = i1; i <= i2; i++){
				newInd.S[i] = all[i-i1];
			}
			newInd.evaluate();
			//cout << "Evalua y vale: " << newInd.cost << endl;
		} else {
			int f = random() % dim;
			int s = min(f + (int)(random() % 10), (int)(dim) - 1);
			int block_size = s - f + 1;
			int new_f_pos = random() % (dim + 1 - block_size);
			//cout << "Mueve " << f << " " << s << " a " << new_f_pos << endl;
			vector<int> block;
			for (int j = f; j <= s; j++){
				block.push_back(newInd.S[j]);
			}
			if (new_f_pos > f){
				for (int i = s + 1; i <= new_f_pos + block_size - 1; i++){
					newInd.S[f + i - s - 1] = newInd.S[i];
				}
				for (int i = 0; i < block_size; i++){
					newInd.S[new_f_pos+i] = block[i];
				}
			} else {
				int ce = s;
				for (int i = f - 1; i >= new_f_pos; i--){
					newInd.S[ce] = newInd.S[i];
					ce--;
				}
				for (int i = 0; i < block_size; i++){
					newInd.S[new_f_pos+i] = block[i];
				}
			}
			newInd.evaluate();
		}
		if (!reqll){
			newInd.local_search(cuttingMult);
		} else {
			newInd.local_search_ll(cuttingMult);
		}
	
		/*P_4 Check cost in the pertutbate solution*/
		if(newInd.cost>=cost){
			S=newInd.S;
			cost=newInd.cost;
		}else{
			newInd=internalClone();
		}
		//Check ils_time
		gettimeofday(&end_bl, NULL);
		stop=(end_bl.tv_sec - start_bl.tv_sec)+(end_bl.tv_usec - start_bl.tv_usec)/1.0e6;
		if (cost > best){
			best = cost;
			//cout << "                                                     Va: " << best - crossRemoved << endl;
		}
	}
	//cout << "Iteraciones: " << count << endl;
	//cout << "Termina en " << cost - crossRemoved << endl;


	//cout << "Termina ILS en " << cost << endl;
}
