/*
 * MAX30100.c
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */


/*********************************************************************
 * INCLUDES
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include "Display.h"
#include "graphics.h"

#include "SPI.h"

#include <ti/sysbios/knl/Clock.h>
#include "util.h"
#include "ICall.h"
//
///* Example/Board Header files */
#ifdef WEARABLE
#include "Board_WEARABLE.h"
#else
#include "Board.h"
#endif

/*********************************************************************
 * MACROS
 */
#define DISPLAY_SPI_SET_CS()			PIN_setOutputValue(displayPinHandle, Board_DISPLAY_CS, 1)
#define DISPLAY_SPI_CLEAR_CS()		PIN_setOutputValue(displayPinHandle, Board_DISPLAY_CS, 0)
#define DISPLAY_SET_BIT(__line__, __byte__, __bit__)	( pixels[__line__][__byte__] |= (1 << __bit__) )
#define DISPLAY_CLEAR_BIT(__line__, __byte__, __bit__)	( pixels[__line__][__byte__] &= ~(1 << __bit__) )

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Clock instances for internal periodic events.
static Clock_Struct displayPeriodicClock;

//SPI
SPI_Transaction displaySPITransaction;

//Main screen buffer
//static uint8_t pixels[DISPLAY_HEIGHT][DISPLAY_BYTES_PER_LINE]; //[128][48]
static uint8_t **pixels; //Buffer allocated on ICall heap

//SPI TX buffer
static uint8_t DisplayTXBuffer[2 + (DISPLAY_BYTES_PER_LINE) + 2]; //52 bytes for line update command

//Pins
const PIN_Config displayPinList[] = {
		Board_DISPLAY_CS	| PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_DISPLAY_EXTCOMIN		| PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		PIN_TERMINATE
};
PIN_State  displayPinState;
PIN_Handle displayPinHandle;

static const uint8_t BitReverseTable256[] =
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Display_clockHandler(UArg arg);


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void Display_Initialise(){
	//Allocate buffer in ICall heap

	//Allocate on a per line basis instead of requiring a big block to be available
	pixels = (uint8_t**)ICall_malloc(DISPLAY_HEIGHT*sizeof(uint8_t*));
	uint8_t i;
	for (i = 0; i < DISPLAY_HEIGHT; i++){
		pixels[i] = (uint8_t*) ICall_malloc(DISPLAY_BYTES_PER_LINE*sizeof(uint8_t));
	}

	if (pixels == NULL){
		System_abort("Couldn't allocate buffer in heap\n");
	}
	SPI_Initialise();

	displaySPITransaction.arg = NULL;
	displaySPITransaction.rxBuf = NULL;
	displaySPITransaction.txBuf = DisplayTXBuffer;
	//displaySPITransaction.count must be set for each transfer

	//Manual control of CS line. Must be high 3us before data and 1us after data
	//TODO: configure control lines (enable, EXTMODE etc)
	//Initialise pins
	displayPinHandle = PIN_open(&displayPinState, displayPinList);
	if (!displayPinHandle) {
	   System_abort("Error opening pins for display\n");
	}

	DISPLAY_SPI_CLEAR_CS(); //Set CS low (deassert)

	Display_Clear();

	//Set screen to black
//	fillScreen(DISPLAY_COLOUR_BLACK);
	//Intialise buffer to white
	uint8_t x, y;
	for (y = 0; y < DISPLAY_HEIGHT; y++){
		for(x = 0; x < DISPLAY_BYTES_PER_LINE; x++){
			pixels[y][x] = 0xff; //Shorthand to set all to white insead of calls to SetPixel()
		}
	}

	fillCircle(64, 64, 20, DISPLAY_COLOUR_YELLOW);
	DisplayPrint("Heart Rate Monitor\n");
	DisplayPrint("by Adam Catley\n");
	Display_Update();


	// Create one-shot clocks for internal periodic events.
	Util_constructClock(&displayPeriodicClock, Display_clockHandler, 500, 0, true, 0); //1Hz

	System_printf("Display initialised\n");
	System_flush();
}

static void Display_clockHandler(UArg arg){
	Util_startClock(&displayPeriodicClock);
	PIN_setOutputValue(displayPinHandle, Board_DISPLAY_EXTCOMIN, !PIN_getOutputValue(Board_DISPLAY_EXTCOMIN)); //Toggle EXCOMIN pin
}

void Display_Clear(){
	DisplayTXBuffer[0] = DISPLAY_CMD_CLEAR;
	DisplayTXBuffer[1] = 0;
	displaySPITransaction.count = 2;

	DISPLAY_SPI_SET_CS();
	SPI_SendReceive(&displaySPITransaction);
	DISPLAY_SPI_CLEAR_CS();
}

void Display_Update(){
	uint8_t row;
	for (row = 0; row < DISPLAY_HEIGHT; row++){
		Display_UpdateRow(row);
	}
}

void Display_UpdateRow(uint8_t row){
	uint8_t i = 0;
	DisplayTXBuffer[i++] = DISPLAY_CMD_UPDATE;
	DisplayTXBuffer[i++] = BitReverseTable256[DISPLAY_WIDTH - (row + 1)]; //Compensate for LSB first display, 1-128 index top to bottom and upside down
	uint8_t currentByte;
	for (currentByte = 0; currentByte < DISPLAY_BYTES_PER_LINE; currentByte++){
		DisplayTXBuffer[i + currentByte] = BitReverseTable256[pixels[row][currentByte]];
	}
	i += currentByte;
	DisplayTXBuffer[i++] = 0; //First dummy byte
	DisplayTXBuffer[i++] = 0; //Second dummy byte

	displaySPITransaction.count = i; //Should be 52

	DISPLAY_SPI_SET_CS();
	SPI_SendReceive(&displaySPITransaction);
	DISPLAY_SPI_CLEAR_CS();
}

void Display_SetPixel(uint8_t row, uint8_t pixel, Display_Colour col){
	pixel = DISPLAY_WIDTH - pixel - 1; //Compensate for rotation of screen

	//Calculate index for LED channel bit
	uint16_t bitIndex = pixel * 3;

	uint8_t redByteIndex = (bitIndex+0)/8;
	uint8_t greenByteIndex = (bitIndex+1)/8;
	uint8_t blueByteIndex = (bitIndex+2)/8;

	uint8_t redValue = (col >> 0) & 0x01;
	uint8_t greenValue = (col >> 1) & 0x01;
	uint8_t blueValue = (col >> 2) & 0x01;

	uint8_t redBitIndex = (bitIndex + 0) % 8;
	uint8_t greenBitIndex = (bitIndex + 1) % 8;
	uint8_t blueBitIndex = (bitIndex + 2) % 8;


	redValue ? DISPLAY_SET_BIT(row, redByteIndex, redBitIndex) : DISPLAY_CLEAR_BIT(row, redByteIndex, redBitIndex);
	greenValue ? DISPLAY_SET_BIT(row, greenByteIndex, greenBitIndex) : DISPLAY_CLEAR_BIT(row, greenByteIndex, greenBitIndex);
	blueValue ? DISPLAY_SET_BIT(row, blueByteIndex, blueBitIndex) : DISPLAY_CLEAR_BIT(row, blueByteIndex, blueBitIndex);
}

void Display_SetRow(uint8_t row, Display_Colour col){
	uint8_t x;
	for (x = 0; x < DISPLAY_WIDTH; x++){
		Display_SetPixel(row, x, col);
	}
}

/*********************************************************************
*********************************************************************/


