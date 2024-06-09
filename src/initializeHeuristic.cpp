/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: initialization of solutions. Only the random and the score are used, the other
one (Mean) is too slow.
********************************************************************************************/

#include "Individual.h"
#include <algorithm>
#include <cfloat>

#include<ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
extern bool isBig;

void Individual::initialize_Random(){
	S.clear();
	for (int i = 0; i < dim; i++) {
		S.push_back(i);
	}
	random_shuffle(S.begin(), S.end());
}

void Individual::initialize_byScore(){
	map<int, long long> scoreByInput;
	for (int i = 0; i < dim; i++){
		for (int v : instance->graph_adjList[i]){
			scoreByInput[v]++;
		}
	}
	long long ac = 0;
	for (auto &p : scoreByInput){
		p.second += ac;
		ac = p.second; 
	}
	vector< pair<double, int> > value2Score(dim);
	for (int i = 0; i < dim; i++){
		for (int v : instance->graph_adjList[i]){
			value2Score[i].first += scoreByInput[v];	
		}
		value2Score[i].first /= instance->graph_adjList[i].size();
		value2Score[i].second = i;
	}
	sort(value2Score.begin(), value2Score.end());
	S.clear();
	for (int i = 0; i < dim; i++){
		S.push_back(value2Score[i].second);
	}
	evaluate();
}

void Individual::initialize_byMean(){
	cost = 0;
	ordered_set< pair<int, pair<int, int> > > passedEdges;
	//Initially insert all edges
	for (int i = dim - 1; i >= 0; i--){
		int c = 0;
		for (int v : instance->graph_adjList[i]){
			passedEdges.insert(make_pair(v, make_pair(i, c)));
			c++;
		}
	}
	
	set<int> pending;
	for (int i = 0; i < dim; i++){
		pending.insert(i);
	}
	//random_shuffle(pending.begin(), pending.end());
	//int block_size = 1000;
	int block_size = max(1, dim / 50);
	while(pending.size()){
		vector < pair<double, int> > allInfo;
		int c;
		for (int u : pending){
			//Temporarily remove
			c = 0;
			for (int v : instance->graph_adjList[u]){
				passedEdges.erase(make_pair(v, make_pair(u, c)));
				c++;
			}
	
			//Calculate cost
			double thisMean = 0;
			for (int v : instance->graph_adjList[u]){
				thisMean += passedEdges.order_of_key(make_pair(v, make_pair(-1, -1)));
			}
			thisMean /= instance->graph_adjList[u].size();
			allInfo.push_back(make_pair(thisMean, u));
			//Reinsert
			c = 0;
			for (int v : instance->graph_adjList[u]){
				passedEdges.insert(make_pair(v, make_pair(u, c)));
				c++;
			}
		}
		sort(allInfo.begin(), allInfo.end());
		if (allInfo.size() > block_size) allInfo.resize(block_size);
		for (auto min_v2 : allInfo){
			int min_v = min_v2.second;
			//Use best one
			S.push_back(min_v);
			//Erase edges
			c = 0;
			for (int v : instance->graph_adjList[min_v]){
				passedEdges.erase(make_pair(v, make_pair(min_v, c)));
				c++;
			}
			//Update cost
			for (int v : instance->graph_adjList[min_v]){
				cost -= passedEdges.order_of_key(make_pair(v, make_pair(-1, -1)));
			}
			pending.erase(min_v);
		}
	}
	//evaluate();
	//cout << "Al evaluar es: " << cost << endl;
}

void Individual::initialize_heuristic() {
	if (initialization == "Random"){
		initialize_Random();
	} else if (initialization == "Mean"){
		initialize_byMean();
	} else if (initialization == "Score"){
		initialize_byScore();
	} else {
		cout << "Initialization not recognized" << endl;
		exit(-1);
	}
	evaluate();
}
