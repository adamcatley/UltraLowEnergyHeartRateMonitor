/**************************************************************************************************
  Filename:       simpleBLEBroadcaster.h
  Revised:        $Date: 2014-04-17 15:59:23 -0700 (Thu, 17 Apr 2014) $
  Revision:       $Revision: 38222 $

  Description:    This file contains the Simple BLE Broadcaster sample application
                  definitions and prototypes.

  Copyright 2011 - 2014 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
**************************************************************************************************/

#ifndef SIMPLEBLEBROADCASTER_H
#define SIMPLEBLEBROADCASTER_H

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

// Internal Events for RTOS application
#define SBB_STATE_CHANGE_EVT				  0x0001
#define SBB_HEART_RATE_EVT					  0x0002
#define SBB_DISPLAY_EVT						  0x0004
#define SBB_FLASH_EVT						  0x0008
#define SBB_VIBRATION_EVT					  0x0010
#define SBB_UNKNOWN_EVT						  0x00FF
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */
  
/*
 * Task creation function for the Simple BLE Broadcaster.
 */
extern void SimpleBLEBroadcaster_createTask(void);
void HeartRate_enqueueMsg(uint8_t event);
void SimpleBLEBroadcaster_SetAdvertisingData(uint8_t index, uint8_t value, uint8_t update);
void SimpleBLEBroadcaster_SetAdvertisingEnabled(uint8_t enable);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEBROADCASTER_H */
