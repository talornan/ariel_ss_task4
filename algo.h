#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "Digraph.h"

// Structure to represent a min heap node
typedef struct MinHeapNode
{
    int  v;
    int dist;
}MinHeapNode;
  
// Structure to represent a min heap
typedef struct MinHeap
{
    // Number of heap nodes present currently
    int size;     
    
    // Capacity of min heap
    int capacity;  
    
    // This is needed for decreaseKey()
    int *pos;    
    struct MinHeapNode **array;
}MinHeap;

struct MinHeapNode* newMinHeapNode(int v,int dist);
struct MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
void minHeapify(struct MinHeap* minHeap,int idx);
int isEmpty(struct MinHeap* minHeap);
struct MinHeapNode* extractMin(struct MinHeap* minHeap);
void decreaseKey(struct MinHeap* minHeap, int v, int dist);
int isInMinHeap(struct MinHeap *minHeap, int v);
void printArr(int* dist, int dest);
int dijkstra(struct Graph* graph, int src , int dest);
int TSP_cmd(Graph* graph,char *citis,int numOfNodes);
int char2int(char c);
void printGraphk(struct Graph* graph);
