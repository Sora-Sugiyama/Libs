/*
 * Parberry.h
 *
 *  Created on: 2023. 8. 9.
 *      Author: Sora Sugiyama (291sa)
 */

#ifndef PARBERRY_H_
#define PARBERRY_H_


#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int dx[8]={-2,-2,2,2,1,-1,1,-1},dy[8]={1,-1,1,-1,-2,-2,2,2};

vector<vector<int> >G,H;
vector<int>visit,answer;
int N=0,K=0;
vector<pair<int,int> >PP;
vector<pair<int,int> >knightTour;
int coordi2idx(int x,int y,int m){
	return (x-1)*m+y;
}
int LAST;
int CountingDeg(int x){
	int ret=0;
	for(int y:G[x])ret+=(visit[y]<=0);
	return ret;
}

int ne,me;
void dfs(int x){
	if(visit[x]==ne*me){
		LAST=x;
		return;
	}
	int mn=2e9,mx=0,nxt=0;
	for(int y:G[x]){
		if(visit[y])continue;
		int DEG=CountingDeg(y),rrr=rand()%123456;
		if(DEG<mn){
			mn=DEG;
			mx=rrr;
			nxt=y;
		}else if(DEG==mn&&mx<rrr){
			mx=rrr;
			nxt=y;
		}
	}
	if(nxt==0){
		cout<<"FAIL\n";
		return;
	}
	visit[nxt]=visit[x]+1;
	H[x+K].push_back(nxt+K);
	H[nxt+K].push_back(x+K);
	dfs(nxt);
}

pair<int,int>A,B,C,D;
int g(int x,int y){
    if(x==A.first&&y==A.second)return B.first;
    if(x==B.first&&y==B.second)return D.first;
    if(x==C.first&&y==C.second)return D.second;
    if(x==D.first&&y==D.second)return C.second;
    if(y==A.first&&x==A.second)return C.first;
    if(y==B.first&&x==B.second)return A.second;
    if(y==C.first&&x==C.second)return A.first;
    if(y==D.first&&x==D.second)return B.second;
    return y;
}

void findPath(int x){
	//cout<<PP[x].first<<" "<<PP[x].second<<"\n";
	knightTour.push_back(PP[x]);
	for(int y:H[x]){
		y=g(x,y);
		if(visit[y])continue;
		visit[y]=visit[x]+1;
		findPath(y);
	}
}

void subP(int n,int m,int nn,int mm){
	ne=n,me=m;
	G.clear();G.resize(n*m+3);
	visit=vector<int>(n*m+1,0);
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			int v=coordi2idx(i,j,m);
			PP[v+K]={i+nn,j+mm};
			for(int k=0;k<8;k++){
				int x=dx[k]+i,y=dy[k]+j;
				if(x>0&&x<=n&&y>0&&y<=m){
					int u=coordi2idx(x,y,m);
					G[v].push_back(u);
				}
			}
		}
	}
	dfs(1);
	H[LAST+K].push_back(1+K);
	H[1+K].push_back(LAST+K);
	K+=n*m;
}

vector<pair<int,int> >Parberry(int n,int x0,int y0){
	N=n;K=0;
	H.resize(N*N+1);
	if(N<5||(N&1)){
		cout<<"There is no closed tour\n";
		return vector<pair<int,int> >();
	}
	srand(time(NULL));
	PP.resize(N*N+3);
	if(N<100){
		subP(N,N,0,0);
		visit=vector<int>(N*N+3,0);visit[coordi2idx(x0,y0,N)]=1;
		findPath(coordi2idx(x0,y0,N));
		return knightTour;
	}else{
		subP(N/2-2,N/2+2,0,0);A={(N/2-2)*(N/2+2),(N/2-4)*(N/2+2)-1};B={K,K};
		subP(N/2-2,N/2-2,0,N/2+2);B={B.first+(N/2-4)*(N/2-2)+1,B.second+(N/2-3)*(N/2-2)+3};C={K,K};
		subP(N/2+2,N/2+2,N/2-2,0);C={C.first+N/2,C.second+N+4};D={K,K};
		subP(N/2+2,N/2-2,N/2-2,N/2+2);D={D.first+1,D.second+(N/2-2)*2+2};
		visit=vector<int>(N*N+3,0);visit[coordi2idx(x0,y0,N)]=1;
		findPath(coordi2idx(x0,y0,N));
	}
	return knightTour;
}

#endif /* PARBERRY_H_ */
