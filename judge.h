/* 
 * File:   judge.h
 * Author: Mario
 *
 * Created on December 2, 2016, 5:08 PM
 */
#include <time.h>
#include "graph.h"
#ifndef JUDGE_H
#define	JUDGE_H
typedef struct {
    char name[100];
    int score;
}player;
typedef struct judge {
    int gameState;
    time_t gameStartTime;
    int gameTime;
    int moves;
    long int gameScore ;
    char user[10];
    graph_p grid;


} judge_t, *judge_p;
#ifdef	__cplusplus
extern "C" {
#endif
    void setLose(judge_t *judge);
    void openEmpty(graph_t *graph, judge_t *judge, int v);
    graph_p createJudge(void);
    void startGame(void);
    void flagCell(graph_t *graph, int v);
    void questionCell(graph_t *graph, int v);
    void openCell(judge_p *judge, graph_t *graph, int v);
    __inline graph_p getGraph(judge_t *judge);
    void checkWin(judge_t *judge);
    void lose(judge_t *judge) ;
    char * setUser(void);
    void finishGame(judge_t *judge, int score);
    void resetAlarm(void);


#ifdef	__cplusplus
}
#endif

#endif	/* JUDGE_H */

