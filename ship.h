/*******************************************************************************
* Name:		           Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID: 	   	  	 balisnom
* Lecture section:   A1
* Lab section:	   	 D06
* TA names: 	   	   Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/
#ifndef _CURSE_EXER_H
#define _CURSE_EXER_H

/* macros */
#define SHIP_LINES 5 //number of lines the ship has
#define FUEL_LETTERS 4
#define BOOST_LETTERS 5
#define FUEL_LINES 5
#define BOOST_LINES 5 
#define GAUGE_LINES 5
#define GAUGE_DECREMENT 0.5
/* macros */
#define POINT_BUF 20
#define ROT_RIGHT 10
#define ROT_LEFT -10
#define TIME 0.05
#define INPUT_BUF 128
#define LINE_BUF 256
/* colours for sketchad's setColor */
#define RED 220, 20, 60
#define BLUE 65, 105, 225
#define YELLOW 238, 232, 170
#define GREY 119, 136, 153
#define GREEN 0, 204, 0 
#define ORANGE 255, 128, 0
#define BLACK 0, 0, 0
/* for the stars improvement */
#define NUM_STARS 50

struct coordinate
{
	double x;
	double y;
};

struct ship
{
	struct coordinate coords[SHIP_LINES];
	int num_coords;
	struct coordinate midpoint;
	double vel_x;
	double vel_y;
	double xlrate;
	double ylrate;
	double angle_deg;
};

struct image
{
	struct coordinate coords[POINT_BUF + 1];
	int num_coords;
};

/* global variables */
extern struct ship* ship;
extern struct ship* thrust_img;
extern FILE* sketchpad;
extern double gravity;
extern double thrust;
extern struct image* landscape;
extern struct image* stars;
extern struct image* fuel_bar;
extern struct image* boost_bar;
extern struct image* fuel_lines;
extern struct image* boost_lines;
extern struct image* fuel_letters;
extern struct image* boost_letters;
extern struct ship* explosion;
extern FILE* in_file;

/* prototype functions */
/* ship functions */
void draw_ship();
void erase_ship();
struct coordinate calc_midpoint();
void rotate_ship(double angle_deg);
void update_ship();
void thrust_ship();
void move_ship(int x_delta, int y_delta);
/* thrust functions */
void draw_thrust();
void erase_thrust();
/* landscape functions */
void parse_landscape();
void draw_landscape();

void cleanup();
/* improvements */
void set_color(int r, int g, int b);
/* random stars */
void set_stars();
void draw_stars();
/* fuel and boost bars */
void draw_fuel();
void draw_boost();
void draw_gauge_lines();
int update_gauge(struct image* gauge_lines, int decrement, int r, int g, int b);
// void set_explosion();

#endif