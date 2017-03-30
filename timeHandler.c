#include <time.h>
#include "judge.h"

double getTime(time_t time1, time_t time2){
    /*Return the time in seconds between two time structures*/
    double seconds = difftime( time1,  time2);
    return seconds;
}

void calculateTime(judge_t *judge){
    /*Calculate the time interval of the game*/
    time_t currentTime;

    time(&currentTime);
    judge->gameTime += getTime(currentTime, judge->gameStartTime);
    judge->gameStartTime = currentTime;
}
void setTime(time_t *clock){
    /*setting the current time into a ime sructure*/
    time(clock);
}
