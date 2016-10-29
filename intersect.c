/*******************************************************************************
* Name:              Kieter Philip Balisnomo
* ONE Card number:   1431294
* Unix ID:           balisnom
* Lecture section:   A1
* Lab section:       D06
* TA names:          Andrew Whittle and Sepideh Heydarabad
*******************************************************************************/

#define _XOPEN_SOURCE
#include <math.h>
#include "intersect.h"

/* checks whether or not the ship has intersected with the landscape
  returns 1 if so, returns 0 otherwise */
int check_intersect(double ax,double ay, double bx,double by,
  double cx,double cy, double dx,double dy)
{
	double distab;
	double thecos;
	double thesin;
	double newx;
	double abpos;

  /* fail if either line segment is zero-length */
  if ((ax == bx && ay == by) || (cx == dx && cy == dy))
  {
  	return NO;
  }

  /* fail if the segments share an end-point */
  if ((ax == cx && ay == cy) || (bx == cx && by == cy)
  		|| (ax == dx && ay==dy) || (bx == dx && by == dy))
  {
    return NO; 
  }

  /* (1) Translate the system so that point a is on the origin */
  bx-= ax; by -= ay;
  cx-= ax; cy -= ay;
  dx-= ax; dy -= ay;

  /* discover the length of segment a-b */
  distab = sqrt(bx * bx + by * by);

  /*  (2) rotate the system so that point b is on the positive x axis */
  thecos = bx / distab;
  thesin = by / distab;
  newx = cx * thecos + cy * thesin;
  cy = cy * thecos - cx * thesin; 
  cx = newx;
  newx = dx * thecos + dy * thesin;
  dy = dy * thecos - dx * thesin; 
  dx = newx;

  /* fail if segment c-d doesn't cross line a-b  */
  if ((cy < 0 && dy < 0) || (cy >= 0 && dy >= 0)) 
  {
  	return NO;
  }

  /* (3) discover the position of the intersection point along line a-b */
  abpos = dx + (cx-dx) * dy / (dy-cy);

  /*  fail if segment c-d crosses line a-b outside of segment a-b */
  if (abpos < 0 || abpos > distab) 
  {
  	return NO;
  }

  /* success */
  return YES; 
}