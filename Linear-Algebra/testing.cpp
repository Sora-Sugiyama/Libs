//============================================================================
// Name        : testing.cpp
// Author      : Sora Sugiyama
// Copyright   : GNU General Public License v3.0
// Description : testing Linear-Algebra headers
//============================================================================

#include <iostream>
#include "linAlgebra.h"
#include "matrix.h"
using namespace std;
int main(){

	cout<<fixed;
	cout.precision(2);

	mat A={
			{1, 1, 1},
			{1, -2, -1},
			{3, 2, 4}
	};
	vecd b={5, 0, 3};

	/*
	 * ~ Solving System of Linear Equation ~
	 * EX)
	 * x1 + x2 + x3 = 5
	 * x1 - 2*x2 - x3 = 0
	 * 3*x1 + 2*x2 + 4*x3 =3
	 *
	 */
	vecd x=SLE_Solver(A,b);
	for(int i=0;i<(int)x.size();i++){
		cout<<"x"<<i<<" = "<<x[i]<<"\n";
	}
	cout<<endl;

	/*
	 * ~ Matrix Inversion and Determinant
	 * EX) B is 4*4 matrix
	 *
	 *
	 */

	mat B={
			{2,-8,6,8},
			{3,-9,5,10},
			{-3,0,1,-2},
			{1,-4,0,6}
	};

	mat Binv=Inversion(B);
	cout<<"Inversion of B :\n";
	for(auto u:Binv){
		for(auto v:u)cout<<v<<" ";
		cout<<"\n";
	}
	cout<<"\n";
	cout<<"detB = "<<det(B);

	cout<<flush;
	return 0;
}
