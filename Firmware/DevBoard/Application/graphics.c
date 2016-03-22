/*
Multifont GFX library is adapted from Adafruit_GFX library by Paul Kourany

Please read README.pdf for details
*/

//  graphics.c

/*********************************************************************
 * INCLUDES
 */

/* XDCtools Header files */
#include <xdc/std.h>

#include "graphics.h"
#include "fonts.h"

/*********************************************************************
 * CONSTANTS
 */

#define swap(a, b) { int16_t t = a; a = b; b = t; }
#define _width DISPLAY_WIDTH
#define _height DISPLAY_HEIGHT
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#define drawPixel(__X__, __Y__, __col__) ( Display_SetPixel(__Y__, __X__, __col__) ) //Map library fucntion to custom implementation

/*********************************************************************
 * LOCAL VARIABLES
 */

int16_t cursor_x = 0, cursor_y = 0;
Display_Colour textcolor = DISPLAY_COLOUR_BLACK, textbgcolor = DISPLAY_COLOUR_WHITE;
int8_t  fontKern = 1;
const uint8_t* fontData = glcdfontBitmaps;
const FontDescriptor* fontDesc = glcdfontDescriptors;
uint8_t textsize = 1,  rotation = 0,  font = GLCDFONT,  fontStart,  fontEnd, wrap = 1;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, Display_Colour color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, Display_Colour color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, Display_Colour color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, Display_Colour color);
void write(uint8_t c);
void drawFastChar(int16_t x, int16_t y, unsigned char c, Display_Colour color, uint16_t bg, uint8_t size);
void drawChar(int16_t x, int16_t y, unsigned char c, Display_Colour color, Display_Colour bg, uint8_t size);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

// Draw a circle outline
void drawCircle(int16_t x0, int16_t y0, int16_t r, Display_Colour color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, Display_Colour color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void fillCircle(int16_t x0, int16_t y0, int16_t r,
		Display_Colour color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, Display_Colour color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
				Display_Colour color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
				Display_Colour color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void drawFastVLine(int16_t x, int16_t y,
				 int16_t h, Display_Colour color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

void drawFastHLine(int16_t x, int16_t y,
				 int16_t w, Display_Colour color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    Display_Colour color) {
  // Update in subclasses if desired!
  int16_t i=x;
  for (; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void fillScreen(Display_Colour color) {
  fillRect(0, 0, _width, _height, color);
}

void write(uint8_t c) {

  if (c == '\n') {
    cursor_y += textsize*fontDesc[0].height;	//all chars are same height so use height of space char
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawFastChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
	uint16_t w = fontDesc[c-fontStart].width;
	uint16_t h = fontDesc[c-fontStart].height;
    if (fontKern > 0 && textcolor != textbgcolor) {
      fillRect(cursor_x+w*textsize,cursor_y,fontKern*textsize,h*textsize,textbgcolor);
    }
    cursor_x += textsize*(w+fontKern);
    if (wrap && (cursor_x > (_width - textsize*w))) {
      cursor_y += textsize*h;
      cursor_x = 0;
    }
  }
}

void DisplayPrint(char* string){
	uint8_t i = 0;
	while (string[i] != '\0'){
		write(string[i]);
		i++;
	}
}

void drawFastChar(int16_t x, int16_t y, unsigned char c,
		Display_Colour color, uint16_t bg, uint8_t size) {
  // Update in subclasses if desired!
  drawChar(x,y,c,color,(Display_Colour)bg,size);
}

// Draw a character
void drawChar(int16_t x, int16_t y, unsigned char c,
		Display_Colour color, Display_Colour bg, uint8_t size) {
	fontStart = pgm_read_byte(fontData+FONT_START);
	fontEnd = pgm_read_byte(fontData+FONT_END);

  if (c < fontStart || c > fontEnd) {
    c = 0;
  }
  else {
    c -= fontStart;
  }

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + (fontDesc[c].width * size) - 1) < 0) || // Clip left
     ((y + (fontDesc[c].height * size) - 1) < 0))   // Clip top
    return;

	uint8_t bitCount=0;
  	uint16_t fontIndex = fontDesc[c].offset + 2; //((fontDesc + c)->offset) + 2;

  int8_t i=0;
  for (; i<fontDesc[c].height; i++ ) {	// i<fontHeight
    uint8_t line;
    int8_t j = 0;
    for (; j<fontDesc[c].width; j++) {			//j<fontWidth
      if (bitCount++%8 == 0) {
        line = pgm_read_byte(fontData+fontIndex++);
      }
      if (line & 0x80) {
        if (size == 1) {// default sizeFast
          drawPixel(x+j, y+i, color);
          }
        else {  // big size
          fillRect(x+(j*size), y+(i*size), size, size, color);
        }
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+j, y+i, bg);
        else {  // big size
          fillRect(x+j*size, y+i*size, size, size, bg);
        }
      }
      line <<= 1;
    }
    bitCount = 0;
  }
}

void setCursor(int16_t x, int16_t y) {//x and y is character index
  uint16_t w = 5;//TODO: remove hardcoded values
  uint16_t h = 8;
  cursor_x = x * w;
  cursor_y = y * h;
}

void setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void setTextColor(Display_Colour c) {
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

//void setTextColor(Display_Colour c, uint16_t b) {
//  textcolor   = c;
//  textbgcolor = b;
//}

/*********************************************************************
*********************************************************************/
