/*
 * actFunc.h
 *
 *  Created on: 2023. 9. 18.
 *      Author: Sora Sugiyama
 */

#ifndef ACTFUNC_H_
#define ACTFUNC_H_

#include <cmath>
#include "matrix.h"
using namespace std;

double Sigmoid(double x){
	return 1.0/(1+exp(-x));
}

double ReLU(double x){
	return x>0?x:0;
}

double LeakyReLU(double m,double x){
	return x>0?x:x*m;
}

vecd Softmax(vecd z){
	double sofsum=0.0;
	for(double x:z)sofsum+=exp(x);
	for(double &x:z)x=exp(x)/sofsum;
	return z;
}

#endif /* ACTFUNC_H_ */
