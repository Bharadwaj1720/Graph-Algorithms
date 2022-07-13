#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<queue>
#include<fstream>
 using namespace std;
int *offset,*edges,*distArray,offCount,eCount;
__global__ void updateQueue(int* d_offset,int* d_edges,int * d_distArray,int *d_count,int* d_level,int* d_offCount){
    int i;
    int tid=(blockDim.x*blockIdx.x)+threadIdx.x;
    int k=(d_offCount[0]) -1;
    if(tid<k && d_level[0]==d_distArray[tid]){
        d_count[0]++;
        printf("%d ",tid);
        int temp1=d_offset[tid];
        int temp2=d_offset[tid+1];
        for(i=temp1;i<temp2;i++){
            int b = d_edges[i];
            if(d_distArray[b]==-1){
                d_distArray[b]=d_distArray[tid]+1;
            }
        }
    }
    
}

void BFS(int s){
    int *d_offset,*d_edges,*d_distArray,*d_count, *d_offCount,*d_level;//This is for GPU
    int threads=1<<10;
    int *count=new int[1];
    int *level=new int [1];
    count[0]=1;
    level[0]=0;
    int size = sizeof(int)*(offCount-1);//This is for CPU
    cudaMalloc(&d_offset,sizeof(int)*offCount);
    cudaMalloc(&d_edges,sizeof(int)*eCount);
    cudaMalloc(&d_distArray,size);
    cudaMalloc(&d_level,sizeof(int));
    cudaMalloc(&d_count,sizeof(int));
    cudaMalloc(&d_offCount,sizeof(int));
    distArray[s]=0;
    cudaMemcpy(d_offset,offset,sizeof(int)*offCount,cudaMemcpyHostToDevice);
    cudaMemcpy(d_edges,edges,sizeof(int)*eCount,cudaMemcpyHostToDevice);
    cudaMemcpy(d_distArray,distArray,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_offCount,&offCount,sizeof(int),cudaMemcpyHostToDevice);
    int blocks = (offCount+threads-2)/(offCount-1);
    int *x=new int[1];
    x[0]=0;
    cout<<"Source as: "<<s<<": ";
    cudaMemcpy(level,x,sizeof(int),cudaMemcpyHostToDevice);
    while(count[0]!=0){
        cudaMemcpy(d_count,x,sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(d_level,level,sizeof(int),cudaMemcpyHostToDevice);
        updateQueue<<<blocks,threads>>>(d_offset,d_edges,d_distArray,d_count,d_level,d_offCount);
        cudaMemcpy(count,d_count,sizeof(int),cudaMemcpyDeviceToHost);
        cudaMemcpy(level,d_level,sizeof(int),cudaMemcpyDeviceToHost);
        level[0]++;
    }
    cudaMemcpy(distArray,d_distArray,size,cudaMemcpyDeviceToHost);
}

void reset(int n){
    int i;
    for(i=0;i<n;i++){
        distArray[i]=-1;
    }
}
int main(int argc, char* argv[]){
    int source,qCount;
    cin>>offCount;
    cin>>eCount;
    cin>>qCount;
    int temp = (int)ceil((offCount-1)/8);
    temp++;
    cout<<offCount-1<<" "<<eCount<<" "<<endl;
    offset=new int[offCount];
    edges=new int[eCount];
    distArray=new int[offCount-1];
    int i;
    for(i=0;i<offCount;i++){
        cin>>offset[i];
    }
    for(i=0;i<eCount;i++){
        cin>>edges[i];
    }
    cout<<"Graph read successfully"<<endl;
    for(i=0;i<qCount;i++){
        cin>>source;
        reset(offCount-1);
        BFS(source);
        cout<<endl;
    }
    return 0;
    
}