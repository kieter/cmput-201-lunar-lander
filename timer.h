/*******************************************************************************
* Name:		           Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID: 	   	  	 balisnom
* Lecture section:   A1
* Lab section:	   	 D06
* TA names: 	   	   Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/
#ifndef _TIMER_H
#define _TIMER_H

/* macros */
/* interval */
#define USEC_INTERV 50000
/* conditions for winning */
#define UPRIGHT 90
#define FAST 20
/* borders of sketchpad */
#define TOP_BORD 0
#define LEFT_BORD 0
#define RIGHT_BORD 639
#define BOT_BORD 479

/* handles the signal */
void handle_signal(int signal); 
/* sees if they won */
int check_win(double cy, double dy);
/* checks to make sure the borders are never crossed */
void check_border();

#endif

