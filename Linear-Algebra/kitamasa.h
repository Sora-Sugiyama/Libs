/*
 * kitamasa.h
 *
 *  Created on: 2023. 10. 25.
 *      Author: Sora Sugiyama
 */

#ifndef KITAMASA_H_
#define KITAMASA_H_


#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll=long long;

struct Kitamasa{
	ll MOD=1e9+7;
	unsigned SIZE=0;
	vector<ll>a,c;

	void setting(vector<ll>A,vector<ll>C,ll Mod){
		if(A.size()!=C.size()){
			cout<<"Error 1"<<endl;
			return;
		}
		SIZE=A.size();
		a=A;c=C;
		MOD=Mod;
	}

	ll calc_n(ll n){
		if(n<(ll)SIZE)return a[n];
		vector<bool>oper;
		while(n>2*SIZE){
			if(n&1){
				oper.push_back(true);
				n^=1;
			}else{
				oper.push_back(false);
				n>>=1;
			}
		}
		while(n>SIZE){
			oper.push_back(true);
			n--;
		}
		vector<ll>C=c;
		reverse(oper.begin(),oper.end());
		vector<vector<ll> >tmp2(SIZE,vector<ll>(SIZE,0));
		vector<ll>tmp1=C;
		for(bool op:oper){
			if(op){
				tmp1[0]=C[SIZE-1]*c[0]%MOD;
				for(unsigned i=1;i<SIZE;i++)tmp1[i]=(C[i-1]+C[SIZE-1]*c[i]%MOD)%MOD;
				C=tmp1;
			}else{
				tmp2[0]=C;
				for(unsigned i=1;i<SIZE;i++){
					tmp2[i][0]=c[0]*tmp2[i-1][SIZE-1]%MOD;
					for(unsigned j=1;j<SIZE;j++)tmp2[i][j]=(tmp2[i-1][j-1]+tmp2[i-1][SIZE-1]*c[j]%MOD)%MOD;
				}
				for(unsigned i=0;i<SIZE;i++){
					C[i]=0;
					for(unsigned j=0;j<SIZE;j++)C[i]=(C[i]+tmp2[0][j]*tmp2[j][i]%MOD)%MOD;
				}
			}
		}
		ll ret=0;
		for(unsigned i=0;i<SIZE;i++)ret=(ret+a[i]*C[i]%MOD)%MOD;

		return ret;
	}
};


#endif /* KITAMASA_H_ */
