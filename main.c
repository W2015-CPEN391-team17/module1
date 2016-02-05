/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "graphics.h"
#include "bluetooth.h"
#include "gps.h"
#include "gps_points.h"

// Screen divisions.
#define MENU_TOP 400

// Initialize all components.
void initialize(void);

// Drawing functions.
void clear_box(int x1, int y1, int x2, int y2);
void clear_screen(void);
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
  printf("Program terminated.\n");

  return 0;
}

void initialize(void)
{
	init_gps();
	init_btport();
}

void clear_box(int x1, int y1, int x2, int y2)
{
	int i;
	for (i = y1; i<y2; i++)
	{
		WriteHLine(x1, i, x2, WHITE);
	}
}

void clear_screen(void)
{
	clear_box(0, 0, XRES, YRES);
}

void draw_field(void)
{
	//Draw middle line
	WriteVLine(XRES/2, 0, YRES, BLACK);
	//TODO Draw circle
	//TODO Draw goal boxes
}

void draw_data(void)
{
	//TODO draw points and lines
}

void draw_menu(void)
{
	clear_box(0, MENU_TOP, XRES, YRES);
	WriteHLine(0, MENU_TOP, YRES, BLACK);
	//TODO draw text here
}
void main_menu(void)
{
	clear_screen();
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
	clear_screen();
	//TODO draw submenu stuff here
	draw_menu();
	while(1)
	{
		//Wait for touch
		//Based on touch coords:
		//Exit submenu (break)
	}
}
