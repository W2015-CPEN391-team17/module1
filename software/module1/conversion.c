/*
 * conversion.c
 *
 *  Created on: 2016-02-06
 *      Author: Kyle
 */

#include "touchscreen.h" //for definition of Point
#include "conversion.h"
#include "graphics.h" //for XRES/YRES

#include <math.h>

//#include <limits.h> //Couldn't get this include to work, idk why. Have used the min/max def'n's from it in conversion.h

/*Mutates points. Assumes player has ran across the field during some point in the game, in both x and y direction.
 *Assumes field is in line with lat/long.
 *
 *Should really just have defined points of where the field is. Maybe use Google Maps/Earth?
 *Basically, this function is just for testing atm, need a new one once we know where our field is, but concept will be similar.
 *
 */

//@deprecated
void convertGPSPointsToPoints( Point points[] ){
	int minx = points[0].x;
	int miny = points[0].y;
	int maxx = points[0].x;
	int maxy = points[0].y;

	int i = 1;

	//Tim could probably figure out the max/min while pulling points, would be faster
	while(points[i].x != INT_MIN){
		if(points[i].x < minx){
			minx = points[i].x;
		}else if(points[i].x > maxx){
			maxx = points[i].x;
		}

		if(points[i].y < miny){
			miny = points[i].y;
		}else if(points[i].y > maxy){
			maxy = points[i].y;
		}
		i++;
	}

	int xRange = maxx - minx;
	int yRange = maxy - miny;

	double mX = XRES / (double) xRange;
	double mY = YRES / (double) yRange;

	i = 1;

	while(points[i].x != INT_MIN){
		points[i].x = mX * (points[i].x - minx);
		points[i].y = mY * (points[i].y - miny);

		i++;
	}
}

/*
 * Converts GPSpoints to Points (for graphics). Does NOT take care of points outside of range. Only works for topRight in quadrant 4
 * relative to topLeft. Needs to be tested.
 */
void convertGPSReal( GPSPoint topLeft, GPSPoint topRight, GPSPoint points[], GPSPoint bottomRight, int nPoints ){
	double theta = -atan((topRight.y - topLeft.y)/(topRight.x - topLeft.x));

	int i;
	for(i = 0; i < nPoints; i++){
		float oldY = points[i].y - topLeft.y;
		points[i].y = topLeft.y + oldY*cos(theta) + (points[i].x - topLeft.x)*sin(theta);
		points[i].x = topLeft.x + (points[i].x - topLeft.x)*cos(theta) - oldY*sin(theta);
	}

	float oldY = bottomRight.y - topLeft.y;
	bottomRight.y = topLeft.y + oldY*cos(theta) + (bottomRight.x - topLeft.x)*sin(theta);
	bottomRight.x = topLeft.x + (bottomRight.x - topLeft.x)*cos(theta) - oldY*sin(theta);

	oldY = topRight.y - topLeft.y;
	topRight.y = topLeft.y + oldY*cos(theta) + (topRight.x - topLeft.x)*sin(theta);
	topRight.x = topLeft.x + (topRight.x - topLeft.x)*cos(theta) - oldY*sin(theta);

	double mX = XRES / (double) (topRight.x - topLeft.x);
	double mY = YRES / (double) (topRight.y - bottomRight.y);

	int k;
	for( k = 0; k < nPoints; k++){
		points[k].x = mX * (points[k].x - topLeft.x);
		points[k].y = mY * (points[k].y - bottomRight.y);

		if(points[k].x < 0 && points[k].x > -1){
			points[k].x = 0;
		}else if(points[k].x > XRES && points[k].x < XRES + 1){
			points[k].x = XRES;
		}

		if(points[k].y < 0 && points[k].y > -1){
					points[k].y = 0;
		}else if(points[k].y > YRES && points[k].y < YRES + 1){
			points[k].y = YRES;
		}
	}

	/*This bit is for testing purposes. Should be changed to XRES/YRES/0 for final*/
	topRight.x = mX * (topRight.x - topLeft.x);//should eval to XRES
	topRight.y = mY * (topRight.y - bottomRight.y);//should eval to YRES

	topLeft.x = mX * (topLeft.x - topLeft.x);//should eval to 0
	topLeft.y = mY * (topLeft.y - bottomRight.y);//should eval to YRES

	bottomRight.x = mX * (bottomRight.x - topLeft.x);//should eval to XRES
	bottomRight.y = mY * (bottomRight.y - bottomRight.y);//should eval to 0
}
