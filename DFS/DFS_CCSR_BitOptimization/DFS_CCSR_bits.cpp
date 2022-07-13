#include<iostream>
#include<vector>
#include<fstream>
#include <chrono> 
#include<bits/stdc++.h>
#include<stack>
using namespace std::chrono;
using namespace std;
int *offset,*edges,*cOffset;
char* cEdges;
int Pointer;
ofstream fout;
char *vArray;
int *power;
struct Vertex{
    int index;
    Vertex* parent;
    int start;
    int end;
};
void reset(int temp){
    int i;
    for(i=0;i<temp;i++){    
        vArray[i]='\0';
    }
}
string toBinary(int n){
    int k=1;
    while(n>=(int)pow(2,k)){
        k++;
    }
    int i;
    string S;
    for(i=0;i<k;i++){
        int temp=n>>i&1;
        if(temp==1){
            S='1'+S;
        }
        else{
            S='0'+S;
        }
    }
    return S;

}
int toDecimal(string s){
    int sum=0,i;
    for(i=0;i<s.length();i++){
        sum=sum*2+s[i]-'0';
    }
    return sum;
}
void encoding(int offCount){
    int i;
    for(i=0;i<offCount-1;i++){
        int j;
        cOffset[i]=Pointer;
        int temp1 = offset[i];
        int temp2 =offset[i+1];
        for(j=temp1;j<temp2;j++){
            int temp = edges[j];
            string S = toBinary(temp);
            int len = S.length();
            float Bits = (float)len;
            int Bytes = ceil(Bits/7);
            if(Bytes==1){
                cEdges[Pointer]=cEdges[Pointer]|toDecimal(S);
                Pointer++;
            }
            else if(Bytes==2){
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-7,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal(S.substr(0,len-7));
                Pointer++;

            }
            else if(Bytes==3){
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-7,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-14,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal(S.substr(0,len-14));
                Pointer++;

            }
            else if(Bytes==4){
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-7,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-14,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-21,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal(S.substr(0,len-21));
                Pointer++;
            }
            else{
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-7,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-14,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-21,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal('1'+S.substr(len-28,7));
                Pointer++;
                cEdges[Pointer]=cEdges[Pointer]|toDecimal(S.substr(0,len-28));
                Pointer++;
            }
        }
    }
    cOffset[offCount-1]=Pointer;

}
int cont,sum,counter,P,mul;
int decoding(){
     cont=1;
     sum=0;
     counter=0;
     mul=1;
     while(cont!=0){
        cont=cEdges[P]&128;
        sum+=(cEdges[P]&127)*mul;
        mul=mul*128;
        counter++;
        P++;
     }
     return sum;

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
};
stack<Vertex*> arr;
void DFS(Graph G){
   while(!arr.empty()){
       Vertex* v=arr.top();
       arr.pop();
       int k=v->index;
       int bit =k/8;
       int offsetBit=k%8;
       if((vArray[bit]>>offsetBit &1)==0){
            //fout<<v->index<<" ";
            vArray[bit]=(int)vArray[bit]|power[offsetBit];
        }
        int i;
        int prev=0;
        int temp1=cOffset[v->index];
        int temp2=cOffset[v->index+1];
        P=temp1;
        while(P<temp2){
            int kk=decoding();
            Vertex *u = G.Adj[kk+prev];
            prev=kk+prev;
            k=u->index;
            bit =k/8;
            offsetBit=k%8;
            if((vArray[bit]>>offsetBit &1)==0){
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
    power=new int[8];
    int i;
    int k;
    for(k=0;k<8;k++){
        power[k]=(int)pow(2,k);
    }
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
    cOffset=new int[offCount];
    P=0;
    cEdges=new char[5*eCount];
    for(i=0;i<5*eCount;i++){
        cEdges[i]='\0';
    }
    Pointer=0;
    G.InitialiseGraph(offCount-1);
   
    cout<<"Graph initialised"<<endl;
    for(i=0;i<offCount;i++){
        fin>>offset[i];
    }
    for(i=0;i<eCount;i++){
        fin>>edges[i];
    }
    
    cout<<"Graph read successfully"<<endl;
    encoding(offCount);
    cout<<"Graph successfully encoded"<<endl;
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