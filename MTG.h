/*
 * MTG.h
 *
 *  Created on: 2023. 8. 28.
 *      Author: Sora Sugiyama (291sa
 *      
 *      Model Tree Generator
 */

#ifndef MTG_H_
#define MTG_H_


#include <vector>
#include <algorithm>
using namespace std;
struct MTG{

	struct Model{
		double Begin,End,slope,intercept;
	};
	vector<Model>models;
	vector<pair<double,int> >DP;

	void GeneratingModelTree(vector<pair<double,double> >data,int n){
		DP.clear();
		DP.resize(data.size()+2);
		DP[0]={1.0,0};

		models.clear();

		vector<Model>A;
		A.resize(data.size()+2);

		sort(data.begin(),data.end());

		double ed=data.back().first,st=data.front().first;
		double step=(ed-st)/double(n);

		int k=0,kk=1;
		for(double x=step+st;x<=ed+step;x+=step){
			while(k<(int)data.size()&&data[k].first<=x)k++;
			DP[kk]={0.0,0};
			int j=0,jj=0;
			for(double s=st;s<x;s+=step){
				while(j<(int)data.size()&&data[j].first<=s)j++;

				if(j==k)continue;

				double mx=0,my=0,sx=0,sy=0,r=0,slp=0,ipt=0;
				for(int i=j-1;i<k;i++){
					mx+=data[i].first;
					my+=data[i].second;
				}

				mx/=double(k-j+1);
				my/=double(k-j+1);

				for(int i=j-1;i<k;i++){
					sx+=(data[i].first-mx)*(data[i].first-mx);
					sy+=(data[i].second-my)*(data[i].second-my);
				}

				sx=sqrt(sx);
				sy=sqrt(sy);

				for(int i=j-1;i<k;i++){
					r+=(data[i].first-mx)*(data[i].second-my)/sx/sy;
				}

				slp=sy/sx*r;
				ipt=my-mx*slp;
				if(DP[kk].first<DP[jj].first*abs(r)){
					DP[kk].first=DP[jj].first*abs(r);
					DP[kk].second=jj;
					A[kk].slope=slp;
					A[kk].intercept=ipt;
					A[kk].Begin=s;
					A[kk].End=x;
				}

				jj++;
			}
			kk++;
		}
		kk--;
		int tmp=kk;
		models.push_back(A[kk]);
		while(DP[tmp].second!=0){
			tmp=DP[tmp].second;
			models.push_back(A[tmp]);
		}
		reverse(models.begin(),models.end());
	}
};


#endif /* MTG_H_ */
