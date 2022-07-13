#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<queue>
#include<fstream>
#include<set>
 using namespace std;
int *offset,*edges,*distArray,offCount,eCount;
__global__ void updateQueue(int* d_offset,int* d_edges,int * d_distArray,int* d_currentQueue,int* d_nextQueue,int *currentSize,int *nextSize){
    int i;
    int tid=(blockDim.x*blockIdx.x)+threadIdx.x;
    int k=currentSize[0];
    if(tid<k){
        printf("%d ",d_currentQueue[tid]);
        int temp1=d_offset[d_currentQueue[tid]];
        int temp2=d_offset[d_currentQueue[tid]+1];
        for(i=temp1;i<temp2;i++){
            int b = d_edges[i];
            if(d_distArray[b]==-1){
                d_distArray[b]=d_distArray[d_currentQueue[tid]]+1;
                int position = atomicAdd(nextSize, 1);
				        d_nextQueue[position] = b;
            }
        }
    }
    
}

void BFS(int s){
    int *d_offset,*d_edges,*d_distArray,*d_currentQueue,*d_nextQueue,*d_currentSize,*d_nextSize;//This is for GPU
    int threads=1<<10;
    int size = sizeof(int)*(offCount-1);//This is for CPU
    int *Queue = new int[offCount-1];//This is for CPU
    int *Qsize=new int[1];//This is for CPU
    cudaMalloc(&d_offset,sizeof(int)*offCount);
    cudaMalloc(&d_edges,sizeof(int)*eCount);
    cudaMalloc(&d_distArray,size);
    cudaMalloc(&d_currentQueue,size);
    cudaMalloc(&d_nextQueue,size);
    cudaMalloc(&d_currentSize,sizeof(int));
    cudaMalloc(&d_nextSize,sizeof(int));

    distArray[s]=0;
    cudaMemcpy(d_offset,offset,sizeof(int)*offCount,cudaMemcpyHostToDevice);
    cudaMemcpy(d_edges,edges,sizeof(int)*eCount,cudaMemcpyHostToDevice);
    cudaMemcpy(d_distArray,distArray,size,cudaMemcpyHostToDevice);
    int blocks = (offCount+threads-2)/(offCount-1);
    Queue[0]=s;
    Qsize[0]=1;
    int *x=new int[1];
    x[0]=0;
    cout<<"Source as: "<<s<<": ";
    
    while(Qsize[0]>0){
        set<int> temp;
        int i;
        cudaMemcpy(d_nextSize,x,sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(d_currentQueue,Queue,size,cudaMemcpyHostToDevice);
        cudaMemcpy(d_currentSize,Qsize,sizeof(int),cudaMemcpyHostToDevice);
        updateQueue<<<blocks,threads>>>(d_offset,d_edges,d_distArray,d_currentQueue,d_nextQueue,d_currentSize,d_nextSize);
        cudaMemcpy(Qsize,d_nextSize,sizeof(int),cudaMemcpyDeviceToHost);
        cudaMemcpy(Queue,d_nextQueue,size,cudaMemcpyDeviceToHost);


        for(i=0;i<*Qsize;i++){
          temp.insert(Queue[i]);
        }
        int y=temp.size();
        i=0;
        for (auto itr : temp)
        {
          Queue[i]=itr;
          i++;
        } 
        Qsize=&y;

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