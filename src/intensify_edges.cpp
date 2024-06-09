/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: 
*Implementation of a stochatic hill-climber with the move operation. Initially the
whole neighborhood was traversed in O(n^2). However for Pace Challenge a
break is done if the current gain is too low (local_search())
*Implementation of iterated local search ils using three types of perturbations (intensify())
*It operates with long long only when required
*In contrast to the intensify.cpp version it operates with the edges to calculate the costs
instead of using a matrix. It it used when the instances are too long to maintain the matrix
********************************************************************************************/

#include "Individual.h"
#include "utils.h"
#include <sys/time.h>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <set>

extern volatile bool finished;
extern long long crossRemoved;
extern long long best;
void Individual::local_search_edges(double time_limit, int cuttingMult){
	//cout << "Tiempo de parada!" << time_limit << endl;
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	vector < int > vs;
	vs.reserve(dim);
	for (int i = 0; i < dim; i++) {
		vs.push_back(i);
	}
	random_shuffle(vs.begin(), vs.end());
	int vToPos[dim];
	for (int i = 0; i < S.size(); i++){
		vToPos[S[i]] = i;
	}

  bool changed = true;

	while (changed) {
		//cuttingMult++;
		long long limit = instance->medWin * (-cuttingMult);
		changed = false;
		struct timeval currentTime; 
		gettimeofday(&currentTime, NULL);
		double current = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
		if (current - initialTime > time_limit){
			return;
		}
		for (int indexv = 0; indexv < vs.size(); indexv++) {
			if (indexv % 1000 == 0){
				//cout << indexv << " de " << vs.size() << endl;
				//cout << "                                                     Va: " << cost - crossRemoved << endl;
				struct timeval currentTime; 
				gettimeofday(&currentTime, NULL);
				double current = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
				//cout << "Tiempo: " << current - initialTime << endl;
				if (current - initialTime > time_limit){
					return;
				}
			}
			long long maxgain = 0;
			//Select an value to move, and look for its best new position
			vector<int> winners;
			int value = vs[indexv];
			int i = vToPos[value];
			winners.push_back(i);
			long long gain = 0;
			maxgain = 0;
			for (int j = i - 1; j >= 0; j--) {//Move to the left
				//gain += ((instance->pair_cost(S[i], S[j]) - instance->pair_cost(S[j], S[i])));
				gain += instance->pair_gain(value, S[j]);
				if (gain < limit + maxgain ){
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
				//gain += ((instance->pair_cost(S[j], S[i]) - instance->pair_cost(S[i], S[j])));
				gain += instance->pair_gain(S[j], value);
				if (gain < limit + maxgain ){
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

			int j = winners[random()%winners.size()];
			if (maxgain > 0) {//If there is a gain, move it
				changed = true;
			}
			int t = S[i];
			if (i < j){
				for (int k = i + 1; k <= j; k++) {
					S[k - 1] = S[k];
					vToPos[S[k-1]] = k - 1;
				}
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

void Individual::ils_edges(double ils_time, int cuttingMult, int swaps) {
	struct timeval start_bl,end_bl;
	double stop=0.0;
	gettimeofday(&start_bl,NULL);
	double initTime = (double) (start_bl.tv_sec) + (double) (start_bl.tv_usec)/1.0e6;
	local_search_edges(ils_time, cuttingMult);
	Individual newInd=internalClone();
	int count=0;
	gettimeofday(&end_bl,NULL);
	double ctime = (double) (end_bl.tv_sec) + (double) (end_bl.tv_usec)/1.0e6 - initTime;
	while((ils_time > ctime) && (!finished)){
		int type = 0;
		int selected = random() % 3;
		if (selected == 0){
			for (int i = 0; i < swaps; i++){
				int f = random() % dim;
				int s = random() % dim;
				swap(newInd.S[f], newInd.S[s]);
			}
			newInd.evaluate();
		} else if (selected == 1){
			type = 1;
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
		} else {
			int f = random() % dim;
			int s = min(f + (int)(random() % 10), (int)(dim) - 1);
			int block_size = s - f + 1;
			int new_f_pos = random() % (dim + 1 - block_size);
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
		newInd.local_search_edges(ils_time - ctime, cuttingMult);
		/*P_4 Check cost in the pertutbate solution*/
		if(newInd.cost>=cost){
			S=newInd.S;
			cost=newInd.cost;
		} else{
			newInd=internalClone();
		}
		if (cost > best){
			best = cost;
			//cout << "                                                     Va: " << best - crossRemoved << endl;
		}

		gettimeofday(&end_bl,NULL);
		ctime = (double) (end_bl.tv_sec) + (double) (end_bl.tv_usec)/1.0e6 - initTime;

	}
}
