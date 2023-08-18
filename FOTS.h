/*
 * FOTS.h
 *
 *  Created on: 2023. 8. 18.
 *      Author: Sora Sugiyama
 *
 *      Function Optimization using a Ternary Search
 */

#ifndef FOTS_H_
#define FOTS_H_

#include "matrix.h"
#include <iostream>
#include <cmath>

mat FOTS(mat w,mat b,mat x){
	for(int i=0;i<(int)w.size();i++){
		double le=-1e18,ri=1e18;int k=100;
		while(k--){
			double m1=(le+le+ri)/3,m2=(le+ri+ri)/3;
			w[i][0]=m1;
			mat r=Subtract(Product(x,w),b);
			double loss1=Product(r,Trans(r))[0][0];
			w[i][0]=m2;
			r=Subtract(Product(x,w),b);
			double loss2=Product(r,Trans(r))[0][0];
			if(loss1<loss2)ri=m2;else le=m1;
		}
		w[i][0]=le;
	}
	mat rr=Subtract(Product(x,w),b);
	cout<<"Loss : "<<Product(rr,Trans(rr))[0][0]<<"\n";
	return w;
}

#endif /* FOTS_H_ */
