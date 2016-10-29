/*******************************************************************************
* Name:		           Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID: 	   	  	 balisnom
* Lecture section:   A1
* Lab section:	   	 D06
* TA names: 	   	   Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <time.h>

#include "timer.h"
#include "ship.h"
#include "intersect.h"
#include "a3.h"

/* blocks the signal every time it occurs and updates the position of the 
	thrust and the ship, checks any intersections, and sees if the game has ended
	yet and whether or not the player has won */
void handle_signal(int signal)
{
	/* for checking intersection */
	double ax = 0,ay = 0;
	double bx = 0,by = 0;
	double cx = 0,cy = 0;
	double dx = 0,dy = 0;
	int intersection = 0;
	/* for iteration */
	int i = 0;

	/* catches only the specified signal continues the following actions if the
		program has not halted yet */
	if(signal == SIGALRM && running)
	{
		/* if the game is over, do not perform the actions in this function */
		if (game_over)
		{
			return;
		}
		/* for signal handling and blocking */
		sigset_t block_mask;
		sigset_t old_mask;
		sigemptyset(&block_mask);
		sigaddset(&block_mask, SIGALRM);

		/* block signal */
		if(sigprocmask(SIG_BLOCK, &block_mask, &old_mask) < 0)
		{
			exit(EXIT_FAILURE);
		}

		/* erase the ship, update it's properties, and redraw */
		erase_ship();
		update_ship();
		/* reset the accelerations of the ship */
		ship->xlrate = 0;
		ship->ylrate = gravity;
		/* reset the accelerations of the thrust */
		thrust_img->xlrate = 0;
		thrust_img->ylrate = gravity;

		/* check intersection of every point */
		for(i = 1; i < ship->num_coords; i++)
		{
			ax = ship->coords[i - 1].x;
			ay = ship->coords[i - 1].y;
			bx = ship->coords[i].x;
			by = ship->coords[i].y;

			for(i = 1; i < landscape->num_coords; i++)
			{
				cx = landscape->coords[i - 1].x;
				cy = landscape->coords[i - 1].y;
				dx = landscape->coords[i].x;
				dy = landscape->coords[i].y;

				intersection = check_intersect(ax,ay, bx,by, cx,cy, dx,dy);
				/* if the ship has intersected with the landscape, the game is over 
					also, check to see if the player won */
				if (intersection == 1)
				{
					game_over = 1;
					won = check_win(cy, dy);
					break;
				}
			}
			if (intersection == 1)
			{
				game_over = 1;
				won = check_win(cy, dy);
				break;
			}
		}

		/* check the last coordinate of the ship against the landscape for
			intersection */
		for(i = 1; i < landscape->num_coords; i++)
		{
			ax = ship->coords[3].x;
			ay = ship->coords[3].y;
			bx = ship->coords[4].x;
			by = ship->coords[4].y;

			cx = landscape->coords[i - 1].x;
			cy = landscape->coords[i - 1].y;
			dx = landscape->coords[i].x;
			dy = landscape->coords[i].y;

			intersection = check_intersect(ax,ay, bx,by, cx,cy, dx,dy);
			/* again, check to see if the player has won */
			if (intersection == 1)
			{
				game_over = 1;
				won = check_win(cy, dy);
				break;
			}
		}
		/* ensures the ship stays within the borders of sketchpad */
		check_border();

		/* if there was an intersection, stop the timer */
		if (intersection == 1)
		{
			struct itimerval timer;
				/* set up the timer */
			timer.it_value.tv_sec = 0;
			timer.it_value.tv_usec = 0;
			timer.it_interval.tv_sec = 0;
			timer.it_interval.tv_usec = 0;
			if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
			{
				exit(EXIT_FAILURE);
			}
		}
		/* unblock signal and continue the game */
		if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
		{
			exit(EXIT_FAILURE);
		}
	}
} /* end of handle_signal */

/* checks to see whether or not the player has won the game
	takes in the land the ship has intersected with as arguments */
int check_win(double cy, double dy)
{
	/* if the ship has landed upright, the ships velocity in the y direction
		is sufficiently slow, and the platform the ship has landed on is flat the
		player has won */
	if (ship->angle_deg == UPRIGHT && ship->vel_y <= FAST && cy == dy)
	{
		return 1;
	}
	/* otherwise they lost */
	else
	{
		return 0;
	}
} /* end of check_win */

/* ensures that the ship stays within the borders of sketchpad by checking
	where the ship's midpoint currently is, moves the ship if the ship is not
	within the borders */
void check_border()
{
	/* find the current midpoint of the ship */
	calc_midpoint();
	/* move the ship away from the border it has reached by redrawing it */
  if(ship->midpoint.x >= RIGHT_BORD)
  {
  	move_ship(-15, 0);
  } /* end of right border condition */
  else if(ship->midpoint.x <= LEFT_BORD)
  {
  	move_ship(15, 0);
  } /* end of left border condition */
  else if(ship->midpoint.y <= TOP_BORD)
  {
  	move_ship(0, 15);
  } /* end of top border condtion */
  else if(ship->midpoint.y >= BOT_BORD)
  {
  	move_ship(0, -15);
  } /* end of bottom border condition */
} /* end of check_border */