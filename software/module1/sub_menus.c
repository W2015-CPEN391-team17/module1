/*
 * sub_menus.c
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#include "sub_menus.h"
#include "graphics.h"
#include "touchScreen.h"

//Struct for background/text colour pairs - only a limited selection are available to the user
typedef struct{
	int background;
	int text;
}back_textPair;

double percentageLeft, percentageRight, percentageForward, percentageBack;

void initInterpret(int count[HEATMAP_H][HEATMAP_V], int nPoints){
	int backCount = 0;
	int leftCount = 0;

	int i;
	for(i = 0; i < HEATMAP_H; i++){
		int j;
		for(j = 0; j < HEATMAP_V; j++){
			if(i <= HEATMAP_H / 2){
				backCount += count[i][j];
			}

			if(j <= HEATMAP_V / 2){
				leftCount += count[i][j];
			}
		}
	}

	percentageBack = (double)backCount / nPoints;
	percentageLeft = (double)leftCount / nPoints;

	percentageForward = 1 - percentageBack;
	percentageRight = 1 - percentageLeft;
}

void SaveLoadMenu(Point* p){
	*p = GetPress();
}

//CHANGE MAIN
void InterpretMenu(Point* p, Colours* scheme){
	WriteFilledRectangle(0, 0, XRES-1, MENU_TOP-1, scheme->menuBackground);

	char* str = "Time on Right:   %";
	str[15] = (char)(100 * percentageRight / 10 + '0');
	str[16] = (char)((int)(100 * percentageRight) % 10 + '0');

	if(str[15] == '0'){
		str[15] = str[16];
		str[16] = '%';
		str[17] = '\0';
	}

	Text(XRES/5, 10, scheme->text, scheme->menuBackground, str, 0);

	str = "Time on Left:   %";

	str[14] = (char)(100 * percentageLeft / 10 + '0');
	str[15] = (char)((int)(100 * percentageLeft) % 10 + '0');

	if(str[14] == '0'){
		str[14] = str[16];
		str[15] = '%';
		str[16] = '\0';
	}

	Text(XRES/5, 210, scheme->text, scheme->menuBackground, str, 0);

	str = "Time Forward:   %";

	str[14] = (char)(100 * percentageForward / 10 + '0');
	str[15] = (char)((int)(100 * percentageForward) % 10 + '0');

	if(str[14] == '0'){
		str[14] = str[16];
		str[15] = '%';
		str[16] = '\0';
	}

	Text(3*XRES/5, 10, scheme->text, scheme->menuBackground, str, 0);

	str = "Time Back:   %";

	str[11] = (char)(100 * percentageForward / 10 + '0');
	str[12] = (char)((int)(100 * percentageBack) % 10 + '0');

	if(str[11] == '0'){
		str[11] = str[16];
		str[12] = '%';
		str[13] = '\0';
	}

	Text(3*XRES/5, 210, scheme->text, scheme->menuBackground, str, 0);

	*p = GetPress();
}

/*
 * Allows user to change the menu background colours as well as text colour and line (for connect-the-dots) colour.
 * Makes sure the text/background pair is acceptable (text is viewable on background).
 * Makes sure the line colour is not white and is not the same as the background colour.
 */
void SettingsMenu(Point* p, Colours* scheme){
	back_textPair pairs[NPAIRS]; //Initialise pairs of background/text colours
	pairs[INITPAIR].background = WHITE;
	pairs[INITPAIR].text = BLACK;
	pairs[1].background = CYAN;
	pairs[1].text = RED;
	pairs[2].background = YELLOW;
	pairs[2].text = BLUE;

	WriteFilledRectangle(0, 0, XRES-1, MENU_TOP-1, BLACK);//Clear background

	do{
		WriteFilledRectangle(XRES/2-100, 0, XRES/2+100, MENU_TOP/2 - BUFFER_BTW_BUTTONS,scheme->menuBackground);//Create buttons
		WriteFilledRectangle(XRES/2-100, MENU_TOP/2, XRES/2+100, MENU_TOP - BUFFER_BTW_BUTTONS,scheme->menuBackground);

		Text(XRES/2-100+10, (MENU_TOP/2 - BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, "Text/background", 0);//Print text on buttons
		Text(XRES/2-100+10, MENU_TOP/2 + 1, scheme->text, scheme->menuBackground, "Line Colour", 0);

		//Display example lines
		WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS-1, XRES/2+100, MENU_TOP/2-1, scheme->connectTheDotsLine);
		WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS, XRES/2+100, MENU_TOP/2, scheme->connectTheDotsLine);
		WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS+1, XRES/2+100, MENU_TOP/2+1, scheme->connectTheDotsLine);

		*p = GetPress();

		//If the text/background colour button is pressed
		if(p->x <= XRES/2+100 && p->x >= XRES/2-100 && p->y <= MENU_TOP/2-BUFFER_BTW_BUTTONS){
			do{
				scheme->menuBackground = pairs[(scheme->pairNum + 1) % NPAIRS].background;
				scheme->text = pairs[(scheme->pairNum + 1) % NPAIRS].text;
				scheme->pairNum = (scheme->pairNum + 1) % NPAIRS;
			}while(scheme->menuBackground == scheme->connectTheDotsLine);//Change the colour pair if the background was the same colour as the line
			GetRelease();
		}else if(p->x <= XRES/2+100 && p->x >= XRES/2-100 && p->y <= MENU_TOP-BUFFER_BTW_BUTTONS && p->y > MENU_TOP/2){ //If the line colour button was pressed
			do{
				scheme->connectTheDotsLine = (scheme->connectTheDotsLine + 1) % NCOLOURS;
			}while(scheme->connectTheDotsLine == WHITE || scheme->connectTheDotsLine == scheme->menuBackground);//Change the line colour if it is white or is the same colour as the background
			GetRelease();
		}else if(!(p->x >= 2 * XRES / 3 && p->y >= MENU_TOP)){//If touched outside of this menu's buttons and not the settings button itself break out of menu
			break;
		}
	}while(1); //Do this while in settings menu

}
