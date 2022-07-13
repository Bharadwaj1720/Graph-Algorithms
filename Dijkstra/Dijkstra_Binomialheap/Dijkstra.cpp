#include <iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include <bits/stdc++.h>
#include <string>
#include <functional>
#include<fstream>
#include <chrono> 

using namespace std::chrono;
using namespace std;
ofstream fout;
int n;
struct Vertex{
    int index;
	struct Node * pnode;
    vector<Vertex*> list;
    vector<int> weight;
    Vertex* parent;
    int dist;
};
//---------------------------------Binomial Heap------------------------------
struct Node {
	Vertex *v; // contains the vertex
	int data; // contains the key
	int degree; // number of children 
	Node *p; // pointer to parent
	Node *child; // pointer to the leftmost child
	Node *sibling; // pointer to the right sibling
};
bool flag;


class BinomialHeap {
private:
	// head points to the root of the leftmost binomial tree
	Node* head=NULL;

	// initializes  the node with default values
	// all pointers are initialized to NULL
	void initializeNode(Node* node, Vertex *v, int degree) {
		node->v=v;
		node->v->pnode=node;
		node->data = v->dist;
		node->degree = degree;
		node->p = NULL;
		node->child = NULL;
		node->sibling = NULL;
	}

	// merge two binomial trees of same degree
	static void linkBinomialTrees(Node* x, Node* y) {
		// x must be parent of y
		y->p = x;
		y->sibling = x->child;
		x->child = y;
		// increase the degree of x
		x->degree += 1;
	}

public:
	BinomialHeap() {
		head = NULL;
	}

	Node* findMinimum() {
		int min = 999999;
		Node* currPtr = head;
		Node* minPtr = NULL;

		while (currPtr != NULL) {
			if (currPtr->data < min) {
				min = currPtr->data;
				minPtr = currPtr;
			}
			currPtr = currPtr->sibling;
		}

		return minPtr;

	}

	
	void insert(Vertex *v) {
		BinomialHeap h;
		Node* node = new Node;
		initializeNode(node, v, 0);
		h.setHead(node);
		merge(h);
		
	}

	void printHeap() {
		Node* currPtr = head;
		while (currPtr != NULL) {
			cout<<"B"<<currPtr->degree<<endl;
			cout<<"There are "<<pow(2, currPtr->degree)<<" nodes in this tree"<<endl;
			cout<<"The level order traversal is"<<endl;
			queue<Node*> q;
			q.push(currPtr);
			while (!q.empty()) {
				Node* p = q.front();
				q.pop();
				cout<<p->data<<" ";

				if (p->child != NULL) {
					Node* tempPtr = p->child;
					while (tempPtr != NULL) {
						q.push(tempPtr);
						tempPtr = tempPtr->sibling;
					}
				} 
			}
			currPtr = currPtr->sibling;
			cout<<endl<<endl;
		}
	}

	Node* getHead() {
		return head;
	}

	void setHead(Node* head) {
		this->head = head;
	}
	void merge(BinomialHeap h1) {
		if(head==NULL){
			head=h1.getHead();
			return;
		}
		
		Node* curr1 = getHead();
		Node* curr2 = h1.getHead();
		Node* curr3 = NULL;
		Node* temp = NULL;
		
		if (curr1!=NULL && curr2!=NULL && (curr1->degree <= curr2->degree)) {
			
			curr3 = curr1;
			curr1 = curr1->sibling;
			
		} else {
			
			curr3 = curr2;
			if(curr2!=NULL){
				curr2 = curr2->sibling;
			}
		}
		
		temp = curr3;
		
		while(curr1 != NULL && curr2 != NULL) {
			if (curr1->degree <= curr2->degree) {
				curr3->sibling = curr1;
				curr1 = curr1->sibling;
			} else {
				curr3->sibling = curr2;
				curr2 = curr2->sibling;
			}

			curr3 = curr3->sibling;
		}
		
		if (curr1 != NULL && curr3!=NULL) {
			while(curr1 != NULL) {
				curr3->sibling = curr1;
				curr1 = curr1->sibling;
				curr3 = curr3->sibling;
			}
		}

		if (curr2 != NULL && curr3!=NULL) {
			
			while(curr2 != NULL) {
				curr3->sibling = curr2;
				curr2 = curr2->sibling;
				curr3 = curr3->sibling;
			}
		}
		
		curr3 = temp;
		Node* prev = NULL;
		Node* next=NULL;
		if(curr3!=NULL){
			next = curr3->sibling;
		}
		while (next != NULL) {
			if (curr3!=NULL && ((curr3->degree != next->degree )|| (next->sibling != NULL && curr3->degree == next->sibling->degree))) {
				prev = curr3;
				curr3 = next;
			} else {
				if (curr3!=NULL && curr3->data <= next->data) {
					curr3->sibling = next->sibling;
					BinomialHeap::linkBinomialTrees(curr3, next);
				} else {
					if (prev == NULL) {
						temp = next;
					} else {
						prev->sibling = next;
					}

					BinomialHeap::linkBinomialTrees(next, curr3);
					curr3 = next;
				}
			}
			if(curr3!=NULL){
				next = curr3->sibling;
			}
		}
		//cout<<temp->v->index<<endl;
		setHead(temp);
		return;
	}

	Node* deleteMin() {
		Node* curr = head;
		Node* prevMin = NULL;
		Node* minPtr = NULL;
		Node* prevPtr = NULL;
		int min = 999999;
		while (curr != NULL) {
			if (curr->data <= min) {
				min = curr->data;
				prevMin = prevPtr;
				minPtr = curr;
			}
			prevPtr = curr;
			curr = curr->sibling;
		}
		
	
		if (minPtr!=NULL && prevMin != NULL && minPtr->sibling != NULL) {
			prevMin->sibling = minPtr->sibling;
		} else if (minPtr!=NULL && prevMin != NULL && minPtr->sibling == NULL) {
			prevMin->sibling = NULL;
		}else if(minPtr!=NULL && prevMin == NULL and minPtr->sibling != NULL) {
		        head = minPtr->sibling;
		}else if(minPtr!=NULL && prevMin == NULL and minPtr->sibling == NULL) {
		        head = NULL;
		}
		Node* childPtr=NULL;
		if(minPtr!=NULL){
			Node* childPtr = minPtr->child;
		}
		while (childPtr != NULL) {
			childPtr->p = NULL;
			childPtr = childPtr->sibling;
		}

		stack<Node*> s;
		if(minPtr!=NULL){
			childPtr = minPtr->child;
		}
		while (childPtr != NULL) {
			s.push(childPtr);
			childPtr = childPtr->sibling;
		}
		Node*temp =new Node;
		if(!s.empty()){
			curr = s.top();
			 temp = curr;
			s.pop();
		}
		else{
			curr=NULL;
			temp = curr;
		}
		while (!s.empty()) {
			curr->sibling = s.top();
			s.pop();
			curr = curr->sibling;
		}
		if(curr!=NULL)
		curr->sibling = NULL;
		BinomialHeap h;
		h.setHead(temp);
		//cout<<"-------------------------------------------------------------------------"<<endl;
		
		merge(h); 
		
		return minPtr;
	}
	void update(Node* n){
		if(n->p!=NULL){
			if(n->data<n->p->data){
				int tdata=n->data;
				Vertex *tv=n->v;
				n->data=n->p->data;
				n->v=n->p->v;
				n->p->data=tdata;
				n->p->v=tv;
				update(n->p);
			}
		}
		
	}
};

//---------------------------Dijkstra-------------------------------------------------------
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
    void addEdge(int v,int u,int w){
        Adj[v]->list.push_back(Adj[u]);
        Adj[v]->weight.push_back(w);
    }
};
void Relax(Vertex *u,Vertex* v,int w){
   
    if(v->dist>u->dist+w){
        v->dist=u->dist+w;
        v->parent=u;
    }
}
void InitialiseSS(Graph G,int s){
    int i;
    for(i=0;i<G.Adj.size();i++){
        G.Adj[i]->parent=NULL;
        G.Adj[i]->dist=20000;
    }
    G.Adj[s]->dist=0;   
}
void Dijkstra(Graph G,int s){
    int i;
    InitialiseSS(G,s);
    BinomialHeap B;
    for(i=0;i<G.Adj.size();i++){
        B.insert(G.Adj[i]);
		
    }
	flag=true;
    int count=G.Adj.size();
    while(count!=0){
        Node* n = B.deleteMin();
        count--;
        Vertex *u=NULL;
		if(n!=NULL){
			u=n->v;
		}
        int j;
		if(u!=NULL){
			for(j=0;j<u->list.size();j++){
				Vertex *v=u->list[j];
				Relax(u,v,u->weight[j]);
				v->pnode->data=v->dist;
				B.update(v->pnode);
			}
		}
    }
}
void Print(Graph G){
    int i;
    for(i=0;i<G.Adj.size();i++){
        fout<<i<<" - "<<G.Adj[i]->dist<<endl;
    }
}
//------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
    Graph G;
    ifstream fin(argv[1]);
    ifstream finq(argv[3]);
    fout.open(argv[2]);

    int vCount,eCount,source,qCount;
    fin>>vCount;
    fin>>eCount;
    finq>>qCount;
    cout<<vCount<<" "<<eCount<<" "<<endl;
    G.InitialiseGraph(vCount);
    cout<<"Graph initialised"<<endl;
    int i;
    for(i=0;i<eCount;i++){
        int u,v,wt;
        fin>>u>>v>>wt;
        if(u<vCount && v<vCount){
            G.addEdge(u,v,wt);
        }
    }
    cout<<"Graph read successfully"<<endl;
    
    auto start = high_resolution_clock::now();
    for(i=0;i<qCount;i++){
        finq>>source;
		//cout<<source<<endl;
        Dijkstra(G,source);
        //Print(G);
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