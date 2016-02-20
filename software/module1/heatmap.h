#ifndef HEATMAP_H_
#define HEATMAP_H_

#include "conversion.h"
#include "graphics.h"
#include "sub_menus.h"

void draw_heatmap(GPSPoint points[], int numPoints, Colours colourScheme);
void connect_points(GPSPoint points[], int numPoints, Colours colourScheme);

#endif /* HEATMAP_H_ */
