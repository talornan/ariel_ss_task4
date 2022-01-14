#include <stdio.h>
#include <stdlib.h>

// A structure to represent an adjacency list node
typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
}AdjListNode;
 
// A structure to represent an adjacency list
typedef struct  AdjList
{
    struct AdjListNode *head;
    int alive;//0-dead , 1-alive
}AdjList;

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest,int weight);