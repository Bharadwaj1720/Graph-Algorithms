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
    return (vArray[bit]>>offset &1);
}
void reset(int temp){
    int i;
    for(i=0;i<temp;i++){    
        vArray[i]='\0';
    }
}
int *offset,*edges;
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
       int temp1=offset[v->index];
       int temp2=offset[v->index+1];
       for(i=temp2-1;i>=temp1;i--){
           Vertex* u=G.Adj[edges[i]];
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
    
    int offCount,eCount,source,qCount;
    fin>>offCount;
    fin>>eCount;
    finq>>qCount;
    int temp = (int)ceil((offCount-1)/8);
    temp++;
    vArray=new char[temp];
    cout<<offCount-1<<" "<<eCount<<" "<<endl;
    offset=new int[offCount];
    edges=new int[eCount];
    G.InitialiseGraph(offCount-1);
    int i;
    cout<<"Graph initialised"<<endl;
    for(i=0;i<offCount;i++){
        fin>>offset[i];
    }
    for(i=0;i<eCount;i++){
        fin>>edges[i];
    }
    
    cout<<"Graph read successfully"<<endl;
    // G.addEdge(0, 1);
    // G.addEdge(0, 2);
    // G.addEdge(1, 2);
    // G.addEdge(2, 0);
    // G.addEdge(2, 3);
    // G.addEdge(3, 3);
     int Total_Duration=0;
    for(i=0;i<qCount;i++){
        finq>>source;
        reset(temp);
        auto start = high_resolution_clock::now();
        DFS_Source(G,source);
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