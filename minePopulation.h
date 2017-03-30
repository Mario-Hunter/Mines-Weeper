/* 
 * File:   minePopulation.h
 * Author: Mario
 *
 * Created on December 1, 2016, 10:03 PM
 */

#ifndef MINEPOPULATION_H
#define	MINEPOPULATION_H

#ifdef	__cplusplus
extern "C" {
#endif

bool containsMine(graph_t* graph,int vertex);
void plantMine(graph_t *graph,int vertex);
void getMinePosition(graph_t* graph,int startingNode);


#ifdef	__cplusplus
}
#endif

#endif	/* MINEPOPULATION_H */

