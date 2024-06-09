/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 

Description: class to store the information of a LOP instance when long long is required.
TODO: make it a template
********************************************************************************************/

#ifndef __PROBLEMLL_H__
#define __PROBLEMLL_H__

using namespace std;

class ProblemLL {
	public:
		ProblemLL(){}
		~ProblemLL(){ }
		long long instance[17000][17000];
		long long gain[17000][17000];
};

#endif
