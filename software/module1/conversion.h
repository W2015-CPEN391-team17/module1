/*
 * conversion.h
 *
 *  Created on: 2016-02-06
 *      Author: Kyle
 */

#ifndef CONVERSION_H_
#define CONVERSION_H_

#define TOPRIGHTLAT (float) 49.268000
#define TOPRIGHTLONG (float) -123.248000
#define TOPLEFTLAT (float) 49.268000
#define TOPLEFTLONG (float) -123.251000
#define BOTRIGHTLAT (float) 49.266500
#define BOTRIGHTLONG (float) -123.248000

#include "touchScreen.h"

typedef struct { float x, y; } GPSPoint;

typedef struct {
	int tag;  // name of struct (used for ID purposes)
	int points_len;  // number of GPSPoints in points
	GPSPoint points[GPSPOINTSET_POINTSMAXLEN];
} GPSPointSet;

GPSPoint* convertGPSPoints( int nPoints );
void convertGPSReal( GPSPoint topLeft, GPSPoint topRight, GPSPoint points[], GPSPoint bottomRight, int nPoints);


#endif /* CONVERSION_H_ */
