/*******************************************************************************
* Name:              Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID:           balisnom
* Lecture section:   A1
* Lab section:       D06
* TA names:          Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/
#define _XOPEN_SOURCE
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "ship.h"

/* constants for the acceleration of the ship and gravity */
double gravity = 0;
double thrust  = 0;
/* whether or not the improved option was specified */
int i_opt = 0;
/* whether or not the ship is in super-boost mode */
int boosting = 0;

/* draws the ship with it's current coordinates to sketchpad */
void draw_ship()
{
  int i = 0;

  /* if the improved option is specified, the ship is blue */
  if (i_opt)
  {
    set_color(BLUE);
  }

  /* send each line of the ship to sketchpad */
  for (i = 1; i < ship->num_coords; i++)
  {
    fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
      lround(ship->coords[i - 1].x),
      lround(ship->coords[i - 1].y),
      lround(ship->coords[i].x),
      lround(ship->coords[i].y));
  } /* end of for loop */

  if (i_opt)
  {
    set_color(BLACK);
  }
  fflush(sketchpad);
} /* end of draw_ship */

/* erases the ship with it's current coordinates on sketchpad */
void erase_ship()
{
  int i = 0;

  /* send the erase command to each line of the ship to sketchpad */
  for (i = 1; i < ship->num_coords; i++)
  {
    fprintf(sketchpad, "eraseSegment %ld %ld %ld %ld\n",
      lround(ship->coords[i - 1].x),
      lround(ship->coords[i - 1].y),
      lround(ship->coords[i].x),
      lround(ship->coords[i].y));
  } /* end of for loop */
} /* end of erase_ship */

/* draws the thrust with it's current coordinates on sketchpad */
void draw_thrust()
{
  int i = 0;

  /* the thrust is red if the improved option is specified */
  if (i_opt)
  {
    set_color(RED);
  }
  
  /* if boosting, the thrust is orange */
  if (boosting)
  {
    set_color(ORANGE);
  }

  /* send all oft he drawsegments commands for the thrust to sketcphad */
  for (i = 1; i < thrust_img->num_coords; i++)
  {
    fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
      lround(thrust_img->coords[i - 1].x),
      lround(thrust_img->coords[i - 1].y),
      lround(thrust_img->coords[i].x),
      lround(thrust_img->coords[i].y));
  } /* end of for loop */
  fflush(sketchpad);

  if (i_opt)
  {
    set_color(BLACK);
  }

  erase_thrust();
} /* end of draw_thrust */

/* erases the current coordinates of the thrust on sketchpad */
void erase_thrust()
{
  int i = 0;

  /* send all of the erasesegment commands of the thrust to sketchpad */
  for (i = 1; i < thrust_img->num_coords; i++)
  {
    fprintf(sketchpad, "eraseSegment %ld %ld %ld %ld\n",
      lround(thrust_img->coords[i - 1].x),
      lround(thrust_img->coords[i - 1].y),
      lround(thrust_img->coords[i].x),
      lround(thrust_img->coords[i].y));
  } /* end of for loop */
} /* end of erase_thrust */

/* MOVE THIS SHIT */
/* reads in a landscape file as specified in the assignment */
void parse_landscape()
{
  /* the landscape */
  landscape = malloc(sizeof(struct image));
  memset(landscape, 0, sizeof(struct image));
  /* for file parsing */
  char line[LINE_BUF + 1];
  memset(line, 0, sizeof(line));
  int line_num = 0;
  char word[LINE_BUF + 1];
  memset(word, 0, sizeof(word));
  /* for parsing the file */
  long x_cur;
  long y_cur;

  /* parse each line of the landscape */
  while(fgets(line, sizeof(line), in_file) != NULL)
  {
    x_cur = 0;
    y_cur = 0;
    line_num++;
    sscanf(line, "%li %li", &x_cur, &y_cur);
    landscape->coords[landscape->num_coords].x = x_cur;
    landscape->coords[landscape->num_coords].y = y_cur;
    landscape->num_coords++;
  } /* end of while loop */
} /* end of parse_landscape */

/* draws the landscape */
void draw_landscape()
{
  int i = 0;

  /* the landscape is grey if improved is specified */
  if (i_opt)
  {
    set_color(GREY);
  }

  /* send each drawsegment to sketchpad */
  for(i = 1; i < landscape->num_coords; i++)
  {
    fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
      lround(landscape->coords[i - 1].x),
      lround(landscape->coords[i - 1].y),
      lround(landscape->coords[i].x),
      lround(landscape->coords[i].y));
  } /* end of for loop */
  fflush(sketchpad);

  if (i_opt)
  {
    set_color(BLACK);
  }
  
} /* end of draw_landscape */

/* calculates and returns the midpoint of the ship */
struct coordinate calc_midpoint()
{
  struct coordinate temp_midpt;
  /* for iteration */
  double x_cur = 0;
  double y_cur = 0;
  int i = 0;
  /* for finding the mid-point */
  double min_x = 0;
  double max_x = 0;
  double min_y = 0;
  double max_y = 0;
  /* these points are the coordinate of the midpoint */
  double centre_x = 0;
  double centre_y = 0;

  /* start searching for the midpoint */
  min_x = ship->coords[0].x;
  max_x = min_x;
  min_y = ship->coords[0].y;
  max_y = min_y;
  /* do sequential search algorithm */
  for (i = 0; i < ship->num_coords; i++)
  {
    x_cur = ship->coords[i].x;
    y_cur = ship->coords[i].y;
    if (x_cur > max_x)
    {
      max_x = x_cur;
    }
    if (x_cur < min_x)
    {
      min_x = x_cur;
    }
    if (y_cur > max_y)
    {
      max_y = y_cur;
    }
    if (y_cur < min_y)
    {
      min_y = y_cur;
    }
  } /* end of for loop to find largest x,y and smallest x,y */

  /* determine the midpoint */
  centre_x = (min_x + max_x) / 2.0;
  centre_y = (min_y + max_y) / 2.0;
  temp_midpt.x = centre_x; temp_midpt.y = centre_y;
  /* update the struct ship so it knows its midpoint */
  ship->midpoint.x = centre_x; ship->midpoint.y = centre_y;

  return temp_midpt;
}

void rotate_ship(double angle_deg)
{
  double centre_x = 0;
  double centre_y = 0;
  struct coordinate midpoint;
  // memset(midpoint, 0, sizeof(struct coordinate));
  int i = 0;
  /* for rotation */
  double rot_x = 0;
  double rot_y = 0;
  double x_cur = 0;
  double y_cur = 0;
  double angle_rad = 0;
  /* for moving the ship to the origin and back */
  midpoint = calc_midpoint();
  centre_x = midpoint.x;
  centre_y = midpoint.y;


  /* rotate right or left by 10 degrees */
  if (angle_deg == ROT_RIGHT)
  {
    ship->angle_deg += ROT_RIGHT;
  }
  else if (angle_deg == ROT_LEFT)
  {
    ship->angle_deg += ROT_LEFT;
  }

  /* determine the angle in radians to use cos and sin */
  angle_rad = angle_deg * M_PI / 180.0;

  /* rotate every point of the ship */
  for(i = 0; i < ship->num_coords; i++)
  {
    x_cur = ship->coords[i].x;
    y_cur = ship->coords[i].y;

    /* rotate the ship by moving it to the origin, rotating, and back again */
    rot_x = cos(angle_rad) * (x_cur - centre_x) - sin(angle_rad) * 
      (y_cur - centre_y) + centre_x;
    rot_y = sin(angle_rad) * (x_cur - centre_x) + cos(angle_rad) * 
      (y_cur - centre_y) + centre_y;
    ship->coords[i].x = rot_x;
    ship->coords[i].y = rot_y;
  }

  /* rotate the thrust so that it's always beneath the ship */
  for(i = 0; i < thrust_img->num_coords; i++)
  {
    x_cur = thrust_img->coords[i].x;
    y_cur = thrust_img->coords[i].y;

    rot_x = cos(angle_rad) * (x_cur - centre_x) - sin(angle_rad) * 
      (y_cur - centre_y) + centre_x;
    rot_y = sin(angle_rad) * (x_cur - centre_x) + cos(angle_rad) * 
      (y_cur - centre_y) + centre_y;
    thrust_img->coords[i].x = rot_x;
    thrust_img->coords[i].y = rot_y;
  }
}
/* deals with updating the ship's positions according to gravity and thrust 
  using (simplified) physics formulas.
  the signal handler calls this function every 0.05 seconds  */
void update_ship()
{
  /* for updating the ships velocities */
  double old_vel_x = ship->vel_x;
  double old_vel_y = ship->vel_y;
  int i = 0;

  /* update ship positions */
  for(i = 0; i < ship->num_coords; i++)
  {
    ship->coords[i].x = ship->coords[i].x + old_vel_x * TIME + 0.5 
    * ship->xlrate * TIME*TIME;
    ship->coords[i].y = ship->coords[i].y + old_vel_y * TIME + 0.5
    * ship->ylrate * TIME*TIME; 
  }

  /* update the thrust's positions */
  for(i = 0; i < thrust_img->num_coords; i++)
  {
    thrust_img->coords[i].x = thrust_img->coords[i].x + old_vel_x * TIME + 0.5 
    * thrust_img->xlrate * TIME*TIME;
    thrust_img->coords[i].y = thrust_img->coords[i].y + old_vel_y * TIME + 0.5
    * thrust_img->ylrate * TIME*TIME; 
  }

  /* update ship velocities */
  ship->vel_x = old_vel_x + ship->xlrate * TIME;
  ship->vel_y = old_vel_y + ship->ylrate * TIME;

  /* update the thrust's velocities */
  thrust_img->vel_x = old_vel_x + thrust_img->xlrate * TIME;
  thrust_img->vel_y = old_vel_y + thrust_img->ylrate * TIME;

  /* redraw the ship */
  draw_ship();
  erase_ship();
  draw_ship();
}

/* deals with the propulsion of the ship due to thrust, also draws thrust 
  when called */
void thrust_ship()
{
  /* convert the ship's angle to radians for sin and cos */
  double angle_rad;
  angle_rad = ship->angle_deg * M_PI / 180.0;

  /* update the ship's and the thrusts acceleration */
  thrust_img->xlrate = thrust * cos(angle_rad);
  thrust_img->ylrate = gravity + thrust * sin(angle_rad);
  ship->xlrate = thrust * cos(angle_rad);
  ship->ylrate = gravity + thrust * sin(angle_rad);

  // draw_ship();
  // erase_ship();
  /* draw the thrust because the ship is thrusting */
  draw_thrust();
  erase_thrust();
}

/* an improvement
  sets the color that sketchpad draws in to be whatever RGB values are 
  specified */
void set_color(int r, int g, int b)
{
  fprintf(sketchpad, "setColor %d %d %d\n", r, g, b);
}

/* an improvement
  randomly sets the positions of some stars near the top of the screen */
void set_stars()
{
  /* for iteration */
  int i = 0;
  int j = 0;
  /* for randomness */
  int random_x = 0;
  int random_y = 0;
  int randomly = 0;
  /* to set the star either on the left or right side */
  int left_right = 0;
  /* the image that contains all of the stars */
  stars = malloc(sizeof(struct image) * NUM_STARS);
  memset(stars, 0, sizeof(struct image) * NUM_STARS);

  /* the first star is always drawn in the same place
    every other one is chosen randomly based on this one */
  stars[0].coords[0].x = 325; stars[0].coords[0].y = 180;
  stars[0].coords[1].x = 332; stars[0].coords[1].y = 170;
  stars[0].coords[2].x = 339; stars[0].coords[2].y = 180;
  stars[0].coords[3].x = 349; stars[0].coords[3].y = 180;
  stars[0].coords[4].x = 340; stars[0].coords[4].y = 189;
  stars[0].coords[5].x = 346; stars[0].coords[5].y = 201;
  stars[0].coords[6].x = 332; stars[0].coords[6].y = 195;
  stars[0].coords[7].x = 320; stars[0].coords[7].y = 202;
  stars[0].coords[8].x = 324; stars[0].coords[8].y = 190;
  stars[0].coords[9].x = 315; stars[0].coords[9].y = 180;
  stars[0].coords[10].x = 325; stars[0].coords[10].y = 180;
  stars[0].num_coords = 11;

  /* pick some coordinates near the top of the screen */
  randomly = (rand() % 100)*5;
  random_x = (rand() % 50)*10;
  random_y = (rand() % 15)*10;
  /* either relatively left or relatively right */
  left_right = rand()%2 + 1;

  /* set all the points of however many stars */
  for (j = 1; j < NUM_STARS; j++)
  {
    randomly = (rand() % 100);
    random_x = (rand() % 50)*10;
    random_y = (rand() % 15)*10;
    left_right = rand()%2 + 1;
    for (i = 0; i < stars[0].num_coords; i++)
    {
      if (left_right == 1)
      {
        stars[j].coords[i].x = stars[0].coords[i].x - random_x;
      }
      else if (left_right == 2)
      {
        stars[j].coords[i].x = stars[0].coords[i].x + random_x; 
      }

      stars[j].coords[i].y = stars[0].coords[i].y - randomly - random_y;
      stars[j].num_coords++;
    }
  }
}

/* an improvement
  draws the randomly selected stars near the top of the screen */
void draw_stars()
{
  /* for iteration */
  int i = 0;
  int j = 0;

  /* the stars are yellow */
  if (i_opt)
  {
    set_color(YELLOW);
  }

  /* draw each and every star by sending the appropriate drawsegment commands
    to sketchpad */
  for (j = 0; j < NUM_STARS ; j++)
  {
    for (i = 1; i < stars[j].num_coords; i++)
    {
      fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
        lround(stars[j].coords[i - 1].x),
        lround(stars[j].coords[i - 1].y),
        lround(stars[j].coords[i].x),
        lround(stars[j].coords[i].y));
    } /* end of for loop drawing every star */
    fflush(sketchpad);
  }
  /* end of for loop iterating over every star struct */
  fflush(sketchpad);

  if (i_opt)
  {
    set_color(BLACK);
  }
}

/* an improvement
  sets up and draws the letters FUEL, the fuel bar frame, and the fuel initial 
  fuel bar lines*/

void draw_fuel()
{
  /* for iteration */
  int i = 0;
  int j = 0;
  /* contains all of the letters of FUEL and initialize */
  fuel_letters = malloc(sizeof(struct image) * FUEL_LETTERS);
  memset(fuel_letters, 0, sizeof(struct image) * FUEL_LETTERS);

  /* the fuel bar */
  fuel_bar->coords[0].x = 500; fuel_bar->coords[0].y = 10;
  fuel_bar->coords[1].x = 630; fuel_bar->coords[1].y = 10;
  fuel_bar->coords[2].x = 630; fuel_bar->coords[2].y = 20;
  fuel_bar->coords[3].x = 500; fuel_bar->coords[3].y = 20;
  fuel_bar->coords[4].x = 500; fuel_bar->coords[4].y = 10;
  fuel_bar->num_coords = 5;

  /* the fuel lines */
  set_color(GREEN);
  fuel_lines[0].coords[0].x = 505; fuel_lines[0].coords[0].y = 13;
  fuel_lines[0].coords[1].x = 625; fuel_lines[0].coords[1].y = 13;
  fuel_lines[0].num_coords = 2;
  fuel_lines[1].coords[0].x = 505; fuel_lines[1].coords[0].y = 14;
  fuel_lines[1].coords[1].x = 625; fuel_lines[1].coords[1].y = 14;
  fuel_lines[1].num_coords = 2;
  fuel_lines[2].coords[0].x = 505; fuel_lines[2].coords[0].y = 15;
  fuel_lines[2].coords[1].x = 625; fuel_lines[2].coords[1].y = 15;
  fuel_lines[2].num_coords = 2;
  fuel_lines[3].coords[0].x = 505; fuel_lines[3].coords[0].y = 16;
  fuel_lines[3].coords[1].x = 625; fuel_lines[3].coords[1].y = 16;
  fuel_lines[3].num_coords = 2;
  fuel_lines[4].coords[0].x = 505; fuel_lines[4].coords[0].y = 17;
  fuel_lines[4].coords[1].x = 625; fuel_lines[4].coords[1].y = 17;
  fuel_lines[4].num_coords = 2;
  draw_gauge_lines(fuel_lines);
  set_color(BLACK);

  /* the letter L */
  fuel_letters[0].coords[0].x = 493; fuel_letters[0].coords[0].y = 12;
  fuel_letters[0].coords[1].x = 493; fuel_letters[0].coords[1].y = 17;
  fuel_letters[0].coords[2].x = 496; fuel_letters[0].coords[2].y = 17;
  fuel_letters[0].num_coords = 3;

  /* the letter E */
  fuel_letters[1].coords[0].x = 487; fuel_letters[1].coords[0].y = 12;
  fuel_letters[1].coords[1].x = 487; fuel_letters[1].coords[1].y = 17;
  fuel_letters[1].coords[2].x = 490; fuel_letters[1].coords[2].y = 17;
  fuel_letters[1].coords[3].x = 487; fuel_letters[1].coords[3].y = 17;
  fuel_letters[1].coords[4].x = 487; fuel_letters[1].coords[4].y = 14;
  fuel_letters[1].coords[5].x = 490; fuel_letters[1].coords[5].y = 14;
  fuel_letters[1].coords[6].x = 487; fuel_letters[1].coords[6].y = 14;
  fuel_letters[1].coords[7].x = 487; fuel_letters[1].coords[7].y = 12;
  fuel_letters[1].coords[8].x = 490; fuel_letters[1].coords[8].y = 12;
  fuel_letters[1].num_coords = 9;

  /* the letter U */
  fuel_letters[2].coords[0].x = 480; fuel_letters[2].coords[0].y = 12;
  fuel_letters[2].coords[1].x = 480; fuel_letters[2].coords[1].y = 17;
  fuel_letters[2].coords[2].x = 484; fuel_letters[2].coords[2].y = 17;
  fuel_letters[2].coords[3].x = 484; fuel_letters[2].coords[3].y = 12;
  fuel_letters[2].num_coords = 4;

  /* the letter F */
  fuel_letters[3].coords[0].x = 475; fuel_letters[3].coords[0].y = 12;
  fuel_letters[3].coords[1].x = 475; fuel_letters[3].coords[1].y = 17;
  fuel_letters[3].coords[2].x = 475; fuel_letters[3].coords[2].y = 14;
  fuel_letters[3].coords[3].x = 478; fuel_letters[3].coords[3].y = 14;
  fuel_letters[3].coords[4].x = 475; fuel_letters[3].coords[4].y = 14;
  fuel_letters[3].coords[5].x = 475; fuel_letters[3].coords[5].y = 12;
  fuel_letters[3].coords[6].x = 478; fuel_letters[3].coords[6].y = 12;
  fuel_letters[3].num_coords = 7;


  /* draw the frame of the fuel bar by sending the drawsegment commands 
    to sketchpad */
  for (i = 1; i < fuel_bar->num_coords; i++)
  {
    fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
      lround(fuel_bar->coords[i - 1].x),
      lround(fuel_bar->coords[i - 1].y),
      lround(fuel_bar->coords[i].x),
      lround(fuel_bar->coords[i].y));
  }

  /* draw every letter of FUEL by sending the appropriate commands to 
    sketchpad */
  for (j = 0; j < FUEL_LETTERS; j++)
  {
    for (i = 1; i < fuel_letters[j].num_coords; i++)
    {
      fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
        lround(fuel_letters[j].coords[i - 1].x),
        lround(fuel_letters[j].coords[i - 1].y),
        lround(fuel_letters[j].coords[i].x),
        lround(fuel_letters[j].coords[i].y));
    } /* end of for loop writing letters */
  } /* end of for loop iterating every letter */
}

/* an improvement
  sets up and draws the letters of BOOST, the frame of the boost bar, 
  and the initial bar of boost */
void draw_boost()
{ 
  /* for iteration */
  int i = 0;
  int j = 0;
  /* stores all fo the letters of BOOST and initializes them */
  boost_letters = malloc(sizeof(struct image) * 5);
  memset(boost_letters, 0, sizeof(struct image) * 5);

  /* the boost bar */
  boost_bar->coords[0].x = 530; boost_bar->coords[0].y = 25;
  boost_bar->coords[1].x = 630; boost_bar->coords[1].y = 25;
  boost_bar->coords[2].x = 630; boost_bar->coords[2].y = 35;
  boost_bar->coords[3].x = 530; boost_bar->coords[3].y = 35;
  boost_bar->coords[4].x = 530; boost_bar->coords[4].y = 25;
  boost_bar->num_coords = 5;
  
  set_color(ORANGE);
  /* the boost lines */
  boost_lines[0].coords[0].x = 535; boost_lines[0].coords[0].y = 28;
  boost_lines[0].coords[1].x = 625; boost_lines[0].coords[1].y = 28;
  boost_lines[0].num_coords = 2;
  boost_lines[1].coords[0].x = 535; boost_lines[1].coords[0].y = 29;
  boost_lines[1].coords[1].x = 625; boost_lines[1].coords[1].y = 29;
  boost_lines[1].num_coords = 2;
  boost_lines[2].coords[0].x = 535; boost_lines[2].coords[0].y = 30;
  boost_lines[2].coords[1].x = 625; boost_lines[2].coords[1].y = 30;
  boost_lines[2].num_coords = 2;
  boost_lines[3].coords[0].x = 535; boost_lines[3].coords[0].y = 31;
  boost_lines[3].coords[1].x = 625; boost_lines[3].coords[1].y = 31;
  boost_lines[3].num_coords = 2;
  boost_lines[4].coords[0].x = 535; boost_lines[4].coords[0].y = 32;
  boost_lines[4].coords[1].x = 625; boost_lines[4].coords[1].y = 32;
  boost_lines[4].num_coords = 2;
  draw_gauge_lines(boost_lines);
  set_color(BLACK);


  /* the letter T*/
  boost_letters[0].coords[0].x = 524; boost_letters[0].coords[0].y = 28;
  boost_letters[0].coords[1].x = 524; boost_letters[0].coords[1].y = 33;
  boost_letters[0].coords[2].x = 524; boost_letters[0].coords[2].y = 28;
  boost_letters[0].coords[3].x = 522; boost_letters[0].coords[3].y = 28;
  boost_letters[0].coords[4].x = 526; boost_letters[0].coords[4].y = 28;
  boost_letters[0].num_coords = 5;

  /* the first letter O from the right */
  boost_letters[1].coords[0].x = 511; boost_letters[1].coords[0].y = 28;
  boost_letters[1].coords[1].x = 515; boost_letters[1].coords[1].y = 28;
  boost_letters[1].coords[2].x = 515; boost_letters[1].coords[2].y = 33;
  boost_letters[1].coords[3].x = 511; boost_letters[1].coords[3].y = 33;
  boost_letters[1].coords[4].x = 511; boost_letters[1].coords[4].y = 28;
  boost_letters[1].num_coords = 5;

  /* the second letter O from the right */
  boost_letters[2].coords[0].x = 505; boost_letters[2].coords[0].y = 28;
  boost_letters[2].coords[1].x = 509; boost_letters[2].coords[1].y = 28;
  boost_letters[2].coords[2].x = 509; boost_letters[2].coords[2].y = 33;
  boost_letters[2].coords[3].x = 505; boost_letters[2].coords[3].y = 33;
  boost_letters[2].coords[4].x = 505; boost_letters[2].coords[4].y = 28;
  boost_letters[2].num_coords = 5;

  /* the letter S */
  boost_letters[3].coords[0].x = 520; boost_letters[3].coords[0].y = 28;
  boost_letters[3].coords[1].x = 517; boost_letters[3].coords[1].y = 28;
  boost_letters[3].coords[2].x = 517; boost_letters[3].coords[2].y = 30.5;
  boost_letters[3].coords[3].x = 520; boost_letters[3].coords[3].y = 30.5;
  boost_letters[3].coords[4].x = 520; boost_letters[3].coords[4].y = 32.5;
  boost_letters[3].coords[5].x = 517; boost_letters[3].coords[5].y = 32.5;
  boost_letters[3].num_coords = 6;

  /* the letter B */
  boost_letters[4].coords[0].x = 499; boost_letters[4].coords[0].y = 28;
  boost_letters[4].coords[1].x = 499; boost_letters[4].coords[1].y = 33;
  boost_letters[4].coords[2].x = 503; boost_letters[4].coords[2].y = 33;
  boost_letters[4].coords[3].x = 503; boost_letters[4].coords[3].y = 30;
  boost_letters[4].coords[4].x = 499; boost_letters[4].coords[4].y = 30;
  boost_letters[4].coords[5].x = 502; boost_letters[4].coords[5].y = 30;
  boost_letters[4].coords[6].x = 502; boost_letters[4].coords[6].y = 28;
  boost_letters[4].coords[7].x = 499; boost_letters[4].coords[7].y = 28;
  boost_letters[4].num_coords = 8;

  /* draws all of the coordinates of the boost bar frame */
  for (i = 1; i < boost_bar->num_coords; i++)
  {
    fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
      lround(boost_bar->coords[i - 1].x),
      lround(boost_bar->coords[i - 1].y),
      lround(boost_bar->coords[i].x),
      lround(boost_bar->coords[i].y));
  }

  /* draws all of the letters of BOOST */
  for (j = 0; j < BOOST_LETTERS; j++)
  {
    for (i = 1; i < boost_letters[j].num_coords; i++)
    {
      fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
        lround(boost_letters[j].coords[i - 1].x),
        lround(boost_letters[j].coords[i - 1].y),
        lround(boost_letters[j].coords[i].x),
        lround(boost_letters[j].coords[i].y));
    } /* end of for loop that writes letters */
  } /* end of for loop iterating over every letter */
}

/* draws the lines both the fuel and boost bars, whatever is passed in */
void draw_gauge_lines(struct image* gauge_lines)
{
  /* for iteration */
  int i = 0;
  int j = 0;

  /* draw every line of the gauge */
  for (j = 0; j < GAUGE_LINES; j++)
  {
    for (i = 1; i < gauge_lines[j].num_coords; i++)
    {
      fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n", 
        lround(gauge_lines[j].coords[i - 1].x),
        lround(gauge_lines[j].coords[i - 1].y),
        lround(gauge_lines[j].coords[i].x),
        lround(gauge_lines[j].coords[i].y));
    } /* end of drawsegment commands */
  } /* end of for loop over every line of the gauge */
  fflush(sketchpad);
}

/* erases the lines of both the fuel and boost bars, whatever is passed in */
void erase_gauge_lines(struct image* gauge_lines)
{
  /* for iteration */
  int i = 0;
  int j = 0;

  /* erase every line of the gauge */
  for (j = 0; j < GAUGE_LINES; j++)
  {
    for (i = 1; i < gauge_lines[j].num_coords; i++)
    {
      fprintf(sketchpad, "eraseSegment %ld %ld %ld %ld\n", 
        lround(gauge_lines[j].coords[i - 1].x),
        lround(gauge_lines[j].coords[i - 1].y),
        lround(gauge_lines[j].coords[i].x),
        lround(gauge_lines[j].coords[i].y));
    } /* end of drawsegment commands */
  } /* end of for loop over every line of the gauge */
}

/* updates the coordinates of every line of the gauges as the bar shrinks
  also draws the fuel bar green and the boost bar orange */
int update_gauge(struct image* gauge_lines, int decrement, int r, int g, int b)
{
  /* for iteration */
  int j = 0;

  /* erase the gauge lines */
  erase_gauge_lines(gauge_lines);

  /* iterate over every line of the gauge */
  for (j = 0; j < GAUGE_LINES; j++)
  {
    /* if the first coordinate of the line is not the last coordinate 
      (there is some fuel or boost left), decrement the gauge */
    if (gauge_lines[j].coords[0].x != gauge_lines[j].coords[1].x)
    {
      gauge_lines[j].coords[1].x -= decrement;
    }    
    /* otherwise, return that there is no fuel or boost left */
    else
    {
      return 1;
    }
  }

  /* sets the color and draws the decreased gauge */
  set_color(r,g,b);
  draw_gauge_lines(gauge_lines);
  set_color(BLACK);
  return 0;
}

/* moves the ship (and thrust) by delta x,y if the ship every crosses 
  the boundaries of sketchpad */
void move_ship(int delta_x, int delta_y)
{
  /* for iteration */
  int i = 0;

  /* erase the ship */
  erase_ship();
  /* move every point by delta x,y respectively */
  for (i = 0; i < ship->num_coords; i++)
  {
    ship->coords[i].x += delta_x;
    ship->coords[i].y += delta_y;
    thrust_img->coords[i].x += delta_x;
    thrust_img->coords[i].y += delta_y;
  }
  /* draw the moved ship */
  draw_ship();
}