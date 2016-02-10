/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "graphics.h"
#include "bluetooth.h"
#include "gps.h"
#include "gps_points.h"
#include "sd_card.h"

// Screen divisions.
#define MENU_TOP 400

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
	init_btport();
}

void cleanup(void)
{
	//Nothing yet
}

void draw_field(void)
{
	//Draw middle line
	WriteVLine(XRES/2, 0, YRES-1, BLACK);
	//TODO Draw circle
	//TODO Draw goal boxes
}

void draw_data(void) // TODO take whatever data structure Tim decides to use (array of ints)
{
	//TODO draw heatmap
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
	draw_field();
	draw_data();
	draw_menu();
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
	while(1)
	{
		//Wait for touch
		//Based on touch coords:
		//Exit submenu (break)
	}
}
