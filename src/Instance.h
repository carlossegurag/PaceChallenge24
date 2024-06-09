/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: Load instances of the crossing Minimization Problem and store it. Moreover
it is able to generate the costs in a similar way than in the Linear Ordering Problem.
It implements a way to reduce the graph.
TODO: This class and the class Problem should be unified, but they came from different
projects.
********************************************************************************************/


#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include <bits/stdc++.h>

using namespace std;

class Instance{  
	public:

 		Instance(bool);
        
		//variables
		int numNodesA;
		int numNodesB;
		int numEdges;
		vector<int> newToOrig;
		vector < vector < int > > similarInOrig;
		vector<vector <int>> graph_adjList;
		vector < vector < long long > > pair_matrix;
        
		//methods
		void loadInstance();
		long long pair_cost(int i, int j);
		void computeMatrix();
		void reduce();
		long long medWin;
		__attribute__((always_inline)) inline long long pair_gain(int i, int j){
			int n = graph_adjList[i].size();
			int m = graph_adjList[j].size();
			if (n + m >= 50){
				return pair_cost(i, j) - pair_cost(j, i);
			} else {
				int u=0,v=0;
				long long local_cost = 0;
				while (u<n and v<m){
					if(graph_adjList[i][u]<graph_adjList[j][v]){
						u++;
					}else{
						int init_u = u;
						while((u<n) && (graph_adjList[i][u] == graph_adjList[j][v])){
							u++;
						}
						v++;
						local_cost+=(u-n);
						local_cost+=init_u;
					}        
				}
				if (u == n){
					local_cost += (long long)(n) * (m - v);
				} 
				return local_cost;
			}
	}

};

#endif

