#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include <chrono> 

using namespace std::chrono;
using namespace std;
ofstream fout;
struct Vertex{
    int index;
    vector<Vertex*> list;
    Vertex* parent;
    int dist;
    char color;
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
    void addEdge(int v,int u){
        Adj[v]->list.push_back(Adj[u]);
    }
};
void BFS(Graph G,int s){
    //fout<<"Source as "<<s<<": ";
    int i;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=-1;
        G.Adj[i]->color='W';
    }
    G.Adj[s]->dist=0;
    G.Adj[s]->color='G';
    //fout<<s<<" ";
    //cout<<s<<" ";
    queue<Vertex*> Q;
    Q.push(G.Adj[s]);
    while(Q.size()!=0){
        Vertex *v = Q.front();
        Q.pop();
        int j;
        for(j=0;j<v->list.size();j++){
            Vertex *b = v->list[j];
            if(b->color=='W'){
                //fout<<b->index<<" ";
                //cout<<b->index<<" ";
                b->parent=v;
                b->color='G';
                b->dist=v->dist+1;
                Q.push(b);
            }
        }
        v->color='B';
    }
    
}

int main(int argc, char* argv[]){
    Graph G;
    // cout<<argc<<endl;
    // cout<<argv[0]<<endl;
    // cout<<argv[1]<<endl;
    // cout<<argv[2]<<endl;
    ifstream fin(argv[1]);
    ifstream finq(argv[3]);
    fout.open(argv[2]);
    //ifstream fin("input");
    
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
        //cout<<u<<" "<<v<<endl;
        if(u<vCount && v<vCount){
            G.addEdge(u,v);
        }
    }
    cout<<"Graph read successfully"<<endl;
    // G.addEdge(0, 1);
    // G.addEdge(0, 2);
    // G.addEdge(1, 2);
    // G.addEdge(2, 0);
    // G.addEdge(2, 3);
    // G.addEdge(3, 3);
    auto start = high_resolution_clock::now();
    for(i=0;i<qCount;i++){
        finq>>source;
        BFS(G,source);
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