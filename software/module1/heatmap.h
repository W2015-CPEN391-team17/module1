#ifndef HEATMAP_H_
#define HEATMAP_H_

#include "conversion.h"
#include "graphics.h"
#include "sub_menus.h"

// Drawing modes
#define NUM_DRAW_MODES 2
#define MODE_HEATMAP 0
#define MODE_CONNECT 1

void draw_heatmap(GPSPoint points[], int numPoints, Colours colourScheme);
void connect_points(GPSPoint points[], int numPoints, Colours colourScheme);
void draw_data(GPSPoint points[], int numPoints, Colours colourScheme, int draw_mode);

#endif /* HEATMAP_H_ */
