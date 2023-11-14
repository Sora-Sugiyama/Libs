#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

// 2023 - 11 - 14
// Written by Sora Sugiyama

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using mat=vector<vector<double> >;
using vecd=vector<double>;

mat Trans(mat A){
    int n=A.back().size(),m=A.size();
    mat ret(n,vecd(m,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            ret[i][j]=A[j][i];
        }
    }
    return ret;
}

mat Sum(mat A,mat B){
    if(A.size()!=B.size()||A.back().size()!=B.back().size()){
        cout<<"Invalid value - Sum\n";
        return A;
    }
    mat ret=A;
    int n=ret.size(),m=ret.back().size();
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)ret[i][j]+=B[i][j];
    }
    return ret;
}

mat Subtract(mat A,mat B){
    if(A.size()!=B.size()||A.back().size()!=B.back().size()){
        cout<<"Invalid value - Subtract\n";
        return A;
    }
    mat ret=A;
    int n=ret.size(),m=ret.back().size();
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)ret[i][j]-=B[i][j];
    }
    return ret;
}

mat NaiveProduct(mat A,mat B){
    if(A.back().size()!=B.size()){
        cout<<"Invalid value - NaiveProduct\n";
        return A;
    }
    int n=A.size(),m=B.back().size(),k=A.back().size();
    mat ret=mat(n,vecd(m,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            for(int r=0;r<k;r++){
                ret[i][j]+=A[i][r]*B[r][j];
            }
        }
    }
    return ret;
}

mat Product(mat A,mat B){
	if(A.back().size()!=B.size()){
		cout<<"Invalid value - Product\n";
		return A;
	}

	if(A.size()==1U||A.back().size()==1U||B.back().size()==1U){
		return NaiveProduct(A,B);
	}
	if(A.size()<=316U&&A.back().size()<=316U&&B.back().size()==316U){
		return NaiveProduct(A,B);
	}

	int n=A.size(),m=A.back().size(),k=B.back().size();
	int N=n,K=k;
	if(n&1){
		A.push_back(vecd(m,0));
		n++;
	}
	if(m&1){
		for(auto &v:A){
			v.push_back(0);
		}
		B.push_back(vecd(k,0));
		m++;
	}
	if(k&1){
		for(auto &v:B){
			v.push_back(0);
		}
		k++;
	}
	mat p,q,r,s,P,Q,R,S;
	int tn=n>>1,tm=m>>1,tk=k>>1;
	for(int i=0;i<tn;i++){
		p.push_back(vecd(A[i].begin(),A[i].begin()+tm));
		q.push_back(vecd(A[i].begin()+tm,A[i].end()));
		r.push_back(vecd(A[i+tn].begin(),A[i+tn].begin()+tm));
		s.push_back(vecd(A[i+tn].begin()+tm,A[i+tn].end()));
	}
	for(int i=0;i<tm;i++){
		P.push_back(vecd(B[i].begin(),B[i].begin()+tk));
		R.push_back(vecd(B[i].begin()+tk,B[i].end()));
		Q.push_back(vecd(B[i+tm].begin(),B[i+tm].begin()+tk));
		S.push_back(vecd(B[i+tm].begin()+tk,B[i+tm].end()));
	}

	mat u,v,w;
	u=NaiveProduct(Subtract(r,p),Subtract(R,S));
	v=NaiveProduct(Sum(r,s),Subtract(R,P));
	w=Sum(NaiveProduct(p,P),NaiveProduct(Subtract(Sum(r,s),p),Subtract(Sum(P,S),R)));

	mat z1,z2,z3,z4;
	z1=Sum(NaiveProduct(p,P),NaiveProduct(q,Q));
	z2=Sum(Sum(w,v),NaiveProduct(Subtract(Sum(p,q),Sum(r,s)),S));
	z3=Sum(Sum(w,u),NaiveProduct(s,Subtract(Sum(Q,R),Sum(P,S))));
	z4=Sum(Sum(w,u),v);

	mat ret;
	for(int i=0;i<tn;i++){
		ret.push_back(z1[i]);
		ret.back().insert(ret.back().end(),z2[i].begin(),z2[i].begin()+K-tk);
	}
	for(int i=0;i<N-tn;i++){
		ret.push_back(z3[i]);
		ret.back().insert(ret.back().end(),z4[i].begin(),z4[i].begin()+K-tk);
	}
	return ret;
}

mat ScharProduct(double c,mat A){
    mat ret=A;
    for(int i=0;i<(int)ret.size();i++){
        for(int j=0;j<(int)ret[i].size();i++){
            ret[i][j]*=c;
        }
    }
    return ret;
}

#endif // MATRIX_H_INCLUDED
