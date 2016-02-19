#ifndef HEATMAP_H_
#define HEATMAP_H_

#include "conversion.h"

// Heatmap properties
#define HEATMAP_H 5
#define HEATMAP_V 3
#define HM_SHADES 8

void draw_heatmap(GPSPoint points[], int numPoints);

#endif /* HEATMAP_H_ */
