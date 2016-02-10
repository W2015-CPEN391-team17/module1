/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "graphics.h"
#include "bluetooth.h"
#include "gps_points.h"

// Screen divisions.
#define MENU_TOP 400
#define GOAL_WIDTH 320

// Meta stuff.
void initialize(void);
void cleanup(void);

// Save and load.
// void save(<array of points>);
// <array of points> load();

// Conversion functions.
// <array of points> gps_to_points(array of points); // Mutates the gps data to the screen.

// Drawing functions.
void draw_field(void);
void draw_data(void);
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

void draw_data(void) // TODO take whatever data structure Tim decides to use (array of ints)
{
	//TODO draw heatmap
	//Don't draw it too small
	//Take advantage of hardware rectangles
}

void draw_menu(void)
{
	WriteFilledRectangle(0, MENU_TOP, XRES-1, YRES-1, WHITE);
	WriteHLine(0, MENU_TOP, YRES, BLACK);
	//TODO draw text here
}
void main_menu(void)
{
	clear_screen(WHITE);
	draw_data();
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
