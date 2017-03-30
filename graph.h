/*graph.h*/
#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <stdbool.h>

typedef enum {UNDIRECTED=0,DIRECTED} graph_type_e;

/* Adjacency list node*/
typedef struct adjlist_node
{
    int vertex;
    /*Index to adjacency list array*/
   struct adjlist_node *next; /*Pointer to the next node*/
   
   bool isMine;/*indicating if the cell have a mine*/
   
   int state;/*indicating the state of the cell [closed,open,flag,question]*/
}adjlist_node_t, *adjlist_node_p;

/* Adjacency list */
typedef struct adjlist
{
    int num_members;
    /*number of members in the list (for future use)*/
    adjlist_node_t *head;
   /*head of the adjacency linked list*/
    adjlist_node_t *node;
} adjlist_t, *adjlist_p;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct graph
{
   graph_type_e type;
   /*Directed or undirected graph */
   int num_vertices;
   /*Number of vertices*/
   adjlist_p adjListArr;
   /*Adjacency lists' array*/
   int width;
   int height;
   int mines;
   int flags;
   int openedCells;
   int startingNode;
} graph_t, *graph_p;

/* Exit function to handle fatal errors*/
adjlist_node_p createNode(int v);

graph_p createGraph(int n, graph_type_e type);

void destroyGraph(graph_p graph);

void addEdge(graph_t *graph, int src, int dest);

void displayGraph(graph_p graph);

adjlist_node_p getNode(graph_t* graph,int vertex);

void setMine(graph_t *graph,int vertex);

bool getMine(graph_t *graph,int vertex);

void mineNotify(graph_t *graph,int v);

int getAdjacentMine(graph_t *graph,int v);

void setState(graph_t *graph,int vertex,int state);

int getState(graph_t *graph,int vertex);
void dfs(graph_t *graph,int v,int *explored);
int getAdjacentFlag(graph_t *graph,int v);
int getWidth(graph_p graph);
/*
__inline void err_exit(char* msg)
{
   printf("[Fatal Error]: %s \nExiting...\n", msg);
   exit(1);
}
 */

#endif
