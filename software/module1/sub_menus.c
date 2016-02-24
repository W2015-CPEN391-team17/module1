/*
 * sub_menus.c
 *
 *  Created on: 2016-02-11
 *      Author: Kyle
 */

#include "sub_menus.h"
#include "graphics.h"
#include "touchScreen.h"
#include "datasets.h"
#include <string.h>

//Struct for background/text colour pairs - only a limited selection are available to the user
typedef struct{
	int background;
	int text;
}back_textPair;

//Variables for the interpret menu
double percentageLeft, percentageRight, percentageForward, percentageBack;

/*
 * Initialises the interpret menu, must be called each time the heat map data is changed. Saves doing computations every time
 * the interpret menu is brought up.
 */
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

/*
 * Displays the Save/Load menu.
 */
void SaveLoadMenu(Point* p, Colours* scheme){
	WriteFilledRectangle(0, 0, XRES-1, MENU_TOP-1, WHITE);

	//If the background is WHITE then just draw boarders in black for buttons
	if(scheme->menuBackground == WHITE){
		Rectangle(XRES/5, YRES/4, 100 + XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, BLACK);
		Rectangle(4*XRES/5 - 100, YRES/4,  4*XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, BLACK);
		Rectangle(XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS, 100 + XRES/5, 50 + YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + BUFFER_BTW_BUTTONS, BLACK);
		Rectangle(4*XRES/5-100, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS, 4*XRES/5, 50 + YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + BUFFER_BTW_BUTTONS, BLACK);
	}else{ //Else use scheme to determine button colour
		WriteFilledRectangle(XRES/5, YRES/4, 100 + XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, scheme->menuBackground);
		WriteFilledRectangle(4*XRES/5 - 100, YRES/4,  4*XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, scheme->menuBackground);
		WriteFilledRectangle(XRES/5, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS, 100 + XRES/5, 50 + YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + BUFFER_BTW_BUTTONS, scheme->menuBackground);
		WriteFilledRectangle(4*XRES/5-100, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS, 4*XRES/5, 50 + YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + BUFFER_BTW_BUTTONS, scheme->menuBackground);
	}

	//Draw text in buttons explaining them
	Text((XRES/5 + (5*BUFFER_BTW_BUTTONS/4) + XRES/5)/2, (YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + YRES/4)/2-5, scheme->text, scheme->menuBackground, "<", 0);
	Text((4*XRES/5 + 4*XRES/5 - (5*BUFFER_BTW_BUTTONS/4))/2, (YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + YRES/4)/2-5, scheme->text, scheme->menuBackground, ">", 0);
	Text(XRES/5 + 10, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS + 10, scheme->text, scheme->menuBackground, "Load", 0);
	Text(4*XRES/5-100 + 10, 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS + 10, scheme->text, scheme->menuBackground, "Save", 0);

	int set = 0;//0 == GPS, else dataSet = set - 1.
	char* str = "DataSet   ";
	Text(XRES/2-25, (YRES/2 + BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, "GPS", 0);

	//boolean
	int buttonTouched = 0;
	do{
		if(buttonTouched){
			buttonTouched = 0;
		}

		*p = GetPress();

		if(p->y > YRES/4 && p->y < 5*BUFFER_BTW_BUTTONS/4 + YRES/4){
			//if decrement button touched, decrement set, display such has been done to user
			if(p->x > XRES/5 && p->x < 100 + XRES/5){
				buttonTouched = 1;
				WriteFilledRectangle(100 + XRES/5 + 1, YRES/4, 4*XRES/5 - 100 - 1, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, WHITE);

				if(set == 0){
					set = MAX_N_SETS;//wrap-around
				}else{
					set--;
				}

				if(set == 0){
					Text(XRES/2-25, (YRES/2 + BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, "GPS", 0);
				}else{
					str[8] = (char)(set / 10) + '0';
					str[9] = (char)(set % 10) + '0';
					if(str[8] == '0'){
						str[8] = str[9];
						str[9] = '\0';
					}

					Text(XRES/2-45, (YRES/2 + BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, str, 0);
				}

				GetRelease();
			}else if(p->x > 4*XRES/5 - 100 && p->x < 4*XRES/5){
				//if increment button touched, increment set, display such has been done to user
				buttonTouched = 1;
				WriteFilledRectangle(100 + XRES/5 + 1, YRES/4, 4*XRES/5 - 100 - 1, 5*BUFFER_BTW_BUTTONS/4 + YRES/4, WHITE);

				if(set == MAX_N_SETS){
					set = 0;//wrap-around
				}else{
					set++;
				}

				if(set == 0){
					Text(XRES/2-25, (YRES/2 + BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, "GPS", 0);
				}else{
					str[8] = (char)(set / 10) + '0';
					str[9] = (char)(set % 10) + '0';
					if(str[8] == '0'){
						str[8] = str[9];
						str[9] = '\0';
					}

					Text(XRES/2-45, (YRES/2 + BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, str, 0);
				}

				GetRelease();
			}
		}else if(p->y < 50 + YRES/4 + 5*BUFFER_BTW_BUTTONS/4 + BUFFER_BTW_BUTTONS && p->y > 5*BUFFER_BTW_BUTTONS/4 + YRES/4 + BUFFER_BTW_BUTTONS){
			//If load button touched, do following
			if(p->x > XRES/5 && p->x < 100 + XRES/5){
				buttonTouched = 1;

				if (set != 0) {
					//GPS is set 0 so we add 1 to get the correct index
					load_into_workingDataSet(set+1);
				} else {
					loadgps_workingDataSet();
				}
				
				WriteFilledRectangle(XRES/4, 0, XRES-1, YRES/4-1, WHITE);
				if(set == 0){
					Text(XRES/3-45, 10, BLACK, WHITE, "GPS Loaded", 0);
				}else{
					char tmpStr[strlen(str)+8];
					strcpy(tmpStr,str);
					strcpy(tmpStr+strlen(str), " Loaded");

					Text(XRES/3-45, 10, BLACK, WHITE, tmpStr, 0);
				}

				GetRelease();
			}else if(p->x > 4*XRES/5-100 && p->x < 4*XRES/5){
				//If save button touched, do following
				buttonTouched = 1;
		
				if (set != 0) { // check set number since cannot save to GPS
					//GPS is set 0 so we add 1 to get the correct index
					save_from_workingDataSet(set+1);
				}

				WriteFilledRectangle(XRES/4, 0, XRES-1, YRES/4-1, WHITE);
				if(set == 0){
					Text(XRES/3-45, 10, BLACK, WHITE, "Cannot save to GPS", 0);
					}else{
					char tmpStr[strlen(str)+26];
					strcpy(tmpStr, "Current DataSet saved to ");
					strcpy(tmpStr+25,str);

					Text(XRES/3-45, 10, BLACK, WHITE, tmpStr, 0);
				}

				GetRelease();
			}
		}

	}while(buttonTouched || (p->y > MENU_TOP && p->x < XRES/3)); //Do this while a button in the submenu has been touched or the Save/Load menu button has been touched
}

/*
 * Displays the interpret menu
 */
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
		str[14] = str[15];
		str[15] = '%';
		str[16] = '\0';
	}

	Text(XRES/5, 210, scheme->text, scheme->menuBackground, str, 0);

	str = "Time Forward:   %";

	str[14] = (char)(100 * percentageForward / 10 + '0');
	str[15] = (char)((int)(100 * percentageForward) % 10 + '0');

	if(str[14] == '0'){
		str[14] = str[15];
		str[15] = '%';
		str[16] = '\0';
	}

	Text(3*XRES/5, 10, scheme->text, scheme->menuBackground, str, 0);

	str = "Time Back:   %";

	str[11] = (char)(100 * percentageBack / 10 + '0');
	str[12] = (char)((int)(100 * percentageBack) % 10 + '0');

	if(str[11] == '0'){
		str[11] = str[12];
		str[12] = '%';
		str[13] = '\0';
	}

	Text(3*XRES/5, 210, scheme->text, scheme->menuBackground, str, 0);

	do{
		*p = GetPress();
	}while(p->x > XRES/3 && p->x < 2*XRES/3 && p->y > MENU_TOP);
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

	int settingsTouched = 0;
	do{
		//Redraw buttons/lines with new colours if the button pressed wasn't the settings button
		if(!settingsTouched){
			WriteFilledRectangle(XRES/2-100, 0, XRES/2+100, MENU_TOP/2 - BUFFER_BTW_BUTTONS,scheme->menuBackground);//Create buttons
			WriteFilledRectangle(XRES/2-100, MENU_TOP/2, XRES/2+100, MENU_TOP - BUFFER_BTW_BUTTONS,scheme->menuBackground);

			Text(XRES/2-100+10, (MENU_TOP/2 - BUFFER_BTW_BUTTONS)/2, scheme->text, scheme->menuBackground, "Text/background", 0);//Print text on buttons
			Text(XRES/2-100+10, MENU_TOP/2 + 1, scheme->text, scheme->menuBackground, "Line Colour", 0);

			//Display example lines
			WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS-1, XRES/2+100, MENU_TOP/2-1, scheme->connectTheDotsLine);
			WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS, XRES/2+100, MENU_TOP/2, scheme->connectTheDotsLine);
			WriteLine(XRES/2-100, MENU_TOP - BUFFER_BTW_BUTTONS+1, XRES/2+100, MENU_TOP/2+1, scheme->connectTheDotsLine);
		}else{
			settingsTouched = 0;
		}

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
		}else{//Else the settings button was touched
			settingsTouched = 1;
		}
	}while(1); //Do this while in settings menu

}
