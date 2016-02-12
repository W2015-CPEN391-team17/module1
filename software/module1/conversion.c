/*
 * conversion.c
 *
 *  Created on: 2016-02-06
 *      Author: Kyle
 */

#include "conversion.h"
#include "graphics.h" //for XRES/YRES and MENU_TOP

#include <math.h>

/*Mutates points. Assumes player has ran across the field during some point in the game, in both x and y direction.
 *Assumes field is in line with lat/long.
 *
 *Should really just have defined points of where the field is. Maybe use Google Maps/Earth?
 *Basically, this function is just for testing atm, need a new one once we know where our field is, but concept will be similar.
 *
 */

//@deprecated
void convertGPSPointsToPoints( GPSPoint points[], int nPoints ){
	double mX = (double) XRES / (TOPRIGHTLONG - TOPLEFTLONG);
	double mY = (double) (YRES - MENU_TOP) / (TOPLEFTLAT - BOTRIGHTLAT);

	for(int i = 0; i < nPoints; i++){
		points[i].x = mX * (points[i].x - TOPLEFTLONG);
		points[i].y = mY * (points[i].y - BOTRIGHTLAT);

		if(points[i].x > XRES && points[i].x < XRES + 1){
			points[i].x = XRES;
		}
		if(points[i].y > (YRES - MENU_TOP) && points[i].y < (YRES - MENU_TOP) + 1){
			points[i].y = (YRES - MENU_TOP);
		}
	}
}

/*
 * Converts GPSpoints to points for graphics. Does NOT take care of ALL points outside of range. Only works for topRight in quadrant 4
 * relative to topLeft. Needs to be tested.
 */
void convertGPSReal( GPSPoint topLeft, GPSPoint topRight, GPSPoint points[], GPSPoint bottomLeft, int nPoints ){
	double theta = -atan((topRight.y - topLeft.y)/(topRight.x - topLeft.x));

	int i;
	for(i = 0; i < nPoints; i++){
		float oldY = points[i].y - topLeft.y;
		points[i].y = topLeft.y + oldY*cos(theta) + (points[i].x - topLeft.x)*sin(theta);
		points[i].x = topLeft.x + (points[i].x - topLeft.x)*cos(theta) - oldY*sin(theta);
	}

	float oldY = bottomLeft.y - topLeft.y;
	bottomLeft.y = topLeft.y + oldY*cos(theta) + (bottomLeft.x - topLeft.x)*sin(theta);
	bottomLeft.x = topLeft.x + (bottomLeft.x - topLeft.x)*cos(theta) - oldY*sin(theta);

	oldY = topRight.y - topLeft.y;
	topRight.y = topLeft.y + oldY*cos(theta) + (topRight.x - topLeft.x)*sin(theta);
	topRight.x = topLeft.x + (topRight.x - topLeft.x)*cos(theta) - oldY*sin(theta);

	double mX = XRES / (double) (topRight.x - topLeft.x);
	double mY = YRES / (double) (topRight.y - bottomLeft.y);

	int k;
	for( k = 0; k < nPoints; k++){
		points[k].x = mX * (points[k].x - topLeft.x);
		points[k].y = mY * (points[k].y - bottomLeft.y);

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
	topRight.y = mY * (topRight.y - bottomLeft.y);//should eval to YRES

	topLeft.x = mX * (topLeft.x - topLeft.x);//should eval to 0
	topLeft.y = mY * (topLeft.y - bottomLeft.y);//should eval to YRES

	bottomLeft.x = mX * (bottomLeft.x - topLeft.x);//should eval to XRES
	bottomLeft.y = mY * (bottomLeft.y - bottomLeft.y);//should eval to 0
}
