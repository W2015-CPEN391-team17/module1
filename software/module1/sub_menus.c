/*
 * sub_menus.c
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#include "sub_menus.h"
#include "graphics.h"
#include "touchScreen.h"

typedef struct{
	int background;
	int text;
}back_textPair;


void SaveLoadMenu(Point* p){

}

void InterpretMenu(Point* p){

}

/*Settings:
 * Menu colour set
 * Line colour set
 * Text colour set ---> colour scheme b/tw Menu colour + text colour
 */
void SettingsMenu(Point* p, Colours* scheme){
	back_textPair pairs[NPAIRS];
	pairs[INITPAIR].background = WHITE;
	pairs[INITPAIR].text = BLACK;
	pairs[1].background = CYAN;
	pairs[1].text = RED;
	pairs[2].background = YELLOW;
	pairs[2].text = BLUE;

	WriteFilledRectangle(0, 0, XRES-1, MENU_TOP-1, BLACK);

	do{
		WriteFilledRectangle(XRES/2-100, 0, XRES/2+100, MENU_TOP/2 - BUFFER_BTW_BUTTONS,scheme->menuBackground);
		WriteFilledRectangle(XRES/2-100, MENU_TOP/2, XRES/2+100, MENU_TOP - BUFFER_BTW_BUTTONS,scheme->menuBackground);

		Text(XRES/2-100+10, (MENU_TOP/2 - 50)/2, scheme->text, scheme->menuBackground, "Text/background", 0);

		*p = GetPress();

		if(p->x <= XRES/2+100 && p->x >= XRES/2-100 && p->y <= MENU_TOP/2-BUFFER_BTW_BUTTONS){
			scheme->menuBackground = pairs[(scheme->pairNum + 1) % NPAIRS].background;
			scheme->text = pairs[(scheme->pairNum + 1) % NPAIRS].text;
			scheme->pairNum = (scheme->pairNum + 1) % NPAIRS;
			GetRelease();
		}else if(p->x <= XRES/2+100 && p->x >= XRES/2-100 && p->y <= MENU_TOP-BUFFER_BTW_BUTTONS && p->y > MENU_TOP/2){
			//Do stuff if other button pressed
			GetRelease();
		}else{
			break;
		}
	}while(1);

}
