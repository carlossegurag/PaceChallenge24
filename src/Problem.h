/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: class to store the information of a LOP instance.
TODO: unify with the Instance class. They are basically the same but came from different
projects and could not unify it for the PaceChallenge.
********************************************************************************************/

#ifndef __PROBLEM_H__
#define __PROBLEM_H__

#define LOPMatrix vector<vector<long long>>
using namespace std;

class Problem {
	public:
		Problem(){}
		~Problem(){ }
		int instance[17000][17000];
		int gain[17000][17000];
};

#endif
