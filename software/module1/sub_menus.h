/*
 * sub_menus.h
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#ifndef SUB_MENUS_H_
#define SUB_MENUS_H_

#include "touchScreen.h"
#define NPAIRS 3 //Number of text/background colours
#define INITPAIR 0 //Should stay 0
#define BUFFER_BTW_BUTTONS 50 //Amount of black pixels between buttons

//Struct for colour set
typedef struct{
	int menuBackground;
	int text;
	int connectTheDotsLine;
	int pairNum;
}Colours;

void SaveLoadMenu(Point*);

void InterpretMenu(Point*);

void SettingsMenu(Point*, Colours*);

#endif /* SUB_MENUS_H_ */
