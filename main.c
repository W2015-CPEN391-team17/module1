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

// Initialize all components.
void initialize(void);

// Drawing functions.
void clear_screen(void); // TODO overload this function to clear a box
void draw_field(void);
void draw_data(void);

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

void clear_screen(void)
{
	//TODO
}

void draw_field(void)
{
	clear_screen();
	//TODO
}

void draw_data(void)
{
	//TODO
}

void main_menu(void)
{
	draw_field();
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
	while(1)
	{
		//Wait for touch
		//Based on touch coords:
		//Exit submenu (break)
	}
}
