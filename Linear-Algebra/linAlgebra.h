/*
 * linAlgebra.h
 *
 *  Created on: 2023. 9. 13.
 *      Author: Sora-Sugiyama
 */

#ifndef LINALGEBRA_H_
#define LINALGEBRA_H_


#include "matrix.h"
#include <algorithm>
using namespace std;
using veci=vector<int>;
using vecll=vector<long long>;

double lfabs(double x){
	return x<0?-x:x;
}

pair<mat,veci>LUP_Decomposition(mat A){ // [1]
	if(A.size()!=A.back().size()){
		cout<<"ERROR : This matrix is non-square matrix. - from LUP_Decomposition() -"<<endl;
		return {mat(),veci()};
	}

	int n=A.size(),kk=0;
	veci pi(n);
	for(int i=0;i<n;i++)pi[i]=i;
	for(int k=0;k<n;k++){
		double p=0;
		for(int i=k;i<n;i++){
			if(lfabs(A[i][k])>p){
				p=A[i][k];
				kk=i;
				break;
			}
		}
		if(p==0){
			cout<<"ERROR : singular matrix - from LUP_Decomposition() -"<<endl;
			return {mat(),veci()};
		}
		// exchange pi[k] with pi[k']
		swap(pi[k],pi[kk]);

		for(int i=0;i<n;i++)swap(A[k][i],A[kk][i]);
		for(int i=k+1;i<n;i++){
			A[i][k]/=A[k][k];
			for(int j=k+1;j<n;j++)A[i][j]-=A[i][k]*A[k][j];
		}
	}

	// now A = L + U ; pi is array such that j=pi[i] iif P[i][j]=1
	return {A,pi};
}

// System of linear equation solver - using LUP -;
vecd SLE_Solver(mat A,vecd b){ // [1]
	if(A.size()!=A.back().size()){
		cout<<"ERROR : This matrix is non-square matrix. - from SLE_Solver() -"<<endl;
		return vecd();
	}
	if(A.size()!=b.size()){
		cout<<"ERROR : Invalid value. - from SLE_Solver() -"<<endl;
		return vecd();
	}
	int n=A.size();
	auto LUpi=LUP_Decomposition(A);
	mat L=LUpi.first,U=LUpi.first;
	veci pi=LUpi.second;

	if(L.empty()){
		cout<<"*Abnormal Exit - SLE_solver()*"<<endl;
		return vecd();
	}

	// Make lower and upper triangular matrix ; LUpi.first = L + P
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i>j)U[i][j]=0;else L[i][j]=0;
			// LUpi.first[i][j] = L[i][j] if i>j else U[i][j]
		}
	}

	vecd x(n,0.0),y(n,0.0);
	for(int i=0;i<n;i++){
		y[i]=b[pi[i]];
		for(int j=0;j<i;j++)y[i]-=L[i][j]*y[j];
	}
	for(int i=n-1;i>=0;i--){
		x[i]=y[i];
		for(int j=n;j>i;j--)x[i]-=U[i][j]*x[j];
		x[i]/=U[i][i];
	}

	return x;
}

mat Inversion(mat A){ // [1]
	if(A.size()!=A.back().size()){
		cout<<"ERROR : This matrix is non-square matrix. - Inversion() -"<<endl;
		return mat();
	}

	int n=A.size();
	auto LUpi=LUP_Decomposition(A);
	mat L=LUpi.first,U=LUpi.first;
	veci pi=LUpi.second;
	mat I(n,vecd(n,0));
	for(int i=0;i<n;i++)I[i][i]=1;

	if(L.empty()){
		cout<<"*Abnormal Exit - Inversion()*"<<endl;
		return mat();
	}

	// Make lower and upper triangular matrix ; LUpi.first = L + P
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
			if(i>j)U[i][j]=0;else L[i][j]=0;
			// LUpi.first[i][j] = L[i][j] if i>j else U[i][j]
		}
	}

	mat X(n,vecd(n,0)),Y(n,vecd(n,0));

	for(int k=0;k<n;k++){
		for(int i=0;i<n;i++){
			Y[i][k]=I[pi[i]][k];
			for(int j=0;j<i;j++)Y[i][k]-=L[i][j]*Y[j][k];
		}
		for(int i=n-1;i>=0;i--){
			X[i][k]=Y[i][k];
			for(int j=n-1;j>i;j--)X[i][k]-=U[i][j]*X[j][k];
			X[i][k]/=U[i][i];
		}
	}

	return X;
}

// dyn_inv
double dynamic_det(mat A,double detA,int k,vecd u){ // [2]
	if(A.size()!=A.back().size()){
		cout<<"ERROR : This matrix is non-square matrix. - from dynamic_det() -"<<endl;
		return 0;
	}

	int n=A.size();

	if(n==0){
		return 0;
	}else if(n==1){
		return A[0][0];
	}else if(n==2){
		return A[0][0]*A[1][1]-A[1][0]*A[0][1];
	}

	// Column update queries;
	vecd w(n,0),ei(n,0);
	mat Ainv=Inversion(A);
	for(int i=0;i<n;i++)w[i]=u[i]-A[k][i];
	ei[k]=1;

	if(Ainv.empty()){
		cout<<"*Abnormal Exit - from dynamic_det()*"<<endl;
		return 0;
	}

	return (1+Product({ei},Product(Ainv,Trans({w})))[0][0])*detA;
}

double det(mat A){ // [1], [3]

	if(A.empty())return 0;

	if(A.size()!=A.back().size()){
		cout<<"ERROR : This matrix is non-square matrix. - det() -"<<endl;
		return 0;
	}

	// In Small Cases, Leibniz formula is more efficient.
	if((int)A.size()==1)return A[0][0];
	if((int)A.size()==2)return A[0][0]*A[1][1]-A[0][1]*A[1][0];
	if((int)A.size()==3)return A[0][0]*(A[1][1]*A[2][2]-A[1][2]*A[2][1])+A[0][1]*(A[1][2]*A[2][0]-A[1][0]*A[2][2])+A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);

	int n=A.size(),kk=0;
	double detA=1;

	for(int k=0;k<n;k++){
		double p=0;
		for(int i=k;i<n;i++){
			if(lfabs(A[i][k])>p){
				p=A[i][k];
				kk=i;
				break;
			}
		}
		if(p==0)return 0;

		if(k!=kk)detA*=-1.0;
		for(int i=0;i<n;i++)swap(A[k][i],A[kk][i]);
		for(int i=k+1;i<n;i++){
			A[i][k]/=A[k][k];
			for(int j=k+1;j<n;j++)A[i][j]-=A[i][k]*A[k][j];
		}
		detA*=A[k][k];
	}

	return detA;
}


/*
 * Reference
 *
 * [1] Thomas H C et al (2009) Introduction to Algorithms, Third Edition. The MIT Press
 * [2] Vissarion F, Luis P (2016) Faster Geometric Algorithms via Dynamic Determinant Computation. Computational Geometry 54:1-16
 * [3] David C L et al (2014) Linear Algebra and Its Applications 5th Edition. Pearson
 */

#endif /* LINALGEBRA_H_ */
