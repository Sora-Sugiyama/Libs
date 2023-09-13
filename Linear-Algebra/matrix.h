#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

// 2023 - 05 - 26
// Written by Sora Sugiyama

#include <iostream>
#include <vector>

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

mat Product(mat A,mat B){
    if(A.back().size()!=B.size()){
        cout<<"Invalid value\n";
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

mat Sum(mat A,mat B){
    if(A.size()!=B.size()||A.back().size()!=B.back().size()){
        cout<<"Invalid value\n";
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
        cout<<"Invalid value\n";
        return A;
    }
    mat ret=A;
    int n=ret.size(),m=ret.back().size();
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)ret[i][j]-=B[i][j];
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
