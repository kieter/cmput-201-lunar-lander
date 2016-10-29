/*******************************************************************************
* Name:		           Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID: 	   	  	 balisnom
* Lecture section:   A1
* Lab section:	   	 D06
* TA names: 	   	   Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/
#ifndef _A3_H
#define _A3_H

/* macros */
#define FUEL_LINES 5
#define BOOST_LINES 5 
#define FUEL_DECR 1
#define BOOST_DECR  2

/* global variables  */
extern int running;
extern int game_over;
extern int won;
extern int i_opt;
extern int boosting;

/* function prototypes */
/* initializes and deinitializes curses */
void init_ncurses();
void shutdown_ncurses();
/* writes crashed or landed on the screen */
void crashed_landed();

#endif 