/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 
********************************************************************************************/


#include <fstream>
# include<iostream>
#include <map>
#include <algorithm>
#include "Instance.h" 

using namespace std;

extern long long crossRemoved; 
extern bool isBig;

Instance::Instance(bool load){
	if (!load) return;
	loadInstance();
	if(graph_adjList.size() > 17000){
		isBig = true;
	} else {
		computeMatrix();
	}
}

void Instance::reduce(){
	newToOrig.resize(numNodesB);
	similarInOrig.resize(numNodesB);

	map< vector < int >, int > difs; //Different types of connectivity from nodes in B and its representant
	map< vector < int >, int > countRep; //Number of nodes with a given connectivity
	vector<bool> remove(numNodesB, false);//Nodes that can be removed
	for (int i = 0; i < numNodesB; i++){
		if (difs.count(graph_adjList[i])){//Already existed
			int reps = countRep[graph_adjList[i]];
			remove[i] = true;
			int idx = difs[graph_adjList[i]];
			for (int v : graph_adjList[i]){
				graph_adjList[idx].push_back(v);
			}
			if (graph_adjList[i].size()){
				crossRemoved += ((1ll + (graph_adjList[i].size() - 1)) * (graph_adjList[i].size() - 1) / 2 * reps);
			}
			similarInOrig[idx].push_back(i);
		} else {
			difs[graph_adjList[i]] = i;
		}
		countRep[graph_adjList[i]]++;
	}
	//Reduce graph
	int c = 0;
	for (int i = 0; i < numNodesB; i++){
		if (remove[i]) continue;
		newToOrig[c] = i;
		graph_adjList[c++] = graph_adjList[i];
	}
	numNodesB = difs.size();
	graph_adjList.resize(numNodesB);
	for (int i = 0; i < graph_adjList.size(); i++){
		sort(graph_adjList[i].begin(), graph_adjList[i].end());
	}
}

/******************************************************************************/
/**
 * Load an instance from a file
 */
void Instance::loadInstance(){
	string p, ocr;
	cin >> p >> ocr >> numNodesA >> numNodesB >> numEdges;
	graph_adjList.resize(numNodesB);
	int u,v;
	for(int i=0;i<numEdges;i++){
		cin >> u >> v;
		graph_adjList[v-numNodesA-1].push_back(u);
	}
	for (int i = 0; i < numNodesB; i++){
		sort(graph_adjList[i].begin(), graph_adjList[i].end());
	}
	reduce();
}

long long Instance::pair_cost(int i, int j){
    // number of crossing between node bi and bj 
    if (i==j)return 0;
		int n = graph_adjList[i].size();
		int m = graph_adjList[j].size();
		long long local_cost = 0;
		if (n + m < 50){
	    int n = graph_adjList[i].size();
	    int m = graph_adjList[j].size();
    	int u=0,v=0;
	    while (u<n and v<m){
	        if(graph_adjList[i][u]<=graph_adjList[j][v]){
	            u++;
	        }else{
	            v++;
	            local_cost+=n-u;
	        }        
	    }
		} else if (n < m){
			int current = 0;
			for (int k = 0; k < n; k++){
				int idx = lower_bound(graph_adjList[j].begin() + current, graph_adjList[j].end(), graph_adjList[i][k]) - graph_adjList[j].begin();
				current = idx;
				local_cost += idx;
			}
		} else {
			int current = 0;
			for (int k = 0; k < m; k++){
				int idx = upper_bound(graph_adjList[i].begin() + current, graph_adjList[i].end(), graph_adjList[j][k]) - graph_adjList[i].begin();
				current = idx;
				local_cost += (n - idx);
			}
		}
    return -local_cost;
}

void Instance::computeMatrix(){
	pair_matrix.resize(numNodesB, vector<long long>(numNodesB));
	for(int i=0;i<numNodesB;i++){
		for(int j=0;j<numNodesB;j++){
 			pair_matrix[i][j]=pair_cost(i,j);
		}
	}
	vector<long long> wins;
	for (int i = 0; i < numNodesB; i++){
		for (int j = i; j < numNodesB; j++){
			wins.push_back(abs(pair_matrix[i][j] - pair_matrix[j][i]));	
		}
	}
	sort(wins.begin(), wins.end());
	medWin = max(wins[wins.size()/2], 1ll);
}
