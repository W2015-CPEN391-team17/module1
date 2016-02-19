#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "conversion.h"

// GPSPoints that are read and written to the SD card are represented
// by this schema:
//
// x0,y0;x1,y1;x2,y2;
//
// If you have an array [ { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 } ]
// it would be represented as:
//
// 1.0,2.0;3.0,4.0;5.0,6.0;
//
// The precision of floats in the schema is unspecified.

// Print file contents of a file corresponding to a given filename.
void sd_card_print_contents(char *filename);

// Attempt to write a GPSPoint value to the SD card.
// Will overwrite data on the SD card.
void sd_card_write_point(GPSPoint point, char *filename);

// Attempt to write an array of GPSPoint values to the SD card.
// The parameter `len` is the length of the points array.
// Will overwrite data on the SD card.
void sd_card_write_points(GPSPoint points[], unsigned long len, char *filename);


// GPSPointSets that are read and written to the SD card are represented
// by this schema:
//
// {tag0:x0,y0;x1,y1;},{tag1:x2,y2;}
//
// If set 0 contains point 0 "1.0,2.0;" and point 1 "3.0,4.0;' and set
// 1 contains point 2 "5.0,6.0;", it would be represented as:
//
// "{[0:1.0,2.0;3.0,4.0;][1:5.0,6.0;]}"
//
// The precision of floats in the schema is unspecified.

// Attempt to write multiple GPSPointSets to the SD card.
// The parameter `len` is the number of GPSPoints in sets[].
// Will overwrite data on the SD card.
// Precondition: len < GPSPOINTSET_POINTSMAXLEN
void sd_card_write_GPSPointSets(GPSPointSet sets[], unsigned long len, char *filename);

#endif /* SD_CARD_H_ */
