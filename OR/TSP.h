/*
 * TSP.h
 *
 *  Created on: 2023. 11. 1.
 *      Author: Sora Sugiyama
 */

#ifndef TSP_H_
#define TSP_H_


#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>
using namespace std;

struct TSP{
public:
	bool wasInit=false,wasGenerated=false;
	unsigned len;
	string DistType;
	vector<int>path;
	vector<vector<double> >Coor;
	vector<vector<double> >distM;
	double loss;

	void Init(unsigned V,string distType,vector<vector<double> >coordi=vector<vector<double> >(),vector<vector<double> >distMat=vector<vector<double> >()){
		if(V<3U){
			cout<<"The number of vertices must be at least 3."<<endl;
			return;
		}
		DistType=distType;
		Coor=coordi;
		distM=distMat;
		len=V;
		if(DistType=="Euclidean"||DistType=="TexiCab"){
			if(coordi.empty()){
				cout<<"Coordinate of vertices are needed."<<endl;
				return;
			}
			if(coordi.size()!=len){
				cout<<"Wrong coordinate."<<endl;
				return;
			}
		}else if(DistType=="Normal"){
			if(distMat.empty()){
				cout<<"Distance matrix is needed."<<endl;
				return;
			}
			if(distMat.size()!=len&&distMat.front().size()!=len){
				cout<<"Wrong distance matrix."<<endl;
				return;
			}
		}else{
			cout<<"Wrong type name.\nThere are valid type of distance.\n";
			cout<<"*====================*\nNormal\nEuclidean\nTexiCab\n*====================*\n"<<endl;
			return;
		}
		wasInit=true;
	}

	double dist(int u,int v){
		if(DistType=="Normal")return distM[u][v];
		else if(DistType=="Euclidean"){
			double a=Coor[u][0]-Coor[v][0],b=Coor[u][1]-Coor[v][1];
			return sqrt(a*a+b*b);
		}
		else if(DistType=="TexiCab"){
			double a=Coor[u][0]-Coor[v][0],b=Coor[u][1]-Coor[v][1];
			return (a>0?a:-a)+(b>0?b:-b);
		}else{
			cout<<"Error : Invalid type of distance."<<endl;
			return 0;
		}
	}

	double LOSS(vector<int>P){
		double ret=0;
		int n=P.size();
		for(int i=0;i<n;i++){
			ret+=dist(P[i],P[(i+1)%n]);
		}
		return ret;
	}

	void CLEAR(){
		len=0;
		wasInit=false;
		wasGenerated=false;
		path.clear();
		loss=0;
		Coor.clear();
		distM.clear();
		DistType="";
	}

private:
void NN(){
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int>uid(0,len-1);
		int x=uid(gen);
		vector<bool>use(len,false);
		use[x]=true;
		path.push_back(x);
		for(unsigned i=1;i<len;i++){
			pair<double,int>nn={9e18,x};
			for(unsigned j=0;j<len;j++){
				pair<double,int>now={dist(x,j),j};
				if(nn>now&&!use[j])nn=now;
			}
			x=nn.second;
			use[x]=true;
			path.push_back(x);
		}
		loss=LOSS(path);
	}

	void Greedy(){
		vector<pair<double,pair<int,int> > >edge;
		for(unsigned i=0;i<len;i++){
			for(unsigned j=0;j<i;j++){
				edge.push_back({dist(i,j),{i,j}});
			}
		}
		sort(edge.begin(),edge.end());

		vector<vector<int> >G(len+1);
		for(auto e:edge){
			int u=e.second.first,v=e.second.second;
			if(G[u].size()>=2U||G[v].size()>=2U)continue;
			G[u].push_back(v);
			G[v].push_back(u);
		}

		vector<vector<int> >paths;
		vector<int>Path(len),Inv(len);
		vector<bool>vis(len,false);
		for(unsigned i=0;i<len;i++){
			if(vis[i])continue;
			vis[i]=true;
			vector<int>tmp={int(i)};
			int x=i;
			while(1){
				for(auto y:G[x]){
					if(vis[y])continue;
					vis[y]=true;
					Path[x]=y;
					Inv[y]=x;
					x=y;
					break;
				}
				tmp.push_back(x);
				Path[x]=i;
				Inv[i]=x;
				break;
			}
			paths.push_back(tmp);
		}

		for(unsigned i=1;i<paths.size();i++){
			auto P=paths[i];
			pair<int,int>e1,e2;
			double mx=-9e18;
			int x=paths.front().front();
			int y=x;
			while(1){
				double w=dist(y,Path[y]);
				if(w>mx){
					mx=w;
					e1={y,Path[y]};
				}
				y=Path[y];
				if(x==y)break;
			}

			mx=-9e18;
			x=P.front();
			y=x;
			while(1){
				double w=dist(y,Path[y]);
				if(w>mx){
					mx=w;
					e2={y,Path[y]};
				}
				y=Path[y];
				if(x==y)break;
			}

			if(dist(e1.first,e2.first)+dist(e1.second,e2.second)<dist(e1.first,e2.second)+dist(e1.second,e2.first)){
				Path[e1.first]=e2.first;
				x=y=e2.first;
				while(1){
					swap(Path[y],Inv[y]);
					y=Path[y];
					if(x==y)break;
				}
				Path[e2.second]=e1.second;
				Inv[e2.first]=e1.first;
				Inv[e1.second]=e2.second;
			}else{
				Path[e1.first]=e2.second;
				Path[e2.first]=e1.second;
				Inv[e1.second]=e1.first;
				Inv[e2.second]=e2.first;
			}
		}

		int x=1;
		path.push_back(x);
		while(Path[x]!=path.front()){
			x=Path[x];
			path.push_back(x);
		}

		loss=LOSS(path);
	}

	void Randomize(){
		path=vector<int>(len);
		for(unsigned i=0;i<len;i++)path[i]=i;
		mt19937 gen(time(NULL));
		shuffle(path.begin(),path.end(),gen);
		loss=LOSS(path);
	}

public:
	void Generator(string type){
		if(!wasInit){
			cout<<"Please initialize with Init() first."<<endl;
			return;
		}

		if(type=="NN")NN();
		else if(type=="Greedy")Greedy();
		else if(type=="Randomize")Randomize();
		else{
			cout<<"Wrong type name"<<endl;
			return;
		}

		wasGenerated=true;
	}

private:
	random_device rd;
	void RandomEdge2opt(){
		if(len<4U){
			cout<<"The number of vertices should be bigger than 3."<<endl;
			return;
		}
		mt19937 gen(rd());
		uniform_int_distribution<int>uid(0,len-1);
		vector<bool>vb(len);
		int a=uid(gen),b=uid(gen);
		vb[a]=vb[(a+1)%len]=true;
		while(vb[b]||vb[(b+1)%len])b=uid(gen);

		vector<int>tmp;
		int i=(b+1)%len,j=a;
		while(i!=j){
			tmp.push_back(path[i]);
			i=(i+1)%len;
		}
		tmp.push_back(path[j]);
		j=(a+1)%len,i=b;
		while(i!=j){
			tmp.push_back(path[i]);
			i=(i+len-1)%len;
		}
		tmp.push_back(path[j]);

		double nowLoss=LOSS(tmp);
		if(nowLoss<loss){
			path=tmp;
			loss=nowLoss;
		}
	}

	void edge2opt(){
		if(len<4U){
			cout<<"The number of vertices should be bigger than 3."<<endl;
			return;
		}
		pair<double,pair<int,int> >cases={9e18,{0,0}};
		for(int a=0;a<int(len);a++){
			for(int b=0;b<a-1;b++){
				pair<double,pair<int,int> >now;
				now={dist(path[a],path[b])+dist(path[(a+1)%len],path[(b+1)%len])-dist(path[a],path[(a+1)%len])-dist(path[b],path[(b+1)%len]),{a,b}};
				if(now<cases)cases=now;
			}
		}
		int a=cases.second.first,b=cases.second.second;
		vector<int>tmp;
		int i=(b+1)%len,j=a;
		while(i!=j){
			tmp.push_back(path[i]);
			i=(i+1)%len;
		}
		tmp.push_back(path[j]);
		j=(a+1)%len,i=b;
		while(i!=j){
			tmp.push_back(path[i]);
			i=(i+len-1)%len;
		}
		tmp.push_back(path[j]);
		double nowLoss=LOSS(tmp);
		if(nowLoss<loss){
			path=tmp;
			loss=nowLoss;
		}
	}

	void RandomEdge3opt(int a=-1,int b=-1,int c=-1){
		if(len<6U){
			cout<<"The number of vertices should be bigger than 6."<<endl;
			return;
		}

		mt19937 gen(rd());
		uniform_int_distribution<int>uid(0,len-1);

		if(a==-1||b==-1||c==-1){
			vector<bool>vb(len);
			a=uid(gen),b=uid(gen),c=uid(gen);
			vb[a]=vb[(a+1)%len]=true;
			while(vb[b]||vb[(b+1)%len])b=uid(gen);
			vb[b]=vb[(b+1)%len]=true;
			while(vb[c]||vb[(c+1)%len])c=uid(gen);
		}

		pair<int,int>A={path[a],path[(a+1)%len]},B={path[b],path[(b+1)%len]},C={path[c],path[(c+1)%len]};
		vector<pair<double,int> >cases={
				{dist(A.first,B.first)+dist(A.second,B.second)+dist(C.first,C.second),1},
				{dist(C.second,A.second)+dist(B.first,B.second)+dist(C.first,A.first),2},
				{dist(A.first,A.second)+dist(B.first,C.first)+dist(B.second,C.second),3},
				{dist(A.second,C.second)+dist(A.first,B.second)+dist(C.first,B.first),4},
				{dist(B.first,C.second)+dist(A.first,B.second)+dist(C.first,A.second),5},
				{dist(B.first,A.first)+dist(C.second,B.second)+dist(C.first,A.second),6}
		};
		sort(cases.begin(),cases.end());

		int Case=cases.front().second;
		vector<int>tmp;
		int i,j;
		A={a,(a+1)%len},B={b,(b+1)%len},C={c,(c+1)%len};
		switch(Case){
		case 1:
			i=B.second,j=A.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=B.first,j=A.second;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+len-1)%len;
			}
			tmp.push_back(path[j]);
			break;
		case 2:
			i=A.second,j=C.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=A.first,j=C.second;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+len-1)%len;
			}
			tmp.push_back(path[j]);
			break;
		case 3:
			i=C.second,j=B.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=C.first,j=B.second;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+len-1)%len;
			}
			tmp.push_back(path[j]);
			break;
		case 4:
			i=B.first,j=A.second;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+len-1)%len;
			}
			tmp.push_back(path[j]);
			i=C.second,j=A.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=B.second,j=C.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			break;
		case 5:
			i=A.second,j=B.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=C.second,j=A.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=B.second,j=C.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			break;
		case 6:
			i=A.second,j=B.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			i=A.first,j=C.second;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+len-1)%len;
			}
			tmp.push_back(path[j]);
			i=B.second,j=C.first;
			while(i!=j){
				tmp.push_back(path[i]);
				i=(i+1)%len;
			}
			tmp.push_back(path[j]);
			break;
		}
		double nowLoss=LOSS(tmp);
		if(nowLoss<loss){
			path=tmp;
			loss=nowLoss;
		}
	}

	void edge3opt(){
		pair<double,vector<int> >opt={9e18,{0,0,0}};
		for(int a=0;a<int(len);a++){
			for(int b=0;b<a-1;b++){
				for(int c=0;c<c-1;c++){
					pair<int,int>A={path[a],path[(a+1)%len]},B={path[b],path[(b+1)%len]},C={path[c],path[(c+1)%len]};
					vector<pair<double,int> >cases={
							{dist(A.first,B.first)+dist(A.second,B.second)+dist(C.first,C.second),1},
							{dist(C.second,A.second)+dist(B.first,B.second)+dist(C.first,A.first),2},
							{dist(A.first,A.second)+dist(B.first,C.first)+dist(B.second,C.second),3},
							{dist(A.second,C.second)+dist(A.first,B.second)+dist(C.first,B.first),4},
							{dist(B.first,C.second)+dist(A.first,B.second)+dist(C.first,A.second),5},
							{dist(B.first,A.first)+dist(C.second,B.second)+dist(C.first,A.second),6}
					};
					pair<double,vector<int> >now={9e18,{a,b,c}};
					for(auto a:cases)now.first=min(now.first,a.first);
					now.first=dist(A.first,A.second)+dist(B.first,B.second)+dist(C.first,C.second)-now.first;
					if(now<opt)opt=now;
				}
			}
		}
		RandomEdge3opt(opt.second[0],opt.second[1],opt.second[2]);
	}

	void vertex2opt(){
		mt19937 gen(rd());
		uniform_int_distribution<int>uid(0,len-1);
		int a=uid(gen),b=uid(gen);
		while(a==b)b=uid(gen);
		double pre,now;
		pre=0;
		if(int((a+len-1)%len)!=b)pre+=dist(path[(a+len-1)%len],path[a]);
		if(int((a+1)%len)!=b)pre+=dist(path[a],path[(a+1)%len]);
		if(int((b+len-1)%len)!=a)pre+=dist(path[(b+len-1)%len],path[b]);
		if(int((b+1)%len)!=a)pre+=dist(path[b],path[(b+1)%len]);
		now=dist(path[(a+len-1)%len],path[b])+dist(path[b],path[(a+1)%len]);
		now+=dist(path[(b+len-1)%len],path[a])+dist(path[a],path[(b+1)%len]);
		if(pre>now){
			swap(path[a],path[b]);
			loss-=pre-now;
		}
	}

	void vertex3opt(){
		mt19937 gen(rd());
		uniform_int_distribution<int>uid(0,len-1);
		int a=uid(gen),b=uid(gen),c=uid(gen);
		while(a==b)b=uid(gen);
		while(a==c||b==c)c=uid(gen);
		vector<int>V={path[a],path[b],path[c]};
		vector<pair<double,int> >cases;
		vector<vector<int> >caseSeq={
				{0,1,2},
				{0,2,1},
				{1,0,2},
				{1,2,0},
				{2,0,1},
				{2,1,0}
		};

		for(int i=0;i<6;i++){
			auto seq=caseSeq[i];
			double now=0;
			vector<int>tmp=path;
			tmp[a]=V[seq[0]];
			tmp[b]=V[seq[1]];
			tmp[c]=V[seq[2]];
			now=LOSS(tmp);
			if(loss>now){
				loss=now;
				path=tmp;
			}
		}
	}

	void LinKernighan(){
		mt19937 gen(rd());
		uniform_int_distribution<int>(0,len-1);
		cout<<"Lin-Kernighan is not ready."<<endl;

		return;
	}

public:
	void optimizer(string type){
		if(!wasGenerated){
			cout<<"Please generate first."<<endl;
			return;
		}
		if(type=="2-opt")edge2opt();
		else if(type=="3-opt")edge3opt();
		else if(type=="2-vertices opt")vertex2opt();
		else if(type=="3-vertices opt")vertex3opt();
		else if(type=="Lin-Kernighan")LinKernighan();
		else if(type=="2-Random edges opt")RandomEdge2opt();
		else if(type=="3-Random edges opt")RandomEdge3opt();
		else{
			cout<<"Invalid name.\nThere are valid optimizer names.\n";
			cout<<"*====================*\n";
			cout<<"2-opt\n3-opt\n2-vertices opt\n3-vertices opt\n2-Random edges opt\n3-Random edges opt\nLin-Kernighan\n";
			cout<<"*====================*\n"<<endl;
		}
	}
};


#endif /* TSP_H_ */
