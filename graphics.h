/*
 * graphics.h
 *
 *  Created on: Jan 26, 2016
 *      Author: nick
 */

#ifndef EXERCISE1_7_H_
#define EXERCISE1_7_H_

#define TRUE 1
#define FALSE 0

#define XRES 800
#define YRES 480

// graphics registers all address begin with '8' so as to by pass data cache on NIOS

#define GraphicsCommandReg   		(*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg   		(*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg   			(*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg   			(*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg   			(*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg   			(*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg   		(*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackGroundColourReg   	(*(volatile unsigned short int *)(0x84000010))

/************************************************************************************************
** This macro pauses until the graphics chip status register indicates that it is idle
***********************************************************************************************/

#define WAIT_FOR_GRAPHICS		while((GraphicsStatusReg & 0x0001) != 0x0001);

//Here are some software routines to drive some simple graphics functions

// #defined constants representing values we write to the graphics 'command' register to get
// it to draw something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these

#define DrawHLine				1
#define DrawVLine				2
#define DrawLine				3
#define	PutAPixel				0xA
#define	GetAPixel				0xB
#define	ProgramPaletteColour    0x10

// defined constants representing colours pre-programmed into colour palette
// there are 256 colours but only 8 are shown below, we write these to the colour registers
//
// the header files "colours.h" contains constants for all 256 colours
// while the course file ColourPaletteData.c contains the 24 bit RGB data
// that is pre-programmed into the palette

#define	BLACK			0
#define	WHITE			1
#define	RED				2
#define	LIME			3
#define	BLUE			4
#define	YELLOW			5
#define	CYAN			6
#define	MAGENTA			7

void WriteAPixel(int x, int y, int Colour);
void WriteHLine(int x1, int y1, int length, int Colour);
void WriteVLine(int x1, int y1, int length, int Colour);
int ReadAPixel(int x, int y);
void ProgramPalette(int PaletteNumber, int RGB);
void HLine(int x1, int y1, int length, int Colour);
void VLine(int x1, int y1, int length, int Colour);

int abs(int a);
int sign(int a);
void Line(int x1, int y1, int x2, int y2, int Colour);

#endif /* EXERCISE1_7_H_ */
