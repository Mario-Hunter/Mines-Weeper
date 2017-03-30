#include <stdio.h>
#include "graph.h"
#include "judge.h"

void printGrid(judge_t *judge) {
    system("cls"); // clear screen
    printf("\n   ");
    for (int j = 1; j <= getGraph(judge)->width; j++) {
        printf("|%3d", j);
    }
    printf("\n");
    for (int j = 1; j <= getGraph(judge)->width + 1; j++) {
        printf("---|");
    }
    printf("\n");
    for (int i = 0; i < getGraph(judge)->height; i++) {
        printf("%2c%c", (char) (i / 26) + 'a', (char) (i + 97) - 26 * (i / 26));
        for (int j = 0; j < getGraph(judge)->width; j++) {
            int v = i * getGraph(judge)->width + j;
            int num;
            switch (judge->gameState) {
                case 1:
                    /*Game is running*/
                    switch (getState(getGraph(judge), v)) {
                        case 0:
                            //closed cell
                            printf("|  X");
                            break;

                        case 2:
                            //open cell
                            num = getAdjacentMine(getGraph(judge), v);
                            if (num == 0) {
                                printf("|   ");
                            } else {
                                printf("|%3d", num);
                            }
                            break;
                        case 3:
                            //flag cell
                            printf("|  F");
                            break;
                        case 4:
                            //question cell
                            printf("|  ?");
                            break;
                    }
                    break;
                case 2:
                    /*Win state*/
                    switch (getState(getGraph(judge), v)) {
                        case 0:
                            //A closed cell containing a mine
                            if (getGraph(judge)->adjListArr[v].node->isMine == 1) {
                                printf("|  F");
                            }

                            break;
                        
                        case 2:
                            num = getAdjacentMine(getGraph(judge), v);
                            if (num == 0) {
                                printf("|   ");
                            } else {
                                printf("|%3d", num);
                            }
                            break;
                        case 3:

                            printf("|  F");
                            break;
                        case 4:
                            printf("|  F");
                            break;


                    }
                    break;
                case 3:
                    /*Lose a game*/
                    switch (getState(getGraph(judge), v)) {
                        case 0:
                            
                            if (getGraph(judge)->adjListArr[v].node->isMine == 1) {
                                printf("|  M");//The closed cell contains a missed mine
                                break;
                            }

                            num = getAdjacentMine(getGraph(judge), v);
                            if (num == 0) {
                                printf("|   ");//empty cell
                            } else {
                                printf("|%3d", num);//its neighbour contains a mine
                            }
                            break;
                        case 1:
                            printf("|  !");//The mine that made the user lose
                            break;
                        case 2:
                            //Open cell
                            num = getAdjacentMine(getGraph(judge), v);
                            if (num == 0) {
                                printf("|   ");
                            } else {
                                printf("|%3d", num);
                            }
                            break;
                        case 3:
                            //A flagged cell
                            if (judge->grid->adjListArr[v].node->isMine == 1) {
                                printf("|  *");//flag a cell containing a mine
                            } else {
                                printf("|  -");//doesn't contain a mine
                            }
                            break;



                        case 4:
                            
                            if (judge->grid->adjListArr[v].node->isMine == 1) {
                                printf("|  *");//question a mined cells
                            } else {
                                printf("|  -");// question a non-mines cells
                            }
                            break;


                    }
                    break;

            }
            
        }
        printf("\n");
        for (int j = 1; j <= getGraph(judge)->width + 1; j++) {
            printf("---|");
        }
        printf("\n");
    }
}
