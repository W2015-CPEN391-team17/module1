/*
 * conversion.h
 *
 *  Created on: 2016-02-06
 *      Author: Kyle
 */

#ifndef CONVERSION_H_
#define CONVERSION_H_

#define TOPRIGHTLAT (float) 49.261738
#define TOPRIGHTLONG (float) -123.249931
#define TOPLEFTLAT (float) 49.261738
#define TOPLEFTLONG (float) -123.250021
#define BOTRIGHTLAT (float) 49.261566
#define BOTRIGHTLONG (float) -123.249931
#define GPSPOINTSET_POINTSMAXLEN 1024

#include "touchScreen.h"

typedef struct { float x, y; } GPSPoint;

typedef struct {
	int tag;  		// name of struct (used for ID purposes)
	int points_len;	// number of GPSPoints in points
	GPSPoint points[GPSPOINTSET_POINTSMAXLEN];
} GPSPointSet;

GPSPoint* convertGPSPoints( int nPoints );
void convertGPSReal( GPSPoint topLeft, GPSPoint topRight, GPSPoint points[], GPSPoint bottomRight, int nPoints);


#endif /* CONVERSION_H_ */
