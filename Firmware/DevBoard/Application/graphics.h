/*
Multifont GFX library is adapted from Adafruit_GFX library by Paul Kourany

Please read README.pdf for details
*/

//  graphics.h

#ifndef _graphics_h
#define _graphics_h

/*********************************************************************
 * INCLUDES
 */

#include <xdc/std.h>

#include "Display.h"

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * FUNCTIONS
 */

void drawCircle(int16_t x0, int16_t y0, int16_t r, Display_Colour color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, Display_Colour color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Display_Colour color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Display_Colour color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Display_Colour color);
void fillScreen(Display_Colour color);
void DisplayPrint(char* string);
void setCursor(int16_t x, int16_t y);
void setTextSize(uint8_t s);
void setTextColor(Display_Colour c);

#endif
