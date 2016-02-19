/*
 * Entry point for module 1.
 *
 */

#include <stdio.h>
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "conversion.h"
#include "touchScreen.h"
#include "graphics.h"
#include "heatmap.h"
#include "bluetooth.h"
#include "gps_points.h"
#include "sub_menus.h"
#include "sd_card.h"

// Meta stuff.
void initialize(void);
void cleanup(void);

// Drawing functions.
void draw_field(void);
void draw_data(GPSPoint points[], int numPoints);
void draw_menu(void);
void write_demo_screen(void);
void connect_points(GPSPoint points[], int numPoints);

// Main menu function
void main_menu(void);

Colours colorScheme;

#define GPSPOINTLEN 2
#define GPSPOINTSETLEN 2

int main()
{
  printf("Starting module 1 code.\n");
  //write_demo_screen();

  initialize();

  // Test writing and reading points
  GPSPoint points0[GPSPOINTLEN];
  GPSPoint p0;
  p0.x = 1.0;
  p0.y = 2.0;
  GPSPoint p1;
  p1.x = 3.0;
  p1.y = 4.0;
  points0[0] = p0;
  points0[1] = p1;
  GPSPoint points1[GPSPOINTLEN];
  GPSPoint p2;
  p2.x = 5.0;
  p2.y = 6.0;
  points1[0] = p2;
  GPSPointSet set0;
  set0.points[0] = p0;
  set0.points[1] = p1;
  set0.points_len = 2;
  set0.tag = 0;
  GPSPointSet set1;
  set1.points[0] = p2;
  set1.points_len = 1;
  set1.tag = 1;

  GPSPointSet sets[GPSPOINTSETLEN];
  sets[0] = set0;
  sets[1] = set1;

  sd_card_write_GPSPointSets(sets, GPSPOINTSETLEN, "TEST.TXT");
  sd_card_print_contents("TEST.TXT");

  main_menu();

  // Should never reach this point, but here in case we implement an exit button.
  cleanup();

  printf("Program terminated.\n");

  return 0;
}

void initialize(void)
{
	colorScheme.menuBackground = WHITE;
	colorScheme.text = BLACK;
	colorScheme.connectTheDotsLine = BLACK;
	colorScheme.pairNum = INITPAIR;

	init_gps();
	Init_Touch();
	init_btport();
	clear_screen(WHITE);
}

void cleanup(void)
{
	//Nothing yet
}

void draw_field(void)
{
	//Centre circle
	WriteCircle(XRES/2, MENU_TOP/2, MENU_TOP/8, BLACK);
	//Middle line
	WriteVLine(XRES/2, 0, MENU_TOP, BLACK);
	//Goals
	WriteVLine(GOAL_WIDTH, MENU_TOP/4, MENU_TOP/2, BLACK);
	WriteVLine(XRES-GOAL_WIDTH, MENU_TOP/4, MENU_TOP/2, BLACK);
	WriteHLine(0, MENU_TOP/4, GOAL_WIDTH, BLACK);
	WriteHLine(0, 3*MENU_TOP/4, GOAL_WIDTH, BLACK);
	WriteHLine(XRES-GOAL_WIDTH, MENU_TOP/4, GOAL_WIDTH-1, BLACK);
	WriteHLine(XRES-GOAL_WIDTH, 3*MENU_TOP/4, GOAL_WIDTH-1, BLACK);
}

void draw_data(GPSPoint points[], int numPoints)
{
	draw_heatmap(points, numPoints);
	draw_field();
}

void draw_menu(void)
{
	WriteFilledRectangle(0, MENU_TOP, XRES-1, YRES-1, colorScheme.menuBackground);
	WriteHLine(0, MENU_TOP, XRES - 1, BLACK);
	WriteVLine(XRES/3, MENU_TOP, YRES - MENU_TOP - 1, BLACK);
	WriteVLine(XRES*2/3, MENU_TOP, YRES - MENU_TOP - 1, BLACK);
	Text(10, (MENU_TOP + YRES)/2, colorScheme.text, colorScheme.menuBackground, "Save/Load", 0);
	Text(XRES/3 + 10, (MENU_TOP + YRES)/2, colorScheme.text, colorScheme.menuBackground, "Interpret", 0);
	Text(XRES*2/3 + 10, (MENU_TOP + YRES)/2, colorScheme.text, colorScheme.menuBackground, "Settings", 0);
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
	p5.x = 450; p5.y = 150;
	p6.x = 600; p6.y = 300;
	p7.x = 700; p7.y = 350;
	p8.x = 799; p8.y = 400;
	p9.x = 750; p9.y = 390;
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

	draw_data(fake, 10);
	draw_field();
	draw_menu();
	Text(0, 0, BLACK, WHITE, "Main Menu", 0);
	Point p;
	p.y = 0;
	int showing_heatmap = TRUE;
	while(1)
	{
		if(p.y < MENU_TOP){
			p = GetPress();
		}

		if(p.y < MENU_TOP){
			if (showing_heatmap) {
				showing_heatmap = FALSE;
				connect_points(fake, 10);
				GetRelease();
			} else {
				showing_heatmap = TRUE;
				draw_data(fake, 10);
				GetRelease();
			}
		}else{
			if(p.x < XRES / 3){
				//Save/Load touched
				SaveLoadMenu(&p, &colorScheme);
			}else if(p.x < 2 * XRES / 3){
				//Interpret touched
				InterpretMenu(&p, &colorScheme);
			}else{
				//Settings touched
				SettingsMenu(&p, &colorScheme);
				if(p.y < MENU_TOP){
					if (showing_heatmap) {
						draw_data(fake, 10);
					} else {
						connect_points(fake, 10);
					}
				}
				draw_menu();
				GetRelease();
			}
		}
	}
}

void write_demo_screen(void) {
	clear_screen(BLACK);

	int colour = LIME;

	int x = 0;
	for(x = 0; x <= XRES-1; x++) {
		  WriteLine(XRES/8, YRES/8, x, 0, colour);
		  if (colour == LIME) {
			  colour = WHITE;
		  } else {
			  colour = LIME;
		  }
	}
	for(x = XRES-1; x >= 0; x--) {
		  WriteLine(XRES/8, YRES/8, x, YRES-1, colour);
		  if (colour == LIME) {
			  colour = WHITE;
		  } else {
			  colour = LIME;
		  }
	}
	int y = 0;
	for(y = 0; y <= YRES-1; y++) {
		  WriteLine(XRES/8, YRES/8, 0, y, colour);
		  if (colour == LIME) {
			  colour = WHITE;
		  } else {
			  colour = LIME;
		  }
	}
	for(y = YRES-1; y >= 0; y--) {
		  WriteLine(XRES/8, YRES/8, XRES-1, y, colour);
		  if (colour == LIME) {
			  colour = WHITE;
		  } else {
			  colour = LIME;
		  }
	}

	int j = 200;
	int k = 0;
	int n = 23;
	int s = 0;
	colour = 1;
	while(TRUE) {
		int r = 0;
		x = XRES/8;
		y = YRES/8;
		s++;
		for(r = 0; r <= YRES/4; r = r + 5) {
			j = (n + j + 1) % 500;
			n = (n + k*j) % 350;
			k = (k + j + n + r) % 50;
			colour = (colour + n + j + k + r + s) % 8;
			x = x + 10;
			y = y + 10;
			WriteCircle(x, y, r, colour);
		}
	}
}

void connect_points(GPSPoint points[], int numPoints)
{
	WriteFilledRectangle(0,0,XRES-1,MENU_TOP-1,WHITE);
	draw_field();

	int colour = colorScheme.connectTheDotsLine;

	GPSPoint point_a;
	GPSPoint point_b;
	int i;
	for(i = 1; i < numPoints; i++) {
		point_a = points[i-1];
		point_b = points[i];
		// draw a line from point_a to point_b
		WriteLine((int)point_a.x, (int)point_a.y, (int)point_b.x, (int)point_b.y, colour);
	}
}
