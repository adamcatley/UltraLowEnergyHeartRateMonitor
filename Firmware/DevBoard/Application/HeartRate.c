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
#include "MAX30100.h"

#include "string.h"
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

///* TI-RTOS Header files */
//#include <ti/drivers/PIN.h>
//#include <ti/drivers/I2C.h>
//
///* Example/Board Header files */
#include "Board.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Task configuration
#define HEART_RATE_TASK_PRIORITY    1
#define HEART_RATE_TASK_STACK_SIZE  600

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
// Entity ID used to check for source and/or destination of messages
//static ICall_EntityID sensorSelfEntity;

// Semaphore used to post events to the application thread
//static ICall_Semaphore sensorSem;

// Task setup
static Task_Struct HeartRateTask;
static Char HeartRateTaskStack[HEART_RATE_TASK_STACK_SIZE];

//Pins
const PIN_Config pinListInt[] = {
       Board_MAX_INT | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE,
       Board_BUTTON | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE,
	   //Board_INA_INT | PIN_INPUT_EN  | PIN_NOPULL | PIN_IRQ_NEGEDGE,
       PIN_TERMINATE
};
PIN_State  intPinState;
PIN_Handle intPinHandle;
static volatile uint8_t intPinEvent = 0;

// MAX30100
static int sampleCount = 0;
static const int numSamples = 50 * 10; //50 * NUM_SECONDS
uint16_t IRSamples[numSamples];


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void HeartRate_TaskFxn(UArg a0, UArg a1);
void HeartRateIntHandler(PIN_Handle handle, PIN_Id pinId);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SensorTagTmp_createTask
 *
 * @brief   Task creation function for the SensorTag
 *
 * @param   none
 *
 * @return  none
 */
void HeartRate_createTask(void)
{
  Task_Params taskParames;
  
  // Create the task for the state machine
  Task_Params_init(&taskParames);
  taskParames.stack = HeartRateTaskStack;
  taskParames.stackSize = HEART_RATE_TASK_STACK_SIZE;
  taskParames.priority = HEART_RATE_TASK_PRIORITY;

  Task_construct(&HeartRateTask, HeartRate_TaskFxn, &taskParames, NULL);
}

/*********************************************************************
* Private functions
*/

/*********************************************************************
 * @fn      HeartRateTaskInit
 *
 * @brief   Initialization function for the heart rate monitor
 *
 */
static void HeartRateTaskInit(void)
{
	System_printf("Heart rate task started\n");
	System_flush();

	//Initialise pins
	intPinHandle = PIN_open(&intPinState, pinListInt);
	if (!intPinHandle) {
	   System_abort("Error opening pins");
	}
	PIN_registerIntCb(intPinHandle, HeartRateIntHandler);
	PIN_setInterrupt(intPinHandle, Board_MAX_INT | PIN_IRQ_NEGEDGE);
	PIN_setInterrupt(intPinHandle, Board_BUTTON | PIN_IRQ_NEGEDGE);
	//PIN_setInterrupt(hStateInt, Board_INA_INT | PIN_IRQ_NEGEDGE);


  // Initilialize the module state variables
  
  // Initialise driver
	MAX30100_Initialise();
	MAX30100_SetSampleRate(MAX_SPO2_50SPS);
	MAX30100_SetPulseWidth(MAX_SPO2_1600US);
	MAX30100_SetIRCurrent(MAX_IR_LED_40MA);
	MAX30100_EnableInterrupt(MAX_INT_HR_RDY);

}

//Handles pin interrupts for MAX30100 and othe devices
void HeartRateIntHandler(PIN_Handle handle, PIN_Id pinId){
	if (pinId == Board_MAX_INT)	intPinEvent = HR_EVENT_MAX;
	else if (pinId == Board_INA_INT) intPinEvent = HR_EVENT_INA;
	else if (pinId == Board_BUTTON) intPinEvent = HR_EVENT_BUTTON;
	else intPinEvent = HR_EVENT_UNKNOWN;
}

void ProcessIntEvent(){//TODO: make queue
	uint8_t eventID = intPinEvent;//make copy as volatile
	intPinEvent = 0; //reset flag immediately
	switch (eventID) {
		case HR_EVENT_MAX://MAX
			if(MAX30100_IsHRReady()){
				IRSamples[sampleCount] = MAX30100_ReadHROnly();
				sampleCount++;
				if (sampleCount >= numSamples){
					sampleCount = 0;
					MAX30100_FlushFIFO();
				}
			}
			break;
		case HR_EVENT_INA:
			break;
		case HR_EVENT_BUTTON: //Button
			MAX30100_SetHRMode();
			break;
		case HR_EVENT_UNKNOWN:
			System_printf("Unexpected interrupt\n");
			System_flush();
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      HeartRate_TaskFxn
 *
 * @brief   The task loop of the heart rate monmnitor task
 *
 * @return  none
 */
static void HeartRate_TaskFxn(UArg a0, UArg a1)
{
  //Initialise variables
  
  // Initialize the task
	HeartRateTaskInit();
  
  // Task loop
  while (1)
  {
	  if (intPinEvent != 0) ProcessIntEvent();

	  //delay_ms(SENSOR_DEFAULT_PERIOD);
  }
}


/*********************************************************************
*********************************************************************/

