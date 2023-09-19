/*
 * ReaGA.h
 *
 *  Created on: 2023. 9. 19.
 *      Author: Sora Sugiyama
 *      
 *      Real-Value Coded GA
 */

#ifndef REAGA_H_
#define REAGA_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

struct RGA{
	vector<vector<double> >Obj;
	vector<vector<vector<double> > >GenerationLog;
	double MinVal=0,MaxVal=0;
	int geneLen=0,numObjs=0;
	bool CodeHasSet=false;
	void setCode(double MN,double MX,int n,int gL){
		MinVal=MN;
		MaxVal=MX;
		numObjs=n;
		geneLen=gL;
		CodeHasSet=true;
	}

	void makeObject(){
		if(!CodeHasSet){
			cout<<"Please set the code first."<<endl;
			return;
		}

		random_device rd;
		mt19937 Rgen(rd());
		uniform_real_distribution<> rRand(MinVal,MaxVal);

		int n=numObjs;
		while(n--){
			vector<double>tmp;
			for(int i=0;i<geneLen;i++)tmp.push_back(rRand(Rgen));

			Obj.push_back(tmp);
		}

		//GenerationLog.push_back(Obj);
	}

	void nextGeneration(vector<int>objNum,int Number_of_Mutation){
		if(!CodeHasSet){
			cout<<"Please set the code first"<<endl;
			return;
		}

		random_device rd;
		mt19937 Rgen(rd());
		uniform_real_distribution<> rRand(MinVal,MaxVal);
		srand(time(NULL));

		int n=(1+objNum.size())/2;
		int m=numObjs-n;
		vector<vector<double> >objs;
		for(int x:objNum)objs.push_back(Obj[x]);

		Obj.clear();

		for(int i=0;i<n;i++)Obj.push_back(objs[i]);

		int N=objs.size();
		while(m--){
			int x=rand()%N,y=rand()%N;
			vector<double>tmp;
			for(int i=0;i<geneLen;i++){
				double t=double(rand()%11);
				double gX=objs[x][i],gy=objs[y][i];
				tmp.push_back(gX*t+gy*(10.0-t));
			}

			for(int i=0;i<Number_of_Mutation;i++){
				int pos=rand()%geneLen;
				tmp[pos]=rRand(Rgen);
			}

			Obj.push_back(tmp);
		}

		//GenerationLog.push_back(Obj);
	}

	void Clear(){
		Obj.clear();
		GenerationLog.clear();
		geneLen=0;
		numObjs=0;
		MinVal=0;
		MaxVal=0;
		CodeHasSet=false;
	}
};


#endif /* REAGA_H_ */
