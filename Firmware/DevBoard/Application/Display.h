/*
 * DISPLAY.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */

// Display properties
#define DISPLAY_HEIGHT					128
#define DISPLAY_WIDTH					128
#define DISPLAY_BITS_PER_PIXEL			3
#define DISPLAY_BYTES_PER_LINE			DISPLAY_WIDTH*DISPLAY_BITS_PER_PIXEL/8

// SPI definitions
#define DISPLAY_CMD_UPDATE				0x80//0x01	//Command, address, data (48 bytes), dummy (2 bytes or address, dummy, data...)
#define DISPLAY_CMD_NOTHING				0x00	//Command, dummy. Used for COM inversion
#define DISPLAY_CMD_CLEAR				0x20//0x04	//Command, dummy
#define DISPLAY_BM_COM_INVERT			0x02	//Bitmask for COM inversion when EXTMODE = low
#define DISPLAY_BM_CMD					0x07	//Bitmask for command bits
#define DISPLAY_SPI_CALLBACK_ID			0x01

typedef enum{
	DISPLAY_COLOUR_BLACK,
	DISPLAY_COLOUR_RED,
	DISPLAY_COLOUR_GREEN,
	DISPLAY_COLOUR_YELLOW,
	DISPLAY_COLOUR_BLUE,
	DISPLAY_COLOUR_MAGENTA,
	DISPLAY_COLOUR_CYAN,
	DISPLAY_COLOUR_WHITE
}Display_Colour;

//Event IDs
#define DISPLAY_EVENT_PERIODIC	0x01
#define DISPLAY_EVENT_MINUTE	0x02
#define DISPLAY_EVENT_UNKNOWN 	0xFF

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */


void Display_Initialise(); //Configure screen
void Display_Clear(); //Display white
void Display_Update(); //Update the whole screen
void Display_UpdateRow(uint8_t row); //Update a single line

void Display_SetPixel(uint8_t row, uint8_t pixel, Display_Colour col);
void Display_SetRow(uint8_t row, Display_Colour col);
void DisplayPrint(char* string);

void DisplayEventHandler();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H_ */
