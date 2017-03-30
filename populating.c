#include "graph.h"
#include "adj.h"
#include "judge.h"
#include <stdio.h>

void populateGraph(judge_p *judge) {
    /*Connect adjacent nodes to each other*/
    for (int i = 0; i < (getGraph(judge)->height); i++) {
        for (int j = 0; j < (getGraph(judge)->width); j++) {
            int adj[8][2] = {
                {i, j + 1},
                {i - 1, j + 1},
                {i - 1, j},
                {i - 1, j - 1},
                {i, j - 1},
                {i + 1, j - 1},
                {i + 1, j},
                {i + 1, j + 1}};
            findAdj(adj, i, j, (getGraph(judge)->width), (getGraph(judge)->height)); // get the adjacent cells
            for (int k = 0; k < 8; k++) {
                if (adj[k][0] != -1) {

                    addEdge(getGraph(judge), i * (getGraph(judge)->width) + j, adj[k][0]*(getGraph(judge)->width) + adj[k][1]);
                }
            }
        }
    }


    return;
}
