#include<iostream>
#include<vector>
#include <bits/stdc++.h>
#include<queue>
#include<fstream>
#include <chrono> 

using namespace std::chrono;
using namespace std;
ofstream fout;
struct Vertex{
    int index;
    vector<Vertex*> list;
    vector<int> weight;
    Vertex* parent;
    int dist;
};
struct Graph{
    int V;
    vector<Vertex*> Adj;
    void InitialiseGraph(int x){
        int i;
        for(i=0;i<x;i++){
            Vertex *v=new Vertex;
            v->index=i;
            Adj.push_back(v);
        }
    }
    void addEdge(int v,int u,int w){
        Adj[v]->list.push_back(Adj[u]);
        Adj[v]->weight.push_back(w);
    }
};
void Relax(Vertex *u,Vertex* v,int w){
   
    if(v->dist>u->dist+w){
        v->dist=u->dist+w;
        v->parent=u;
    }
}
void InitialiseSS(Graph G,int s){
    int i;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=20000;
    }
    G.Adj[s]->dist=0;   
}
void Dijkstra(Graph G,int s){
    int i;
    InitialiseSS(G,s);
    auto compare =[](Vertex* &x,Vertex* &y){return x->dist>y->dist;};
    priority_queue<Vertex*, vector<Vertex*>, decltype(compare)> Q(compare);
    for(i=0;i<G.Adj.size();i++){
        Q.push(G.Adj[i]);
    }
    while(Q.size()!=0){
        Vertex *u = Q.top();
        int j;
        for(j=0;j<u->list.size();j++){
            Vertex *v=u->list[j];
            Relax(u,v,u->weight[j]);
        }
        Q.pop();
    }
}
void Print(Graph G){
    int i;
    for(i=0;i<G.Adj.size();i++){
        fout<<i<<" - "<<G.Adj[i]->dist<<endl;
    }
}
// int main(){
//     // Graph G;
//     // G.InitialiseGraph(9);
//     // G.addEdge(0, 1, 4);
//     // G.addEdge(0, 7, 8);
//     // G.addEdge(1, 2, 8);
//     // G.addEdge(1, 7, 11);
//     // G.addEdge(2, 3, 7);
//     // G.addEdge(2, 8, 2);
//     // G.addEdge(2, 5, 4);
//     // G.addEdge(3, 4, 9);
//     // G.addEdge(3, 5, 14);
//     // G.addEdge(4, 5, 10);
//     // G.addEdge(5, 6, 2);
//     // G.addEdge(6, 7, 1);
//     // G.addEdge(6, 8, 6);
//     // G.addEdge(7, 8, 7);
//     // Dijkstra(G,0);
//     // Print(G);
// }
int main(int argc, char* argv[]){
    Graph G;
    ifstream fin(argv[1]);
    ifstream finq(argv[3]);
    fout.open(argv[2]);

    int vCount,eCount,source,qCount;
    fin>>vCount;
    fin>>eCount;
    finq>>qCount;
    cout<<vCount<<" "<<eCount<<" "<<endl;
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    int i;
    for(i=0;i<eCount;i++){
        int u,v;
        fin>>u>>v;
        if(u<vCount && v<vCount){
            G.addEdge(u,v,1);
        }
    }
    cout<<"Graph read successfully"<<endl;
    
    auto start = high_resolution_clock::now();
    for(i=0;i<qCount;i++){
        finq>>source;
        Dijkstra(G,source);
        //Print(G);
        //fout<<endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    fout <<"Average Duration(micro): " <<duration.count()/qCount << endl;
    cout <<"Average Duration(micro): " <<duration.count()/qCount << endl;
    fin.close();
    fout.close();
    return 0;
    
}