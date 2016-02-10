/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "conversion.h"
#include "touchScreen.h"
#include "graphics.h"
#include "bluetooth.h"
#include "gps_points.h"
#include "sd_card.h"

// Screen divisions.
#define MENU_TOP 400
#define GOAL_WIDTH 320

// Drawing heatmap
#define HEATMAP_H 5
#define HEATMAP_V 3
#define INIT_COLOUR 0x00006400

// Meta stuff.
void initialize(void);
void cleanup(void);

// Drawing functions.
void draw_field(void);
void draw_data(GPSPoint points[], int numPoints);
void draw_menu(void);

// Main menu function, should return struct point eventually, but void for now
void main_menu(void);
// Sub-menu function, should return struct point eventually, but void for now
void sub_menu(void);

int main()
{
  printf("Starting module 1 code.\n");

  initialize();
  main_menu();

  // Should never reach this point, but here in case we implement an exit button.
  cleanup();
  printf("Program terminated.\n");

  return 0;
}

void initialize(void)
{
	init_gps();
	printf("GPS initialized.\n");
	Init_Touch();
	printf("Touchscreen initialized.\n");
	init_btport();
	printf("Bluetooth port initialized.\n");
	clear_screen(WHITE);
	printf("Screen cleared.\n");
}

void cleanup(void)
{
	//Nothing yet
}

void draw_field(void)
{
	//Centre circle
	Circle(XRES/2, YRES/2, YRES/8, BLACK);
	Circle(XRES/2, YRES/2, YRES/8 - 1, WHITE);
	//Middle line
	WriteVLine(XRES/2, 0, YRES-1, BLACK);
	//Goals
	WriteFilledRectangle(0, YRES/4, GOAL_WIDTH, 3*YRES/4, BLACK);
	WriteFilledRectangle(0, YRES/4 + 1, GOAL_WIDTH - 1, 3*YRES/4 - 1, WHITE);
	WriteFilledRectangle(XRES - GOAL_WIDTH, YRES/4, XRES, 3*YRES/4, BLACK);
	WriteFilledRectangle(XRES - GOAL_WIDTH + 1, YRES/4 + 1, XRES, 3*YRES/4 - 1, WHITE);
}

void draw_data(GPSPoint points[], int numPoints)
{
	//Initialize 2D array representing points
	int count[HEATMAP_H][HEATMAP_V];
	int x, y;
	for (y = 0; y < HEATMAP_V; y++) {
		for (x = 0; x < HEATMAP_H; x++) {
			count[x][y] = INIT_COLOUR;
		}
	}

	//Check where points land
	int i;
	for (i = 0; i < numPoints; i++) {
		int xi, yi;
		for (yi = 0; yi < HEATMAP_V; yi++) {
			for (xi = 0; xi < HEATMAP_H; xi++) {
				if (points[i].x < ((xi+1) * XRES/HEATMAP_H) && points[i].x >= (xi * XRES/HEATMAP_H) &&
				 	points[i].y < ((yi+1) * YRES/HEATMAP_V) && points[i].y >= (yi * YRES/HEATMAP_V)) {
					break;
				}
			}
		}
		count[xi][yi]++;
	}

	//TODO make colour drawing proportional rather than absolute

	int h, v;
	for (v = 0; v < HEATMAP_V; v++) {
		for (h = 0; h < HEATMAP_H; h++) {
			WriteFilledRectangle(h * XRES/HEATMAP_H, v * YRES/HEATMAP_V, (h + 1) * XRES/HEATMAP_H, (v + 1) * YRES/HEATMAP_V, count[h][v]);
		}
	}
}

void draw_menu(void)
{
	WriteFilledRectangle(0, MENU_TOP, XRES-1, YRES-1, WHITE);
	WriteHLine(0, MENU_TOP, YRES, BLACK);
	Text(0, MENU_TOP, BLACK, WHITE, "Tap for more information", 0);
}
void main_menu(void)
{
	clear_screen(WHITE);
	//draw_data(<GPSPoint array or something>, 10); TODO how did this get here i am not good with computer
	draw_field();
	draw_menu();
	Text(0, 0, BLACK, WHITE, "Main Menu", 0);
	while(1)
	{
		//Wait for touch
		//Based on touch coords:
		//Open submenu
		//On return from submenu, clear screen and redraw the field
	}
}

void sub_menu(void)
{
	clear_screen(WHITE);
	//TODO draw submenu stuff here
	draw_menu();
	Text(0, 0, BLACK, WHITE, "Additional Info", 0);
	while(1)
	{
		//Wait for touch
		//Based on touch coords:
		//Exit submenu (break)
	}
}
