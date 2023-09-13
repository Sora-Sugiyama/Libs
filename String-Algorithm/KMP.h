/*
 * KMP.h
 *
 *  Created on: 2023. 8. 9.
 *      Author: Sora Sugiyama (291sa)
 */

#ifndef KMP_H_
#define KMP_H_


#include <string>
#include <vector>
using namespace std;

// P is search keyword, T is search target.
vector<int>KMP(string P,string T){

	vector<int>pi(P.size()+3,0);
	int l=P.size(),L=T.size();
	for(int i=1,j=0;i<l;i++){
		while(j>0&&P[i]!=P[j])j=pi[j-1];
		if(P[i]==P[j])pi[i]=++j;
	}

	vector<int>ret;
	for(int i=0,j=0;i<L;i++){
		while(j>0&&T[i]!=P[j])j=pi[j-1];
		if(T[i]==P[j])j++;
		if(j==l)ret.push_back(i-l+2);
	}
	return ret;
}



#endif /* KMP_H_ */
