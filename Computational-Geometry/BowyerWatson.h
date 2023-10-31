/*
 * BowyerWatson.h
 *
 *  Created on: 2023. 10. 31.
 *      Author: Sora Sugiyama
 */

#ifndef BOWYERWATSON_H_
#define BOWYERWATSON_H_

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <map>
#include <cmath>
using namespace std;

double dist2d(vector<double>p,vector<double>q){
	return sqrt((p[0]-q[0])*(p[0]-q[0])+(p[1]-q[1])*(p[1]-q[1]));
}

struct BowyerWatson2d{
	vector<vector<double> >points;
	set<vector<vector<double> > >triangulation,lines;
	bool wasInit=false;

	void addPoint(vector<double>p){
		vector<vector<vector<double> > >badT;
		for(auto it=triangulation.begin();it!=triangulation.end();it++){
			auto T=*it;
			auto a=dist2d(T[0],T[1]),b=dist2d(T[1],T[2]),c=dist2d(T[0],T[2]);
			double r=a*b*c/sqrt((a+b+c)*(b+c-a)*(c+a-b)*(a+b-c));
			double x1=T[0][0],x2=T[1][0],x3=T[2][0],y1=T[0][1],y2=T[1][1],y3=T[2][1];
			vector<double>cc={
					-((x2*x2+y2*y2-x1*x1-y1*y1)*(y3-y2)-(x2*x2+y2*y2-x3*x3-y3*y3)*(y1-y2))/(2*(x1-x2)*(y3-y2)-2*(x3-x2)*(y1-y2)),
					-((y2*y2+x2*x2-y1*y1-x1*x1)*(x3-x2)-(y2*y2+x2*x2-y3*y3-x3*x3)*(x1-x2))/(2*(y1-y2)*(x3-x2)-2*(y3-y2)*(x1-x2))
			};
			if(dist2d(cc,p)<=r)badT.push_back(T);
		}
		map<vector<vector<double> >,int>use;
		for(auto T:badT){
			for(int i=0;i<3;i++){
				auto a=T[i],b=T[(i+1)%3];
				if(a>b)swap(a,b);
				use[{a,b}]++;
			}
			triangulation.erase(T);
		}

		vector<vector<vector<double> > >poly;
		for(auto T:badT){
			for(int i=0;i<3;i++){
				auto a=T[i],b=T[(i+1)%3];
				if(a>b)swap(a,b);
				vector<vector<double> >edge={a,b};
				if(use[edge]==1){
					poly.push_back(edge);
				}
			}
		}

		for(auto E:poly){
			vector<vector<double> >newT={p};
			for(auto q:E){
				auto a=q,b=p;
				if(a>b)swap(a,b);
				newT.push_back(q);
			}
			sort(newT.begin(),newT.end());
			triangulation.insert(newT);
		}
	}

	void Delaunator(){
		triangulation.clear();
		vector<vector<double> >superT;
		double r=0;
		for(auto p:points){
			r=max(r,sqrt(p[0]*p[0]+p[1]*p[1]));
		}
		r=2*r+10;

		// NOTE : 1.5/sqrt(3) ~= 0.86602540378
		superT={{-0.86602540378*r,-0.5*r},{0,r},{0.86602540378*r,-0.5*r}};
		triangulation.insert(superT);

		for(auto p:points){
			addPoint(p);
		}

		vector<vector<vector<double> > >era;
		for(auto it=triangulation.begin();it!=triangulation.end();it++){
			auto T=*it;
			bool flag=false;
			for(auto p:T){
				if(flag)break;
				for(auto q:superT){
					if(p==q){
						era.push_back(T);
						break;
					}
				}
			}
		}
		for(auto T:era)triangulation.erase(T);

		for(auto it=triangulation.begin();it!=triangulation.end();it++){
			auto T=*it;
			for(int i=0;i<3;i++){
				auto a=T[i],b=T[(i+1)%3];
				if(a>b)swap(a,b);
				lines.insert({a,b});
			}
		}
	}

	void Init(vector<vector<double> >P){
		for(auto p:P){
			if(p.size()!=2U){
				cout<<"Input points are must be 2-dimensional."<<endl;
				return;
			}
		}
		points=P;
		wasInit=true;
	}

	void CLEAR(){
		points.clear();
		triangulation.clear();
		lines.clear();
		wasInit=false;
	}
};


#endif /* BOWYERWATSON_H_ */
