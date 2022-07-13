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
    vector<Node*> D;
};
int main(int argc, char* argv[]){
    ifstream fin(argv[1]);
    fout.open(argv[2]);
    int vCount,cCount;
    fin>>vCount;
    fin>>cCount;
    cout<<vCount<<" "<<cCount<<" "<<endl;
    int i,j;
    Node arr[cCount];
    for(i=0;i<cCount;i++){
        fin>>arr[i].index>>arr[i].u>>arr[i].v>>arr[i].t>>arr[i].l;
    }
    cout<<"Nodes created successfully"<<endl;
    for(i=0;i<cCount;i++){
        cout<<i<<": ";
        for(j=0;j<cCount;j++){
            if((arr[i].v == arr[j].u) && (arr[i].t+arr[i].l<=arr[j].t) ){
                cout<<arr[j].index<<" ";
                arr[i].D.push_back(&arr[j]);
            }
        }
        cout<<endl;
    }
    fin.close();
    fout.close();
    return 0;
    
}