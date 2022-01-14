#include "edge.h"
#include <assert.h>

typedef struct Graph
{
    int V;
    struct AdjList* array;
} Graph;

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest,int weight);
