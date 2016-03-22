/*
 * DRV2605L.c
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

#include "DRV2605L.h"

#include <ti/drivers/I2C.h>
#include "I2C.h"
//
///* Example/Board Header files */
#include "Board.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define DRV2605L_I2C_BUFFER_LENGTH 				2

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
uint8_t         DRV2605L_txBuffer[DRV2605L_I2C_BUFFER_LENGTH];
uint8_t         DRV2605L_rxBuffer[DRV2605L_I2C_BUFFER_LENGTH];
I2C_Transaction DRV2605L_i2cTransaction;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void DRV2605L_Initialise(){ //TODO: add status return value
	I2C_Initialise(); //Should be initialised previously

    DRV2605L_i2cTransaction.slaveAddress = DRV_ADDR;
	DRV2605L_i2cTransaction.writeBuf = DRV2605L_txBuffer;
	DRV2605L_i2cTransaction.readBuf = DRV2605L_rxBuffer;

	//Check for device
	if (DRV2605L_ReadStatus() >> 5 == DRV_DEV_ID) System_printf("Found DRV2605L device\n");
	else System_printf("Error finding DRV2605L device\n");
	System_flush();

	//DRV2605L_Reset(); //Reset device to a known state. Wait after
	DRV2605L_Standby();
}

void DRV2605L_Reset(){
	I2C_writeReg(&DRV2605L_i2cTransaction, DRV_REG_MODE, DRV_MODE_RESET);
}

uint8_t DRV2605L_ReadStatus(){
	return I2C_readReg(&DRV2605L_i2cTransaction, DRV_REG_STATUS);
}

void DRV2605L_Standby(){
	uint8_t oldValue = I2C_readReg(&DRV2605L_i2cTransaction, DRV_REG_MODE) & ~DRV_MODE_STANDBY;
	I2C_writeReg(&DRV2605L_i2cTransaction, DRV_REG_MODE, oldValue | DRV_MODE_STANDBY);
}

/*********************************************************************
*********************************************************************/


