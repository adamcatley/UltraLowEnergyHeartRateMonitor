/*******************************************************************************
  Filename:       Vibration.c
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

//#include "simpleBLEBroadcaster.h"
#include "vibration.h"
#include "DRV2605L.h"

//#include <ti/sysbios/knl/Clock.h>

#include <ICall.h>
#include "util.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define VIBRATION_PERIODIC_EVENT_PERIOD			500
#define VIBRATION_EVENT_PERIODIC 0xFF //TODO: set value, and move to simpleBLEBroadcaster?

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
static Clock_Struct vibrationPeriodicClock;

static volatile uint8_t event = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Vibration_periodicClockHandler(UArg arg);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */


/*********************************************************************
 * @fn      HeartRateInit
 *
 * @brief   Initialization function for the heart rate monitor
 *
 */
void VibrationInit(void)
{
	DRV2605L_Initialise();
	uint8_t status = DRV2605L_ReadStatus();

	// Create one-shot clocks for internal periodic events.
	//Util_constructClock(&vibrationPeriodicClock, Vibration_periodicClockHandler, VIBRATION_PERIODIC_EVENT_PERIOD, 0, true, 0);

	System_printf("Vibration initialised: %x\n", status);
	System_flush();
}

/*********************************************************************
* Private functions
*/

static void Vibration_periodicClockHandler(UArg arg){
	event = VIBRATION_EVENT_PERIODIC;
	HeartRate_enqueueMsg(VIBRATION_EVENT_PERIODIC);//TODO: set correct value
	Util_startClock(&vibrationPeriodicClock);
}

/*********************************************************************
*********************************************************************/

