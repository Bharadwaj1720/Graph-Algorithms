#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include <chrono> 
#include<math.h>
using namespace std::chrono;
using namespace std;
ofstream fout;
char *vArray;
struct Vertex{
    int index;
    vector<Vertex*> list;
    Vertex* parent;
    int dist;
};
void set(int s){
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
void BFS(Graph G,int s){
    fout<<"Source as "<<s<<": ";
    int i;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=-1;
    }
    G.Adj[s]->dist=0;
    set(s);
    fout<<s<<" ";
    //cout<<s<<" ";
    queue<Vertex*> Q;
    Q.push(G.Adj[s]);
    while(Q.size()!=0){
        Vertex *v = Q.front();
        Q.pop();
        int j;
        for(j=0;j<v->list.size();j++){
            Vertex *b = v->list[j];
            if(check(b->index)==false){
                set(b->index);
                fout<<b->index<<" ";
                //cout<<b->index<<" ";
                b->parent=v;
                b->dist=v->dist+1;
                Q.push(b);
            }
        }
        set(v->index);
    }
    
}

int main(int argc, char* argv[]){
    Graph G;
    int i;
    ifstream fin(argv[1]);
    ifstream finq(argv[3]);
    fout.open(argv[2]);
    int vCount,eCount,source,qCount;
    fin>>vCount;
    fin>>eCount;
    finq>>qCount;
    int temp = (int)ceil(vCount/8);
    temp++;
    vArray=new char[temp];
    
    cout<<vCount<<" "<<eCount<<" "<<endl;
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    
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
        reset(temp);
        BFS(G,source);
        fout<<endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    fout <<"Average Duration(micro): " <<duration.count()/qCount << endl;
    cout <<"Average Duration(micro): " <<duration.count()/qCount << endl;
    fin.close();
    fout.close();
    return 0;
    
}