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
   // vArray[bit]=(int)vArray[bit]|(int)pow(2,offset);
    switch(offset){
        case 0: vArray[bit]=(int)vArray[bit]|1; return;
        case 1: vArray[bit]=(int)vArray[bit]|2;return;
        case 2: vArray[bit]=(int)vArray[bit]|4;return;
        case 3: vArray[bit]=(int)vArray[bit]|8;return;
        case 4: vArray[bit]=(int)vArray[bit]|16;return;
        case 5: vArray[bit]=(int)vArray[bit]|32;return;
        case 6: vArray[bit]=(int)vArray[bit]|64;return;
        case 7: vArray[bit]=(int)vArray[bit]|128;return;
    }
}
bool check(int s){
    int i;
    int bit = s/8;
    int offset = s%8;
    //return (vArray[bit]>>offset &1);
    int k=vArray[bit];
    switch(offset){
        case 0: return (k &1);
        case 1: return (k &2);
        case 2: return (k &4);
        case 3: return (k&8);
        case 4: return (k &16);
        case 5: return (k &32);
        case 6: return (k &64);
        default: return (k &128);
    }
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
    //fout<<"Source as "<<s<<": ";
    int i;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=-1;
    }
    G.Adj[s]->dist=0;
    set(s);
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
            if(check(b->index)==false){
                set(b->index);
                //fout<<b->index<<" ";
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
    int Total_Duration=0;
    for(i=0;i<qCount;i++){
        finq>>source;
        reset(temp);
        auto start = high_resolution_clock::now();
        BFS(G,source);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start); 
        Total_Duration+=duration.count();
        //fout<<endl;
    }
    
   
    fout <<"Average Duration(micro): " <<Total_Duration/qCount << endl;
    cout <<"Average Duration(micro): " <<Total_Duration/qCount << endl;
    fin.close();
    fout.close();
    return 0;
    
}