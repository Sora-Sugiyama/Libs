/*
 * DisjointSet.h
 *
 *  Created on: 2023. 8. 10.
 *      Author: Sora Sugiyama
 */

#ifndef DISJOINTSET_H_
#define DISJOINTSET_H_

#include <vector>

struct DS{

	vector<int>P,S;
	int Size;

	void MakeSet(int V){
		P=S=vector<int>(V+2,0);
		Size=V;
		for(int i=0;i<=V;i++){
			P[i]=i;
			S[i]=1;
		}
	}

	int UnionFind(int x){
		while(x!=P[x])x=P[x];
		return x;
	}

	void UnionMerge(int x,int y){
		while(x!=P[x])x=P[x];
		while(y!=P[y]){
			int tmp=y;
			y=P[y];
			P[tmp]=x;
		}
		if(x!=y){
			S[x]+=S[y];
			S[y]=0;
			P[y]=y;
		}
	}

	int SizeOfUnion(int x){
		while(x!=P[x])x=P[x];
		return S[x];
	}

	void Clear(){
		P.clear();S.clear();
		Size=0;
	}

};


#endif /* DISJOINTSET_H_ */
