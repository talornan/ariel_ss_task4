#include "Digraph.h"

// A utility function that creates a graph of V vertices
Graph* createGraph(int V)
{
    struct Graph* graph =
        (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    
    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array =
      (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
    // Initialize each adjacency list as empty by
    // making head as NULL 
    for (int i = 0; i < V; i++){
        graph->array[i].head = NULL;
        graph->array[i].alive = 1;
    }
    return graph;
}



// Adds an edge to an directed graph
void addEdge(struct Graph* graph, int src, int dest,int weight)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
  struct AdjListNode* newNode = newAdjListNode(dest,weight);
  if(graph->array[src].head == NULL)  graph->array[src].head = newNode;
  else{
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
  }
}
  
// void shortsPath_cmd( head)
// {

// }

// void TSP_cmd( head)
// {

// }