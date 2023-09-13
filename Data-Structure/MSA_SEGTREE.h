/*
 * MSA_SEGTREE.h
 *
 *  Created on: 2023. 8. 9.
 *      Author: Sora Sugiyama
 */

#ifndef MSA_SEGTREE_H_
#define MSA_SEGTREE_H_


#include <bits/stdc++.h>
using namespace std;

struct MSASEG{
	long long st=1;
	void sizing(int n){
		while(n>st)st<<=1;
	}
    struct Data{
        long long sum,pref,suff,ans;
        Data(){}
        Data(long long sum,long long pref,long long suff,long long ans):sum(sum),pref(pref),suff(suff),ans(ans){}
    }
    vector<Data>seg(st+2,0);
    Data g(Data a,Data b){
        Data ret;
        ret.pref=max(a.pref,a.sum+b.pref);
        ret.suff=max(a.suff+b.sum,b.suff);
        ret.sum=a.sum+b.sum;
        ret.ans=max({a.suff+b.pref,a.ans,b.ans});
        return ret;
    }
    void upd(long long w,long long pos){
        pos=pos+st-1;
        seg[pos]=Data(w,w,w,w);
        while(pos>>=1)seg[pos]=g(seg[pos<<1],seg[(pos<<1)|1]);
    }
    Data ST(int S,int E,int s,int e,int pos){
        if(S>e||s>E)return Data(0,-1e18,-1e18,-1e18);
        if(S<=s&&e<=E)return seg[pos];
        return g(ST(S,E,s,(s+e)>>1,pos<<1),ST(S,E,((s+e)>>1)+1,e,(pos<<1)|1));
    }
    long long MSA(int S,int E){
        return ST(S,E,1,st,1).ans;
    }
}SEG;


#endif /* MSA_SEGTREE_H_ */
