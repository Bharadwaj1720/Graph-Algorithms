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
char *vArray;
struct Vertex{
    int index;
    vector<Vertex*> list;
    Vertex* parent;
    int start;
    int end;
};
void setBit(int s){
    int bit = s/8;
    int offset = s%8;
     vArray[bit]=(int)vArray[bit]|(int)pow(2,offset);
}
bool check(int s){
    int i;
    int bit = s/8;
    int offset = s%8;
    return (vArray[bit]>>offset &1);
}
void reset(int temp){
    int i;
    for(i=0;i<temp;i++){    
        vArray[i]='\0';
    }
}
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
};
stack<Vertex*> arr;
void DFS(Graph G){
   while(!arr.empty()){
       Vertex* v=arr.top();
       arr.pop();
       if(check(v->index)!=true){
            //fout<<v->index<<" ";
            setBit(v->index);
        }
       int i;
       for(i=v->list.size()-1;i>=0;i--){
           Vertex* u=v->list[i];
           if(check(u->index)!=true){
               arr.push(u);
           }
       }
   }
}
void DFS_Source(Graph G,int s){
    int i;
    
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
    }
    //fout<<"Source as "<<s<<": ";
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
    int temp = (int)ceil(vCount/8);
    temp++;
    vArray=new char[temp];

    cout<<vCount<<" "<<eCount<<" "<<endl;
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    int i;
    for(i=0;i<eCount;i++){
        int u,v;
        fin>>u>>v;
        //cout<<u<<" "<<v<<endl;
        if(u<vCount && v<vCount){
            G.addEdge(u,v);
        }
    }
    cout<<"Graph read successfully"<<endl;
    auto start = high_resolution_clock::now();
    for(i=0;i<qCount;i++){
        finq>>source;
        timet=0;
        reset(temp);
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