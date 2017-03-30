/*graph.c*/
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <stdbool.h>

/* Function to create an adjacency list node*/
adjlist_node_p createNode(int v) {
    adjlist_node_p newNode = (adjlist_node_p) malloc(sizeof (adjlist_node_t));
    /*
    if(!newNode)
        err_exit("Unable to allocate memory for new node");
     */
    newNode->vertex = v;
    newNode->next = NULL;
    newNode->isMine = false;
    newNode->state = 0;

    return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph_p createGraph(int n, graph_type_e type) {
    int i;
    graph_p graph = (graph_p) malloc(sizeof (graph_t));
    /*
    if(!graph)
        err_exit("Unable to allocate memory for graph");
     * */
    graph->num_vertices = n;
    graph->type = type;

    /* Create an array of adjacency lists*/
    graph->adjListArr = (adjlist_p) malloc(n * sizeof (adjlist_t));
    /*
    if(!graph->adjListArr)
        err_exit("Unable to allocate memory for adjacency list array");
     * */

    for (i = 0; i < n; i++) {

        graph->adjListArr[i].node = createNode(i);
        graph->adjListArr[i].head = NULL;
        graph->adjListArr[i].num_members = 0;

    }

    return graph;
}

/*Destroys the graph*/
void destroyGraph(graph_p graph) {
    if (graph) {
        if (graph->adjListArr) {
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->num_vertices; v++) {
                adjlist_node_p adjListPtr = graph->adjListArr[v].head;
                while (adjListPtr) {
                    adjlist_node_p tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->adjListArr);
        }
        /*Free the graph*/
        free(graph);
    }
}

/* Adds an edge to a graph*/
void addEdge(graph_t *graph, int src, int dest) {
    /* Add an edge from src to dst in the adjacency list*/
    adjlist_node_p newNode = createNode(dest);
    newNode->next = graph->adjListArr[src].head;

    graph->adjListArr[src].head = newNode;
    graph->adjListArr[src].num_members++;

    if (graph->type == UNDIRECTED) {
        /* Add an edge from dest to src also*/
        newNode = createNode(src);
        newNode->next = graph->adjListArr[dest].head;
        graph->adjListArr[dest].head = newNode;
        graph->adjListArr[dest].num_members++;
    }
}

/* Function to print the adjacency list of graph*/
void displayGraph(graph_p graph) {
    int i;
    printf("vertices: %d\n", graph->num_vertices);
    for (i = 0; i < graph->num_vertices; i++) {
        adjlist_node_p adjListPtr = graph->adjListArr[i].head;
        //adjlist_node_p adjListPtr = getNode(graph,i);
        //bool mine =  graph->adjListArr[i].head->isMine;
        printf("\n%d :", i);
        while (adjListPtr) {
            printf("%d (%d)->", adjListPtr->vertex, adjListPtr->state);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
    }

    adjlist_node_p getNode(graph_t* graph, int vertex) {
        return graph->adjListArr[vertex].head;
    }
}

adjlist_node_p getNode(graph_t* graph, int vertex) {
    /*return a pointer to the node*/
    return graph->adjListArr[vertex].node;
}


void setMine(graph_t *graph, int vertex) {
    /*putting a mine in vertex v*/
    adjlist_node_p newNode = getNode(graph, vertex);

    if (newNode) {

        newNode->isMine = true;

        mineNotify(graph, vertex);
    }
}

bool getMine(graph_t *graph, int vertex) {
    /*return true if V contains mine*/
    adjlist_node_p newNode = getNode(graph, vertex);
    return newNode->isMine;
}

void setState(graph_t *graph, int vertex, int state) {
    /* changing the state of a vertex*/
    adjlist_node_p newNode = getNode(graph, vertex);
    if (newNode) {
        newNode->state = state;
        stateNotify(graph, vertex, state); // notify adjacent cells vertex state
    }
}

int getState(graph_t *graph, int vertex) {
    adjlist_node_p newNode = getNode(graph, vertex);
    return newNode->state;
}

int getAdjacentMine(graph_t *graph, int v) {
    /*return the adjacent number of mines*/
    adjlist_node_p node = graph->adjListArr[v].head;
    int mines = 0;
    while (node) {
        if (node->isMine) {
            mines++;
        }
        node = node->next;
    }
    return mines;

}

int getAdjacentFlag(graph_t *graph, int v) {
    /*return the number of the adjacent flags*/
    adjlist_node_p node = graph->adjListArr[v].head;
    int flags = 0;
    while (node) {
        if (node->state == 3) {
            flags++;
        }
        node = node->next;
    }
    return flags;
}

__inline int getVerticesNumber(graph_t *graph) {
    /*return the number of vertices in a graph*/
    return graph->num_vertices;
}

void depthFirstSearch(graph_t *graph, int v) {
    /*perform a dfs on cell V*/
    int explored[getVerticesNumber(graph)];
    for (int i = 0; i < getVerticesNumber(graph); i++) {
        explored[i] = 0;
    }

    dfs(graph, v, explored);

}

void dfs(graph_t *graph, int v, int *explored) {
    
    explored[v] = 1; // mark v as explored
    adjlist_node_p w = graph->adjListArr[v].head;
    int members = graph->adjListArr[v].num_members;
    for (int i = 0; i < members; i++) {
        //iterate over adjacent cells
        if (w && !explored[w->vertex]) {
            //if not explered
            if (w->isMine || w->state != 0) {
                // if mine or cell not closed return 
                return;
            }
            if (w->state != 2) {
                // if not opened increment opened cells
                graph->openedCells++;
            }

            if (getAdjacentMine(graph, w->vertex) != 0) {
                // if adjacent mines > 0
                // open cell, return 
                setState(graph, w->vertex, 2);
                explored[w->vertex] = 1;

            } else {
                // empty cell, perform dfs on w-> vertex
                setState(graph, w->vertex, 2);
                dfs(graph, w->vertex, explored);
            }
        }
        if (w) {
            //get next adjacent cell
            w = w->next;
        }
    }
    return;

}
