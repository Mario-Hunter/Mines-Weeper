/* 
 * File:   inputHandler.h
 * Author: Mario
 *
 * Created on December 2, 2016, 9:42 PM
 */

#ifndef INPUTHANDLER_H
#define	INPUTHANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

int hash(char query[],int width,int height);
void takeInput(graph_p *graph);
//bool validateCol(int col,int width);

#ifdef	__cplusplus
}
#endif

#endif	/* INPUTHANDLER_H */

