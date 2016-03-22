/*******************************************************************************
  Filename:       HeartRate.c
  Revised:        $Date: 2013-11-06 17:27:44 +0100 (on, 06 nov 2013) $
  Revision:       $Revision: 35922 $

  Description:

  Copyright 2015  Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include "HeartRate.h"
#include "simpleBLEBroadcaster.h"
#include "MAX30100.h"

#ifdef DISPLAY
#include "Display.h"
#endif

#include "string.h"
#include <ti/sysbios/knl/Clock.h>

#include <driverlib/aon_batmon.h>

#include <ICall.h>
#include "util.h"

///* Example/Board Header files */
#ifdef WEARABLE
#include "Board_WEARABLE.h"
#include "vibration.h"
#include "flash.h"
#include "Si1147.h"
#else
#include "Board.h"
#endif

/*********************************************************************
 * MACROS
 */

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
static Clock_Struct periodicClock;

//Pins
const PIN_Config pinListInt[] = {
       Board_MAX_INT | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE,
	   Board_BUTTON | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE | PIN_HYSTERESIS,
	   //Board_INA_INT | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE,
       PIN_TERMINATE
};
PIN_State  intPinState;
PIN_Handle intPinHandle;
static volatile uint8_t event = 0;

// MAX30100
static int sampleCount = 0;
static const int sampleRate = 10;//Hz
static const int sampleDuration = 10;//seconds
static const int samplePeriod = 1000 / sampleRate;//ms
static const int numSamples = sampleRate * sampleDuration;
static uint16_t IRSamples[numSamples];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void HeartRateIntHandler(PIN_Handle handle, PIN_Id pinId);

static void HeartRate_performPeriodicTask(void);
static void HeartRate_clockHandler(UArg arg);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */


/*********************************************************************
* Private functions
*/

/*********************************************************************
 * @fn      HeartRateInit
 *
 * @brief   Initialization function for the heart rate monitor
 *
 */
void HeartRateInit(void)
{

	//Initialise pins
	intPinHandle = PIN_open(&intPinState, pinListInt);
	if (!intPinHandle) {
	   System_abort("Error opening pins for HeartRate\n");
	}
	PIN_registerIntCb(intPinHandle, HeartRateIntHandler);
	PIN_setInterrupt(intPinHandle, Board_MAX_INT | PIN_IRQ_NEGEDGE);
	PIN_setInterrupt(intPinHandle, Board_BUTTON | PIN_IRQ_POSEDGE);
	//PIN_setInterrupt(hStateInt, Board_INA_INT | PIN_IRQ_NEGEDGE);

	// Initialise drivers
	MAX30100_Initialise();
	MAX30100_SetSampleRate(MAX_SPO2_50SPS);
	MAX30100_SetPulseWidth(MAX_SPO2_1600US);
	MAX30100_SetIRCurrent(MAX_IR_LED_40MA);
	MAX30100_EnableInterrupt(MAX_INT_HR_RDY);
	MAX30100_Shutdown(); //Keep asleep once configured until used

#ifdef WEARABLE
	VibrationInit();
	FlashInit();
	Si1147_Initialise();
#endif

#ifdef DISPLAY
	Display_Initialise();
#endif

	// Create one-shot clocks for internal periodic events.
	Util_constructClock(&periodicClock, HeartRate_clockHandler, samplePeriod, 0, false, 0);

	System_printf("Heart Rate initialised\n");
	System_flush();
}

//Handles pin interrupts for MAX30100 and othe devices
void HeartRateIntHandler(PIN_Handle handle, PIN_Id pinId){
	switch (pinId) {
		case Board_MAX_INT:
			event = HR_EVENT_MAX;
			break;
//		case Board_INA_INT:
//			event = HR_EVENT_INA;
//			break;
		case Board_BUTTON:
			event = HR_EVENT_BUTTON;
			break;
		default:
			event = HR_EVENT_UNKNOWN;
			break;
	}

	HeartRate_enqueueMsg(SBB_HEART_RATE_EVT);
}

void HeartRateEventHandler(){//TODO: make queue
	uint8_t eventID = event;//make copy as volatile
	event = 0; //reset flag immediately

	int i = 0;

	switch (eventID) {
		case HR_EVENT_MAX://MAX
			if(MAX30100_IsHRReady()){
				IRSamples[sampleCount] = MAX30100_ReadHROnly();
				sampleCount++;
				if( sampleCount % 5 == 0){
					uint16_t packetCount = sampleCount / 5;
					SimpleBLEBroadcaster_SetAdvertisingData(0x07, (uint8_t)(packetCount & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x08, (uint8_t)(packetCount >> 8), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x09, (uint8_t)sampleRate, 0);

					SimpleBLEBroadcaster_SetAdvertisingData(0x0a, (uint8_t)(IRSamples[sampleCount-5] & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x0b, (uint8_t)(IRSamples[sampleCount-5] >> 8), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x0c, (uint8_t)(IRSamples[sampleCount-4] & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x0d, (uint8_t)(IRSamples[sampleCount-4] >> 8), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x0e, (uint8_t)(IRSamples[sampleCount-3] & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x0f, (uint8_t)(IRSamples[sampleCount-3] >> 8), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x10, (uint8_t)(IRSamples[sampleCount-2] & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x11, (uint8_t)(IRSamples[sampleCount-2] >> 8), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x12, (uint8_t)(IRSamples[sampleCount-1] & 0xff), 0);
					SimpleBLEBroadcaster_SetAdvertisingData(0x13, (uint8_t)(IRSamples[sampleCount-1] >> 8), 1);
				}
				if (sampleCount >= numSamples){
					sampleCount = 0;
					MAX30100_FlushFIFO();
					MAX30100_Shutdown();
					Util_stopClock(&periodicClock);//stop sampling clock
					SimpleBLEBroadcaster_SetAdvertisingData(0x07, (uint8_t)(0), 0); //reset sample count
					SimpleBLEBroadcaster_SetAdvertisingData(0x08, (uint8_t)(0), 0); //TODO: reset all advetising data to initial values
					SimpleBLEBroadcaster_SetAdvertisingEnabled(false);//TODO: check works?
					System_printf("Finished sampling\n");
					System_flush();
#ifdef DISPLAY
					DisplayPrint("Finished sampling\n");
					Display_Update();
#endif
				}
				else {
					MAX30100_Shutdown();
				}
			}
			break;
		case HR_EVENT_INA:
			break;
		case HR_EVENT_BUTTON: //Button
#ifdef DISPLAY
			for (i = 0; i < 16; i++){//Display all colours
				Display_SetRow(i, DISPLAY_COLOUR_WHITE);
				Display_SetRow(i+16, DISPLAY_COLOUR_BLACK);
				Display_SetRow(i+32, DISPLAY_COLOUR_BLUE);
				Display_SetRow(i+48, DISPLAY_COLOUR_CYAN);
				Display_SetRow(i+64, DISPLAY_COLOUR_GREEN);
				Display_SetRow(i+80, DISPLAY_COLOUR_MAGENTA);
				Display_SetRow(i+96, DISPLAY_COLOUR_RED);
				Display_SetRow(i+112, DISPLAY_COLOUR_YELLOW);
			}

			DisplayPrint("Sampling at 10Hz\n");
			Display_Update();
#endif
			Util_startClock(&periodicClock);//start sampling clock
			break;
		case HR_EVENT_PERIODIC:
			HeartRate_performPeriodicTask();
			break;
		case HR_EVENT_UNKNOWN:
		default:
			System_printf("Unexpected HR event\n");
			System_flush();
			break;
	}
}

void HeartRate_performPeriodicTask(void){
	Util_startClock(&periodicClock);
	MAX30100_SetHRMode();//start sensor sampling
}

static void HeartRate_clockHandler(UArg arg){
	event = HR_EVENT_PERIODIC;
	HeartRate_enqueueMsg(SBB_HEART_RATE_EVT);
}

/*********************************************************************
*********************************************************************/

