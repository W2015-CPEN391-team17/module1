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
	Circle(XRES/2, (YRES - (YRES-MENU_TOP))/2, (YRES - (YRES-MENU_TOP))/8, BLACK);
	//Middle line
	WriteVLine(XRES/2, 0, MENU_TOP-YRES-1, BLACK);
	//Goals
	//WriteFilledRectangle(0, YRES/4, GOAL_WIDTH, 3*YRES/4, BLACK);
	//WriteFilledRectangle(0, YRES/4 + 1, GOAL_WIDTH - 1, 3*YRES/4 - 1, WHITE);
	//WriteFilledRectangle(XRES - GOAL_WIDTH, YRES/4, XRES - 1, 3*YRES/4, BLACK);
	//WriteFilledRectangle(XRES - GOAL_WIDTH + 1, YRES/4 + 1, XRES - 1, 3*YRES/4 - 1, WHITE);
	printf("Field drawn.\n");
}

void draw_data(GPSPoint points[], int numPoints)
{
	//Initialize 2D array representing points
	int count[HEATMAP_H][HEATMAP_V];
	int x, y;
	for (y = 0; y < HEATMAP_V; y++) {
		for (x = 0; x < HEATMAP_H; x++) {
			count[x][y] = INIT_COLOUR;//0;
		}
	}

	//Check where points land
	int i;
	for (i = 0; i < numPoints; i++) {
		int xi, yi;
		for (yi = 0; yi < HEATMAP_V; yi++) {
			for (xi = 0; xi < HEATMAP_H; xi++) {
				if (points[i].x < ((xi+1) * XRES/HEATMAP_H) && points[i].x >= (xi * XRES/HEATMAP_H) &&
				 	points[i].y < ((yi+1) * (YRES - (YRES-MENU_TOP))/HEATMAP_V) && points[i].y >= (yi * (YRES - (YRES-MENU_TOP))/HEATMAP_V)) {
					printf("Point landed in (%i, %i)\n", xi, yi);
					break;
				}
			}
		}
		count[xi][yi]++;
	}

	//Make colours proportional to number of points
	int colours[HEATMAP_H][HEATMAP_V];
	//TODO actually do what the above comment says this does

	//Draw points
	int h, v;
	for (v = 0; v < HEATMAP_V; v++) {
		for (h = 0; h < HEATMAP_H; h++) {
			WriteFilledRectangle(h * (XRES-1)/HEATMAP_H, v * (YRES - (YRES-MENU_TOP))/HEATMAP_V, (h + 1) * (XRES-1)/HEATMAP_H, (v + 1) * (YRES - (YRES-MENU_TOP))/HEATMAP_V, count[h][v]);
		}
	}
	printf("Heatmap drawn.\n");
}

void draw_menu(void)
{
	WriteFilledRectangle(0, MENU_TOP, XRES-1, YRES-1, WHITE);
	WriteHLine(0, MENU_TOP, XRES - 1, LIME);
	WriteVLine(XRES/3, MENU_TOP, YRES - MENU_TOP - 1, BLACK);
	WriteVLine(XRES*2/3, MENU_TOP, YRES - MENU_TOP - 1, BLACK);
	Text(10, (MENU_TOP + YRES)/2, BLACK, WHITE, "Save", 0);
	Text(XRES/3 + 10, (MENU_TOP + YRES)/2, BLACK, WHITE, "Interpret", 0);
	Text(XRES*2/3 + 10, (MENU_TOP + YRES)/2, BLACK, WHITE, "Settings", 0);
	printf("Menu drawn.\n");
}
void main_menu(void)
{
	clear_screen(WHITE);

	//Fake GPS data points for testing
	GPSPoint p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	p0.x = 0; p0.y = 0;
	p1.x = 100; p1.y = 50;
	p2.x = 200; p2.y = 100;
	p3.x = 300; p3.y = 150;
	p4.x = 400; p4.y = 200;
	p5.x = 500; p5.y = 250;
	p6.x = 600; p6.y = 300;
	p7.x = 700; p7.y = 350;
	p8.x = 799; p8.y = 400;
	p9.x = 799; p9.y = 479;
	GPSPoint fake[10];
	fake[0] = p0;
	fake[1] = p1;
	fake[2] = p2;
	fake[3] = p3;
	fake[4] = p4;
	fake[5] = p5;
	fake[6] = p6;
	fake[7] = p7;
	fake[8] = p8;
	fake[9] = p9;

	//draw_data(fake, 10);
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
