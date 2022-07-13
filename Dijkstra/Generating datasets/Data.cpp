#include<iostream>
#include <stdlib.h>
#include<fstream>
using namespace std;
ofstream fout;
int main(int argc, char* argv[]){
    ifstream fin(argv[1]);
    fout.open(argv[2]);
    int vCount,eCount;
    fin>>vCount;
    fin>>eCount;
    cout<<vCount<<" "<<eCount<<" "<<endl;
    fout<<vCount<<" "<<eCount<<" "<<endl;
    int i;
    for(i=0;i<eCount;i++){
        int u,v;
        fin>>u>>v;
        int r=rand()%100+1;
        fout<<u<<" "<<v<<" "<<r<<endl;
        
    }
    fin.close();
    fout.close();
    return 0;
    
}