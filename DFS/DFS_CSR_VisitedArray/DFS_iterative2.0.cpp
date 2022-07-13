#include<iostream>
#include<vector>
#include<fstream>
#include <chrono> 
#include<bits/stdc++.h>
#include<stack>
using namespace std::chrono;
using namespace std;
int timet=0;
ofstream fout;
struct Vertex{
    int index;
    vector<Vertex*> list;
    Vertex* parent;
    char color;
    int start;
    int end;
};
vector<int> offset;
vector<Vertex*> edges;
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
    void addEdge(int v,int u){
        Adj[v]->list.push_back(Adj[u]);
    }
    void CSR(){
    int i,j=0;
    for(i=0;i<Adj.size();i++){
        Vertex*v=Adj[i];
        offset[i]=j;
        int k=j+v->list.size();
        int s,ss;
        for(ss=0;ss<v->list.size();ss++){
            edges.push_back(v->list[ss]);
        }
        j=k;
    }
        offset[Adj.size()]=j;
    }
};
stack<Vertex*> arr;
void DFS(Graph G){
    
   while(!arr.empty()){
       Vertex* v=arr.top();
       arr.pop();
      
       if(v->color=='W'){
            v->color='B';
            //fout<<v->index<<" ";
        }

       int i;
       int temp1=offset[v->index];
       int temp2=offset[v->index+1];
       for(i=temp2-1;i>=temp1;i--){
           Vertex* u=edges[i];
           
           if(u->color!='B'){
               arr.push(u);
           }
       }
   }
}
void DFS_Source(Graph G,int s){
    int i;
    
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->color='W';
    }
   // fout<<"Source as "<<s<<": ";
   arr.push(G.Adj[s]);
    DFS(G);
    
}
int main(int argc,char *argv[] ){
    Graph G;
    // G.InitialiseGraph(4);
    // G.addEdge(0, 1);
    // G.addEdge(0, 2);
    // G.addEdge(1, 2);
    // G.addEdge(2, 0);
    // G.addEdge(2, 3);
    // G.addEdge(3, 3);
    //DFS_Source(G,2);
    // int source=2;
    ifstream fin(argv[1]);
    ifstream finq(argv[3]);
    fout.open(argv[2]);
    int vCount,eCount,qCount,source;
    fin>>vCount>>eCount;
    finq>>qCount;
    cout<<vCount<<" "<<eCount<<" "<<endl;
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    int i;
    for(i=0;i<=vCount;i++){
        offset.push_back(0);
    }
    for(i=0;i<eCount;i++){
        int u,v;
        fin>>u>>v;
        //cout<<u<<" "<<v<<endl;
        if(u<vCount && v<vCount){
            G.addEdge(u,v);
        }
    }
    cout<<"Graph read successfully"<<endl;
    G.CSR();
    cout<<"Converted to CSR"<<endl;
    auto start = high_resolution_clock::now();
    for(i=0;i<qCount;i++){
        finq>>source;
        timet=0;
        DFS_Source(G,source);
        //fout<<endl;

    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    fout <<"Avg Duration(micro): " <<duration.count()/qCount << endl;
    cout <<"Avg Duration(micro): " <<duration.count()/qCount << endl;
    //fin.close();
    
    return 0;
}