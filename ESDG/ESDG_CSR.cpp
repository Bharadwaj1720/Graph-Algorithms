#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include <chrono> 
using namespace std;
ofstream fout;

struct Node{
    int index;
    int u;
    int v;
    int t;
    int l;
    int level;
    vector<Node*> D;
    vector<Node*> ID;
};
int main(int argc, char* argv[]){
    ifstream fin(argv[1]);
    fout.open(argv[2]);
    int vCount,cCount;
    fin>>vCount;
    fin>>cCount;
    cout<<vCount<<" "<<cCount<<" "<<endl;
    int i,j,TotD=0;

    Node arr[cCount];
    for(i=0;i<cCount;i++){
        fin>>arr[i].index>>arr[i].u>>arr[i].v>>arr[i].t>>arr[i].l;
    }
    cout<<"Nodes created successfully"<<endl;
    for(i=0;i<cCount;i++){
        for(j=0;j<cCount;j++){
            if((arr[i].v == arr[j].u) && (arr[i].t+arr[i].l<=arr[j].t) ){
                arr[i].D.push_back(&arr[j]);
                TotD++;
                arr[j].ID.push_back(&arr[i]); // Stores all inward vertices.
            }
        }
    }
   
    for(i=0;i<cCount;i++){
        arr[i].level=1;
    }
    for(i=0;i<cCount;i++){
        for(j=0;j<arr[i].ID.size();j++){
            int Pl=arr[i].ID[j]->level+1;
            if(Pl>arr[i].level){
                arr[i].level = Pl;
            }
        }
    }
    
    int offset[cCount+1];
    int edges[TotD];
    j=0;
    int mn=0;
    for(i=0;i<cCount;i++){
        offset[i]=j;
        int k=j+arr[i].D.size();
        int ss;
        for(ss=0;ss<arr[i].D.size();ss++){
            edges[mn]=arr[i].D[ss]->index;
            mn++;
        }
        j=k;
    }
    offset[cCount]=j;
    fout<<cCount<<" "<<TotD<<endl;
    for(i=0;i<=cCount;i++){
        fout<<offset[i]<<" ";
    }
    fout<<endl;
    for(i=0;i<TotD;i++){
        fout<<edges[i]<<" ";
    }
    fout<<endl;
    for(i=0;i<cCount;i++){
        fout<<arr[i].index<<" "<<arr[i].u<<" "<<arr[i].v<<" "<<arr[i].t<<" "<<arr[i].l<<" "<<arr[i].level<<endl;
    }

    fin.close();
    fout.close();
    return 0;
    
}