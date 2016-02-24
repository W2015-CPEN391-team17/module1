/*
 * sub_menus.h
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#ifndef SUB_MENUS_H_
#define SUB_MENUS_H_

#include "touchScreen.h"
#include "graphics.h"

#define NPAIRS 3 //Number of text/background colours
#define INITPAIR 0 //Should stay 0 - is the initial text/background pair
#define BUFFER_BTW_BUTTONS 50 //Amount of black pixels between buttons

void initInterpret(int count[HEATMAP_H][HEATMAP_V], int nPoints);

void SaveLoadMenu(Point*, Colours*);

void InterpretMenu(Point*, Colours*);

void SettingsMenu(Point*, Colours*);

#endif /* SUB_MENUS_H_ */
