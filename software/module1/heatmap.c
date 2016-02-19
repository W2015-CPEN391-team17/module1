/*
 * heatmap.c
 *
 * Functions for drawing the heatmap of GPS points.
 */

#include "graphics.h"
#include "heatmap.h"

void draw_heatmap(GPSPoint points[], int numPoints)
{
	//Initialize 2D array representing points
	int count[HEATMAP_H][HEATMAP_V] = {0};

	//TODO Initialize array of heatmap shades
	int shades[HM_SHADES];
	shades[0] = 1;
	shades[1] = 0;
	shades[2] = 2;
	shades[3] = 7;
	shades[4] = 4;
	shades[5] = 6;
	shades[6] = 3;
	shades[7] = 5;

	//Check where points land
	int i;
	for (i = 0; i < numPoints; i++) {
		int xi, yi;
		for (yi = 0; yi < HEATMAP_V; yi++) {
			for (xi = 0; xi < HEATMAP_H; xi++) {
				if (points[i].x < ((xi+1) * XRES/HEATMAP_H) && points[i].x >= (xi * XRES/HEATMAP_H) &&
				 	points[i].y < ((yi+1) * MENU_TOP/HEATMAP_V) && points[i].y >= (yi * MENU_TOP/HEATMAP_V)) {
					count[xi][yi]++;
					printf("Point landed in (%d, %d), count now %d\n", xi, yi, count[xi][yi]);
					break;
				}
			}
		}
	}

	//Find max and min counts
	int x, y;
	int max_count = 0;
	int min_count = numPoints;
	for (y = 0; y < HEATMAP_V; y++) {
		for (x = 0; x < HEATMAP_H; x++) {
			if (count[x][y] > max_count) {
				max_count = count[x][y];
			}
			if (count[x][y] < min_count) {
				min_count = count[x][y];
			}
		}
	}
	printf("Minimum count is %d\n", min_count);
	printf("Maximum count is %d\n", max_count);

	//Make colours proportional to number of points
	int shade;
	int colours[HEATMAP_H][HEATMAP_V];
	for (y = 0; y < HEATMAP_V; y++) {
		for (x = 0; x < HEATMAP_H; x++) {
			shade = ((count[x][y] - min_count) * (HM_SHADES - 1))/(max_count - min_count);
			colours[x][y] = shades[shade];
			printf("Block (%d, %d) set to shade %d.\n", x, y, shade);
		}
	}

	//Draw points
	int h, v;
	for (v = 0; v < HEATMAP_V; v++) {
		for (h = 0; h < HEATMAP_H; h++) {
			WriteFilledRectangle(h * (XRES-1)/HEATMAP_H, v * MENU_TOP/HEATMAP_V, (h + 1) * (XRES-1)/HEATMAP_H, (v + 1) * MENU_TOP/HEATMAP_V, colours[h][v]);
			printf("Drew (%i, %i) with colour %i\n", h, v, colours[h][v]);
		}
	}
	printf("Heatmap drawn.\n");
}
