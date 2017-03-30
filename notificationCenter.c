#include "graph.h"
#include "judge.h"

void mineNotify(graph_t *graph, int v) {
    /*Notify the adjacent cell of a mine that the cell v contains a mine*/
    adjlist_node_p parent = graph->adjListArr[v].head;

    while (parent != NULL) {
        adjlist_node_p child = graph->adjListArr[parent->vertex].head;
        while (child != NULL && child->vertex != v) {
            child = child->next;

        }
        if (child != NULL) {
            child->isMine = true;
        }

        parent = parent->next;
    }
    return;
}

void stateNotify(graph_t *graph, int v, int state) {
    /*Notify the adjacent cells that the cell state has changed*/
    adjlist_node_p parent = graph->adjListArr[v].head;
    while (parent) {
        adjlist_node_p child = graph->adjListArr[parent->vertex].head;
        while (child->vertex != v) {
            child = child->next;
        }
        child->state = state;
        parent = parent->next;
    }
    return;
}

/*
void loseNotify(judge_t *judge){
    setLost(judge);
}*/
void emptyNotify(graph_t *graph, judge_t *judge, int v) {
    openEmpty(graph, judge, v);
}
