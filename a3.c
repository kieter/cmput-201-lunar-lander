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
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#include "a3.h"
#include "ship.h"
#include "timer.h"
#include "intersect.h"

/* game images */
struct ship* ship;
struct image* landscape;
struct ship* thrust_img;
/* improvements */
struct image* stars;
struct image* fuel_bar;
struct image* boost_bar;
struct image* fuel_lines;
struct image* boost_lines;
struct ship* explosion;
struct image* fuel_letters;
struct image* boost_letters;
/* game states */
int running = 1;
int game_over = 0;
int won = 0;
/* files */
FILE* sketchpad;
FILE* in_file;

/* main essentially runs the entire game, main accepts the arguments,
	initializes the timer, utilizes ncurses, and sets up the ship and frees
	all of the pointers */
int main(int argc, char* argv[])
{
	running = 1;


	/* for piping to sketchpad */
	const char exec_name[] = "java -jar Sketchpad.jar";
	// FILE* sketchpad;
	/* for argument checking */
	double g_option = 0;
	double t_option = 0;
	char f_option[INPUT_BUF];
	// FILE* in_file;
	memset(f_option, 0, sizeof(f_option));
	int opt;
	/* useless pointer for strtod */
	char* ptr;
	/* for ncurses */
	int c = 0;
	/* for the timer and the signal */
	struct sigaction handler;
	struct itimerval timer;
	/* for improvements */
	int out_of_fuel = 0;
	int out_of_boost = 0;

	sketchpad = popen(exec_name, "w");

	/* initialize the ship */
	ship = malloc(sizeof(struct ship));
	memset(ship, 0, sizeof(struct ship));
	/* set the ship's points */
	ship->coords[0].x = 300;
	ship->coords[0].y = 20;
	ship->coords[1].x = 320;
	ship->coords[1].y = 20;
	ship->coords[2].x = 325;
	ship->coords[2].y = 50;
	ship->coords[3].x = 295;
	ship->coords[3].y = 50;
	ship->coords[4].x = 300;
	ship->coords[4].y = 20;
	ship->num_coords = 5;
	/* initialize (most of) the ship's physics */
	ship->angle_deg = 90;
	ship->vel_x = 0;
	ship->vel_y = 0;
	ship->xlrate = 0;

	/* initialize the thrust image */
	thrust_img = malloc(sizeof(struct ship));
	memset(thrust_img, 0, sizeof(struct ship));
	/* set the thrust_img's points */
	thrust_img->coords[0].x = 295;
	thrust_img->coords[0].y = 50;
	thrust_img->coords[1].x = 310;
	thrust_img->coords[1].y = 57;
	thrust_img->coords[2].x = 325;
	thrust_img->coords[2].y = 50;
	thrust_img->num_coords = 3;
	/* intialize (most of) the thrust_img's physics */
	thrust_img->angle_deg = 90;
	thrust_img->vel_x = 0;
	thrust_img->vel_y = 0;
	thrust_img->xlrate = 0;

  /* argument checking */
	while((opt = getopt(argc, argv, "g:t:f:i")) != -1)
	{
		switch(opt)
		{
			/* the g option is gravity */
			case 'g':
				g_option = strtod(optarg, &ptr);
				if (g_option < 0 || gravity > 20)
				{
					printf("gravity < 0 or gravity > 20 is no allowed.\n");
					exit(EXIT_FAILURE);
				}
				break;
			/* the t option is thrust */
			case 't':
				t_option = strtod(optarg, &ptr);
				if (t_option > 0 || t_option < -20)
				{
					printf("thrust > 0 or thrust < -20 is not allowed.\n");
					exit(EXIT_FAILURE);
				}
				break;
 			/* the f option is following */
			case 'f':
				strncpy(f_option, optarg, INPUT_BUF);
				break;
			/* the improved option */
			case 'i':
				i_opt = 1;
				break;
			/* if the option is unknown */
			case '?':
				printf("Unknown option or good opion but no value.\n");
				exit(EXIT_FAILURE);
				break;
		} /* end of switch */
	} /* end of while loop */
	/* display the given arguments to screen */
	printf("g_option = %lf, t_option = %lf, f_option = %s\n", g_option, 
			t_option, f_option );

	/* read in the arguments given */
	in_file = fopen(f_option, "r");
	gravity = g_option;

	/* initialize the ship's and the thrust's y acceleration to gravity */
	ship->ylrate = gravity;
	thrust_img->ylrate = gravity;
	thrust = t_option;

	/* initialize the curses and print starting message */
	init_ncurses();
	mvprintw(5, 10, "Press any key to play Lunar Lander.");
	refresh();
  c = getch();
	nodelay( stdscr, TRUE ); 

	/* draw all of the standard game images */
	parse_landscape();
	draw_landscape();
	draw_ship();
	erase_ship();

	/* if the -i option is specified */
	if (i_opt)
	{
		/* in order to randomly generate stars */
		srand(time(NULL));
		/* fuel bar and boost bar setup */
		fuel_bar = malloc(sizeof(struct image));
		memset(fuel_bar, 0, sizeof(struct image));
		boost_bar = malloc(sizeof(struct image));
		memset(boost_bar, 0, sizeof(struct image));
		fuel_lines = malloc(sizeof(struct image) * FUEL_LINES);
  	memset(fuel_lines, 0, sizeof(struct image) * FUEL_LINES);
  	boost_lines = malloc(sizeof(struct image) * BOOST_LINES);
  	memset(fuel_lines, 0, sizeof(struct image) * BOOST_LINES);
  	/* draw all of the improvements */
		draw_fuel();
		draw_boost();
		set_stars();
		draw_stars();
	}

	/* game start! */
	erase();
  mvprintw(5, 10, "Press arrow keys to move, space to thrust, 'q' to quit." );
  refresh();

  /* set the signal handler */
  handler.sa_handler = handle_signal;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	/* ensure that the only signal being handled is the one specified */
	if(sigaction(SIGALRM, &handler, NULL) < 0)
	{
		exit(EXIT_FAILURE);
	}

	/* set up the timer */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = USEC_INTERV;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = USEC_INTERV;
	if(setitimer(ITIMER_REAL, &timer, NULL) < 0)
	{
		exit(EXIT_FAILURE);
	}
	
	/* main game */
	while (running)
	{
		c = getch();
		if (game_over)
		{
			crashed_landed();
		}
		if (c == ERR)
		{
			continue;
		}
		erase();
		if (i_opt)
		{
			mvprintw(5, 10, "Press arrow keys to move, up or space to thrust.");
			mvprintw(6, 10, "'q' to quit and 'b' to boost.");
		}
		else
		{
			mvprintw( 5, 10, "Press arrow keys to move, up or space to thrust, 'q' to quit." );
		}
		if (i_opt)
		{
			move(7, 10);
		}
		else
		{
			move(6, 10);
		}

		/* ncurses */
		switch(c)
		{
			/* up is the thrust key */
			case ' ':
			case KEY_UP:
				printw("Up key or Spacebar pressed.");
				/* if i option, there is fuel and a boost gauge */
				if (i_opt)
				{
					out_of_fuel = update_gauge(fuel_lines, FUEL_DECR, GREEN);
					if (!out_of_fuel)
					{
						thrust_ship();
					}
					if (boosting)
					{
						out_of_boost = update_gauge(boost_lines, BOOST_DECR, ORANGE);
						if (out_of_boost && !out_of_fuel)
						{
							boosting = 0;
							mvprintw(7, 10, "Out of super-boost...");
							thrust /= 3;
						}
						thrust_ship();
					}
				}
				else
				{
					thrust_ship();
				}
				break;
			/* rotate right */
			case KEY_RIGHT:
				printw( "Right key pressed.");
				erase_ship();
				rotate_ship(ROT_RIGHT);
				break;
			/* rotate left */
			case KEY_LEFT:
				printw("Left key pressed.");
				erase_ship();
				rotate_ship(ROT_LEFT);
				break;
			/* q is to quit */
			case 'q':
				running = 0;
				break;
			/* b is available if it is improved */
			if (i_opt)
			{	
				case 'b':
					if (boosting)
					{
						printw("Super-boost disengaged...");
						thrust /= 3;
						boosting = 0;
					}
					else
					{
						printw("Super-boost engaged!");
						thrust *= 3;
						boosting = 1;
					}
			}

			default:
				break;
		} /* end of switch */
		/* if the game is over, print crash or landed to the terminal */
		if (game_over)
		{
			crashed_landed();
			erase_ship();
			erase_thrust();
		}
	} /* end of curses loop */

	/* cleanup */
	shutdown_ncurses();
	free(ship);
	free(thrust_img);
	free(stars);
	free(landscape);
	if (i_opt)
	{
		free(fuel_bar);
		free(boost_bar);
		free(fuel_letters);
		free(boost_letters);
		free(fuel_lines);
		free(boost_lines);
	}
	fprintf(sketchpad, "end\n");
	pclose(sketchpad);
	fclose(in_file);
	
	return 0;
} /* end of main */

/* displays CRASHED ! ! ! or LANDED ! ! ! to the user */
void crashed_landed()
{
	if (won == 1)
	{
		mvprintw(9, 10, "LANDED ! ! !\n");
		refresh();
	}
	else if (won == 0)
	{
		mvprintw(9, 10, "CRASHED ! ! !\n");
		refresh();
	}
}

/* initializes the curses */
void init_ncurses()
{
  int r;

  initscr(); 

  r = cbreak(); assert( r != ERR );

  r = noecho(); assert( r != ERR );

  r = nonl(); assert( r != ERR );

  r = curs_set( 0 ); assert( r != ERR );

  r = leaveok( stdscr, TRUE ); assert( r != ERR );

  r = intrflush( stdscr, FALSE ); assert( r != ERR );

  r = keypad( stdscr, TRUE ); assert( r != ERR );
} /* end of init_curses() */

/* shutsdown the curses */
void shutdown_ncurses()
{
  endwin();
} /* end of shutdown_curses */

