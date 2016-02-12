/*
 * sub_menus.h
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#ifndef SUB_MENUS_H_
#define SUB_MENUS_H_

#include "touchScreen.h"

typedef struct{
	int menuBackground;
	int text;
	int connectTheDotsLine;
}Colours;

void SaveLoadMenu(Point*);

void InterpretMenu(Point*);

void SettingsMenu(Point* p, Colours* scheme);

#endif /* SUB_MENUS_H_ */
