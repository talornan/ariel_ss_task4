#include <stdio.h>
#include <stdlib.h>
#include "algo.h"
#include <ctype.h> //isDigit function

void run_over_input(char* input);
void A_init_graph(char* input);
void B_command(char* input);
void removeSrcEdges(int node_id);
void removeDestEdges(int node_id);
void D_command(char* input);
void S_command(char* input);
void T_command(char* input);
void destroyGraph();
Graph* graph = NULL;


//main
int main()
{
    //read&write input into string
    int i=0;
    char ch;
    char *input = calloc(i, sizeof(char));
    for (i = 0; (ch=getchar())!='\n'&&ch!=EOF; i++)
    {
        input = realloc(input, ((i + 1) * sizeof(char)));
        *(input + i) = ch;
    }
    input = realloc(input, ((i + 2) * sizeof(char)));
    *(input + i) = '~';
    *(input + i + 1) = '\0';
    
    run_over_input(input);
    free(input);
    destroyGraph();
}

// runs over input and send commands.
void run_over_input(char* input) {
    while (*input!='\0'&&*input!='~')
    {
        char command = *input;
        input += 2;
        switch (command)
        {
        case 'A':
            A_init_graph(input);
            break;

        case 'B':
            B_command(input);
            break;
        
        case 'D':
            D_command(input);
            break;

        case 'S':
            S_command(input);
            break;

        case 'T':
            T_command(input);
            break;
        }
    }
}


// A command - init graph
void A_init_graph(char* input) {
    int numberOfNodes = char2int(*input);
    if(graph == NULL) graph = createGraph(numberOfNodes);
    else{
       destroyGraph(graph);
       graph = createGraph(numberOfNodes);
    }
    input+=2;
    int src,dest,weight;
    while(*input == 'n'){
        input+=2;//go to src
        src = char2int(*input);
        input+=2;//go to the first dest
        while(isdigit(*input)){
            dest = char2int(*input);
            input+=2;//go to weight
            weight = char2int(*input);
            addEdge(graph , src , dest , weight);
            input+=2;//go to next dest or new 'n'
        }
    }
    // printGraphk();
}


// B command - addes 1 node and dest edges 
// if node already exists, deletes all dest edges before adding the new ones.
void B_command(char* input) {
    int node_id = char2int(*input); 
    if(graph->V > node_id){
        //if node exist already - delete all his dest edges
        //if node was existed (might be dead/deleted) , make him alive for useage.
        if(graph->array[node_id].alive==1){
            removeSrcEdges(node_id);
        }else{
            graph->array[node_id].alive=1;
            graph->array[node_id].head=NULL;
        }
    }//node does not exist , realloc the graph size and add the node
    else{
        int last_V = graph->V;
        graph->V = node_id+1; //update graph size
        graph->array = (struct AdjList*) realloc(graph->array , graph->V * sizeof(struct AdjList));
        //init the node 
        graph->array[node_id].head=NULL;
        graph->array[node_id].alive=1;
        //if node_id>last_v so there are nodes in between that added because the realloc fun
        //those nodes should be dead
        if(node_id>last_V){
            for (int i = last_V; i < node_id; i++){
                graph->array[i].alive=0;
                graph->array[i].head=NULL;
            }
        }
    }
    int dest,weight;
    input+=2;//go to dest
    while (isdigit(*input)){
        dest = char2int(*input);
        input+=2;//go top weight
        weight = char2int(*input);
        addEdge(graph , node_id , dest , weight);
        input+=2;//after weight
    }
}
//remove all Edges from node_id to other nodes , edge.src=node_id
void removeSrcEdges(int node_id){
if(graph->array[node_id].head!=NULL){
    struct AdjListNode* current = graph->array[node_id].head;
        while(current !=NULL){
           struct AdjListNode* temp = current;
            current = current->next;
            temp->next = NULL;
        free(temp);
        }
        graph->array[node_id].head=NULL;
    }   
}

//D command - delete node and all his edges
void D_command(char* input) {
    int node_id = char2int(*input);
    if(graph->array[node_id].alive==0) return;
    removeSrcEdges(node_id);
    removeDestEdges(node_id);
    //remove node only
    graph->array[node_id].alive=0;
    graph->array[node_id].head=NULL;
}
//S command - diajkstra
void S_command(char* input){
    int src=-1 , dest=-1;
    if(isdigit(*input)){
        src = char2int(*input);
        input+=2; //go to dest
        if(isdigit(*input)){
            dest = char2int(*input);
        }
    }
    if(src==-1||dest==-1) return;
    int distance = dijkstra(graph,src,dest);
    printf("Dijsktra shortest path: %d\n" , distance);
}
void T_command(char* input){ 
    int i=0;
    char ch = *input;
    char *list = calloc(i, sizeof(char));
    int exist[graph->V];
    for(int i=0; i<graph->V; i++) exist[i] = 0; //init all exist to 0
    while(isdigit(ch) && ch!='~' && ch!='\0'){
        int node = char2int(ch);
        if(exist[node]==0 && graph->array[node].alive==1){
        list = realloc(list, ((i + 1) * sizeof(char)));
        list[i] = ch;
        i+=1;
        exist[node] = 1;
        }
        input+=2;
        ch = *input;
    }
    list = realloc(list, ((i + 1) * sizeof(char)));
    *(list + i) = '\0';
    int answer = TSP_cmd(graph , list , i);
    printf("TSP shortest path: %d\n" , answer);
}


//remove all Edges from other nodes to node_id , edge.dest=node_id
void removeDestEdges(int node_id){
    for(int i=0; i<graph->V; i++){
        if(graph->array[i].alive==1){
            if(graph->array[i].head!=NULL){//check if node_id is one of i's adj
            struct AdjListNode* temp = graph->array[i].head;
            struct AdjListNode* prev = temp;
                if(temp->dest==node_id){//if node_id is the first adj
                    graph->array[i].head=temp->next;
                    free(temp);
                    continue;
                }else{//if node_id is not the first adj
                    while(temp->next!=NULL){
                        prev = temp;
                        temp = temp->next;
                        if(temp->dest==node_id){
                        prev->next=temp->next;
                        free(temp);
                        break;
                    }
                }  
                }
            }
        }
      
    }
}



void destroyGraph(){
//delete all edges from graph
for (int i=0; i<graph->V; i++){
    if(graph->array[i].alive==0) continue;
    if(graph->array[i].head==NULL){
        continue;
    }else{
       struct AdjListNode* current = graph->array[i].head;
        while(current !=NULL){
            struct AdjListNode* temp = current;
            current = current->next;
            temp->next = NULL;
        free(temp);
        }
    }
}
//delete all nodes from graph
    for (int i=0; i<graph->V; i++){
        graph->array[i].head=NULL;
    }
    free(graph->array);
    free(graph);
}

