#ifndef HEATMAP_H_
#define HEATMAP_H_

#include "conversion.h"

// Heatmap properties
#define HEATMAP_H 10
#define HEATMAP_V 6
#define HM_SHADES 10

void draw_heatmap(GPSPoint points[], int numPoints);

#endif /* HEATMAP_H_ */
