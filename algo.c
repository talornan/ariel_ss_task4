#include "algo.h"
#define MAX_VALUE  99999

// A utility function to create a 
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int index,int dist)
{
    struct MinHeapNode* minHeapNode =(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = index;
    minHeapNode->dist = dist;
    return minHeapNode;
}
  
// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
  
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
  
// A standard function to heapify at given idx This function also updates 
// position of nodes when they are swapped.Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap,int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
  
    if (left < minHeap->size &&
        minHeap->array[left]->dist < 
         minHeap->array[smallest]->dist )
      smallest = left;
  
    if (right < minHeap->size &&
        minHeap->array[right]->dist <
         minHeap->array[smallest]->dist )
      smallest = right;
  
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = 
             minHeap->array[smallest];
        MinHeapNode *idxNode = 
                 minHeap->array[idx];
  
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
  
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], 
                         &minHeap->array[idx]);
  
        minHeapify(minHeap, smallest);
    }
}
  
// A utility function to check if 
// the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
  
// Standard function to extract 
// minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
  
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
  
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
  
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
  
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}
  
// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
  
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
  
    // Travel up while the complete tree is not hepified.This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < 
           minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = 
                                      (i-1)/2;
        minHeap->pos[minHeap->array[
                             (i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  
                 &minHeap->array[(i - 1) / 2]);
  
        // move to parent index
        i = (i - 1) / 2;
    }
}
  
// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return 1;
   return 0;
}
   
// The main function that calculates 
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
int dijkstra(struct Graph* graph, int src , int dest)
{   
    if(graph==NULL||src==dest) 
    return -1;
    int *dist; 
    const int V = graph->V;
    dist = (int*) malloc (V * sizeof(int));
         
    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
  
    // Initialize min heap with all 
    // vertices. dist value of all vertices 
    for (int v = 0; v < V; v++)
    {
        *(dist+v) = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, *(dist+v));
        minHeap->pos[v] = v;
    }
  
    // Make dist value of src vertex 
    // as 0 so that it is extracted first
    minHeap->array[src]->v = src;
    minHeap->array[src]->dist = *(dist+src);
    minHeap->pos[src] = src;
    *(dist+src) = 0;
    decreaseKey(minHeap, src, *(dist+src));

    // Initially size of min heap is equal to V
    minHeap->size = V;
  
    // min heap contains all nodes whose shortest distance
    // is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        
        // Store the extracted vertex number
        int u = minHeapNode->v; 
  
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values 
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
  
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && 
                      *(dist+u) != INT_MAX && 
              *(dist+u) + pCrawl->weight < *(dist+v))
            {
                *(dist+v) = *(dist+u) + pCrawl->weight;
  
                // update distance value in min heap also 
                decreaseKey(minHeap, v, *(dist+v));
            }
            pCrawl = pCrawl->next;
        }
        free(minHeapNode);
    }
    int distance = *(dist+dest);
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
    free(dist);
    if(distance == INT_MAX) return -1;
    return distance;
}

// casts char to int
int char2int(char c){
    return (int)(c - '0');
}


int TSP_cmd(struct Graph* graph,char *citis,int numOfNodes){

    int mat[numOfNodes][numOfNodes];
    for(int i=0; i<numOfNodes; i++){
        for(int j=0; j<numOfNodes; j++){
                mat[i][j] = MAX_VALUE;
        }
    }

    for(int i=0; i<numOfNodes; i++){
        for(int j=0; j<numOfNodes; j++){
            if(i==j){
                continue;
            }
            
            int src = char2int(*(citis+i));
            int dest = char2int(*(citis+j));
            struct AdjListNode* temp = graph->array[src].head;
            while(temp!=NULL){
                if(temp->dest==dest){
                    mat[i][j] = temp->weight;
                    break;
                }else{
                    temp = temp->next;
                }
            }
        }
    }
    //floyd warshall
    int mini = MAX_VALUE;
    int min = MAX_VALUE;
    for(int i=0; i<numOfNodes; i++){
        for(int j=0; j<numOfNodes; j++){
            int src = char2int(*(citis+i));
            int dest = char2int(*(citis+j));
            int dijk = dijkstra(graph , src , dest);
            if(dijk != -1){
                mat[i][j] = dijk;
                if(min > dijk){
                    min = dijk;
                    mini = i;
                }
            }
        }
    }
    // for(int i=0; i<numOfNodes; i++){
    //     for(int j=0; j<numOfNodes; j++){
    //         printf("\n%d " , mat[i][j]);
    //     }
    // }
    int visited[graph->V];
    for(int i=0; i<graph->V; i++){
        visited[i] = 0;
    }
    int start_node = char2int(*(citis+mini));
    visited[start_node] = 1; //start node
    int countVisited = 1;
    int sum=0;
    for(int i=0; i<numOfNodes; i++){
        //find min in row
        int row_min = MAX_VALUE;
        int row_min_j; 
        for(int k=0; k<numOfNodes; k++){
            int node_k = char2int(*(citis+k));
            if(mat[mini][k] < row_min && visited[node_k]==0){
                row_min = mat[mini][k];
                row_min_j = k;
            }
        }
        if(row_min == MAX_VALUE) break;
        int node_row_min_j = char2int(*(citis+row_min_j));
        visited[node_row_min_j] = 1;
        countVisited++;
        sum+=row_min;
        mini = row_min_j;
    }
    if(countVisited != numOfNodes) sum = -1; 
    free(citis);
    return sum;
}


void printGraphk(struct Graph* graph){
    if(graph==NULL){
        printf("graph is Null");
        return;
    } 
        int num_nodes=0 , num_edges=0;
        printf("\nGreatest node that ever created:  %d" , graph->V-1);//might be dead
        printf("\n nodes in graph:");
        //print all nodes in graph (only alive ones)
        for (int i=0; i<graph->V; i++)
        {
            if(graph->array[i].alive==1){
            printf(" %d" , i);
                num_nodes++;
            }
        }
        //print all edges in graph
        for (int i=0; i<graph->V; i++)
        {
            if(graph->array[i].alive==1){
                if(graph->array[i].head!=NULL){
                    printf("\n Edges from %d to:" , i);
                    struct AdjListNode* temp = graph->array[i].head;
                    while(temp!=NULL){
                        printf(" %d(w%d)" , temp->dest , temp->weight);
                        temp = temp->next;
                        num_edges++;
                    }
                }
            }
        }
        printf("\n|V| = %d" , num_nodes); //|V| = num of nodes in graph        
        printf("\n|E| = %d\n" , num_edges); //|E| = num of edges in graph
}



