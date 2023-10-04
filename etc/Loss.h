/*
 * Loss.h
 *
 *  Created on: 2023. 10. 3.
 *      Author: Sora Sugiyama
 */

#ifndef LOSS_H_
#define LOSS_H_

#include <vector>
#include <cmath>
#include "matrix.h"
#include "actFunc.h"
using namespace std;

double CrossEntropyErr(mat &x,vector<mat>&W,mat &b){
	mat tmp=x;
	for(mat t:W){
		tmp=Product(tmp,t);
		for(auto &u:tmp){
			for(auto &v:u)v=Sigmoid(v);
		}
	}

	double loss=0;
	for(int i=0;i<(int)b.size();i++){
		if(tmp[i][0]<0){
			cout<<"ERR\n";
			assert(false);
		}
		loss-=b[i][0]*log(tmp[i][0]);
	}
	return loss;
}

double MSE(mat &x,vector<mat>&W,mat &b){
	mat tmp=x;
	for(mat t:W){
		tmp=Product(tmp,t);
		for(auto &u:tmp){
			for(auto &v:u)v=Sigmoid(v);
		}
	}
	mat r=Subtract(b,tmp);
	r=Product(Trans(r),r);
	return r[0][0];
}

#endif /* LOSS_H_ */
