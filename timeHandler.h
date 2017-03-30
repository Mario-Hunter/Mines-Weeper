/* 
 * File:   timeHandler.h
 * Author: Mario
 *
 * Created on December 4, 2016, 5:12 PM
 */

#ifndef TIMEHANDLER_H
#define	TIMEHANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

void setTime(time_t *clock);
double getTime(time_t time1, time_t time2);



#ifdef	__cplusplus
}
#endif

#endif	/* TIMEHANDLER_H */

