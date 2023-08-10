/*
 * GeneticAlgorithm.h
 *
 *  Created on: 2023. 8. 10.
 *      Author: Sora Sugiyama
 */

#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct GA{
	vector<vector<bool> >Obj;
	vector<vector<vector<bool> > >GenerationLog;
	int geneLen=0;

	void makeObject(int n,int gL){
		geneLen=gL;

		srand(time(NULL));

		while(n--){
			vector<bool>tmp;
			for(int i=0;i<gL;i++)tmp.push_back(rand()&1);

			Obj.push_back(tmp);
		}

		GenerationLog.push_back(Obj);
	}

	void reGeneration(vector<int>objNum,int Number_of_Mutation){
		srand(time(NULL));

		int m=Obj.size();
		vector<vector<bool> >objs;
		for(int x:objNum)objs.push_back(Obj[x]);

		Obj.clear();

		int N=objs.size();
		while(m--){
			int x=rand()%N,y=rand()%N;

			vector<bool>tmp;
			for(int i=0;i<geneLen;i++){
				if(rand()&1)tmp.push_back(objs[x][i]);
				else tmp.push_back(objs[y][i]);
			}

			for(int i=0;i<Number_of_Mutation;i++){
				int pos=rand()%geneLen;
				tmp[pos]=(rand()&1);
			}

			Obj.push_back(tmp);
		}

		GenerationLog.push_back(Obj);
	}

	void Clear(){
		Obj.clear();
		GenerationLog.clear();
		geneLen=0;
	}
};


#endif /* GENETICALGORITHM_H_ */
