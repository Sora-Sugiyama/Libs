/*
 * bitSegtree.h
 *
 *  Created on: 2023. 9. 12.
 *      Author: Sora Sugiyama
 */

#ifndef BITSEGTREE_H_
#define BITSEGTREE_H_


#include <vector>
using namespace std;
struct bitSegtree{
	vector<unsigned long long>seg,lazy;
	int SIZE=0,stz=1,bitsetSIZE=0;

	/*
  * Variable Description ;
	* seg : segment tree
	* lazy : lazy propagation
	* stz : number of leaf bits
	* SIZE : size of seg
	* bitsetSIZE : size of array - number of considering bits -
	*/

	void Clear(){
		seg.clear();
		lazy.clear();
		SIZE=0;
		bitsetSIZE=0;
		stz=1;

		return;
	}

	void Generate(int size_of_array){
		Clear();

		bitsetSIZE=size_of_array;
		SIZE=(size_of_array>>4)+3;
		while(stz<bitsetSIZE)stz<<=1;

		seg.resize(SIZE);
		lazy.resize(SIZE);

		return;
	}

	void UpdateLeaf(int s,int e,int pos){
		if(s>e)return;
		int l=s-((s>>6)<<6),r=e-((e>>6)<<6);
		unsigned long long bit=0ULL;
		int shifter=r-l+1;
		bit = shifter==64?(bit-1):(((1ULL<<shifter)-1)<<l);
		seg[pos]^=bit;
	}

	void UpdateChildren(int s,int e,int pos,int S,int E){
		if(!lazy[pos])return;
		int ee=(s+e)>>1,ss=((s+e)>>1)+1;
		if(((ss-1)>>6)==((e-1)>>6)){
			UpdateLeaf(ss-1,e-1,(pos<<1)|1);
		}else{
			lazy[(pos<<1)|1]^=1;
			seg[(pos<<1)|1]=(e-ss+1)-seg[(pos<<1)|1];
		}
		if(((s-1)>>6)==((ee-1)>>6)){
			UpdateLeaf(s-1,ee-1,pos<<1);
		}else{
			lazy[pos<<1]^=1;
			seg[pos<<1]=(ee-s+1)-seg[pos<<1];
		}
		lazy[pos]=0;

		return;
	}

	int sum_with_segment_tree(int s,int e,int pos,int S,int E){
		if(S>e||s>E)return 0;
		if(((s-1)>>6)<((e-1)>>6))UpdateChildren(s,e,pos,S,E);
		if(((e-1)>>6)==((s-1)>>6)){

			int l=max(s,S)-1,r=min(e,E)-1;
			l-=((l>>6)<<6),r-=((r>>6)<<6);

			unsigned long long bit=0ULL;
			int shifter=r-l+1;

			bit = shifter==64?(bit-1):(((1ULL<<shifter)-1)<<l);
			unsigned long long tmp=(seg[pos]&bit);

			return __builtin_popcountll(tmp);
		}
		if(S<=s&&e<=E)return seg[pos];

		return sum_with_segment_tree(s,(s+e)>>1,pos<<1,S,E)+sum_with_segment_tree(((s+e)>>1)+1,e,(pos<<1)|1,S,E);
	}

	int Sum(int Left,int Right){
		return sum_with_segment_tree(1,stz,1,Left,Right);
	}

	void lazy_prop(int s,int e,int pos,int S,int E){
		if(S>e||s>E)return;
		if(((s-1)>>6)<((e-1)>>6))UpdateChildren(s,e,pos,S,E);
		if(((e-1)>>6)==((s-1)>>6)){
			int l=max(s,S)-1,r=min(e,E)-1;
			l-=((l>>6)<<6),r-=((r>>6)<<6);

			int tmp=__builtin_popcountll(seg[pos]);
			int shifter=r-l+1;
			unsigned long long bit=0ULL;

			bit = shifter==64?(bit-1):(((1ULL<<shifter)-1)<<l);
			seg[pos]^=bit;

			int p=pos,dw=__builtin_popcountll(seg[pos])-tmp;
			while(p>>=1)seg[p]+=dw;

			return;
		}
		if(S<=s&&e<=E){
			int p=pos,dw=(e-s+1)-(seg[pos]<<1);
			lazy[pos]^=1;
			seg[pos]=(e-s+1)-seg[pos];
			while(p>>=1)seg[p]+=dw;

			return;
		}

		lazy_prop(s,(s+e)>>1,pos<<1,S,E);
		lazy_prop(((s+e)>>1)+1,e,(pos<<1)|1,S,E);

		return;
	}

	// Update the array : change the state of bits for all [Left, Right]. ( 0 -> 1 or 1 -> 0 )
	void Update(int Left,int Right){
		lazy_prop(1,stz,1,Left,Right);
		return;
	}
};


#endif /* BITSEGTREE_H_ */
