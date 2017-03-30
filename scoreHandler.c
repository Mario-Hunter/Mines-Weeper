#include <math.h>

#include "judge.h"
#include "graph.h"

long int getScore(judge_t *judge) {
    
    calculateTime(judge); // update the gameTime
    if (pow(judge->grid->width, 4) * pow(judge->grid->height, 4)<(judge->gameTime * judge->moves)) {
        judge->gameScore = 0;
        

        return judge->gameScore;
    }
    judge->gameScore = (long int) pow(judge->grid->width, 4) * pow(judge->grid->height, 4) / (judge->gameTime * judge->moves);
    //printf("\nTime %d \n Moves %d\n SCOOOORE %ld\n", judge->gameTime, judge->moves, judge->gameScore);
    return judge->gameScore;
}
