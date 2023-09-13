/*
 * ShortestPath.h
 *
 *  Created on: 2023. 8. 9.
 *      Author: Sora Sugiyama
 */

#ifndef SHORTESTPATH_H_
#define SHORTESTPATH_H_

#include <iostream>
#include <vector>
#include <queue>
#include <deque>

// inf = 9e18

// Dijkstra's Algorithm
vector<long long>Dijkstra(vector<vector<pair<int,long long> > >G,int dpt){
	vector<long long>dist(G.size()+3,9e18);
	priority_queue<pair<long long,int> >pq;
	pq.push({0,dpt});dist[dpt]=0;
	while(!pq.empty()){
		pair<long long,int>u=pq.top();pq.pop();
		if(-u.first>dist[u.second])continue;
		for(auto v:G[u.second]){
			if(v.second<0){
				cout<<"Invalid weight.\n";
				return vector<long long>();
			}
			if(dist[v.first]>v.second-u.first){
				dist[v.first]=v.second-u.first;
				pq.push({u.first-v.second,v.first});
			}
		}
	}
	return dist;
}

// 0-1 BFS
vector<int>zoBFS(vector<vector<pair<int,int> > >G,int dpt){
	vector<int>dist(G.size()+3,2e9);
	deque<int>dq;
	dq.push_back(dpt);dist[dpt]=0;
	while(!dq.empty()){
		int u=dq.front();dq.pop_front();
		for(auto v:G[u]){
			if(v.second!=0&&v.second!=1){
				cout<<"Invalid weight.\n";
				return vector<int>();
			}
			if(dist[v.first]>v.second+dist[u]){
				dist[v.first]=v.second+dist[u];
				if(v.second==0)dq.push_front(v.first);
				else dq.push_back(v.first);
			}
		}
	}
	return dist;
}

/*
 * Johnson's Algorithm
 * Comming Soon...
vector<long long>Johnson(vector<vector<pair<int,int> > >G,int dpt){

}
*/

// Floyd-Warshall Algorithm


vector<vector<long long> >Floyd_Warshall(vector<vector<long long> >W){
	vector<vector<long long> >D=W;
    int n=W.size()-1;
    for(int k=1;k<=n;k++){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                D[i][j]=min(D[i][j],D[i][k]+D[k][j]);
            }
        }
    }
	return D;
}

// Shortest Path Faster Algorithm (SPFA)
vector<long long>SPFA(vector<vector<pair<int,long long> > >G,int dpt){
	vector<long long>dist(G.size()+3,9e18);
	vector<int>cy(G.size()+3,0);
	vector<bool>inQ(G.size()+3,0);
	queue<int>q;q.push(dpt); inQ[dpt]=true; dist[dpt]=0; cy[dpt]=1;
	while(!q.empty()){
		int u=q.front();q.pop();
		inQ[u]=false;
		for(auto v:G[u]){
			if(dist[v.first]>v.second+dist[u]){
				dist[v.first]=v.second+dist[u];
				if(!inQ[v.first]){
					cy[v.first]++;
					if(cy[v.first]>=(int)G.size()){
						cout<<"Negative Cycle exist.\n";
						return vector<long long>();
					}
					q.push(v.first);
					inQ[v.first]=true;
				}
			}
		}
	}
	return dist;
}


#endif /* SHORTESTPATH_H_ */
