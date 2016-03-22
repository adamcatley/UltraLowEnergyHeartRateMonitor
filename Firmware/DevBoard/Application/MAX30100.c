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

#include "MAX30100.h"


///* TI-RTOS Header files */
#include <ti/drivers/I2C.h>
#include "I2C.h"


///* Example/Board Header files */
#ifdef WEARABLE
#include "Board_WEARABLE.h"
#else
#include "Board.h"
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define MAX30100_I2C_BUFFER_LENGTH		4

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
uint8_t         MAX30100_txBuffer[MAX30100_I2C_BUFFER_LENGTH];
uint8_t         MAX30100_rxBuffer[MAX30100_I2C_BUFFER_LENGTH];
I2C_Transaction MAX30100_i2cTransaction;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void MAX30100_Initialise(){ //TODO: add status return value
	I2C_Initialise();

    MAX30100_i2cTransaction.slaveAddress = MAX_ADDR;
	MAX30100_i2cTransaction.writeBuf = MAX30100_txBuffer;
	MAX30100_i2cTransaction.readBuf = MAX30100_rxBuffer;

	//Check for device
	if (I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_DEV_ID) == MAX_DEV_ID) System_printf("Found MAX030100 device\n");
	else System_abort("Error finding MAX30100 device\n");

	MAX30100_Reset(); //Reset device to a known state
	System_flush();
}

void MAX30100_Reset(){
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_MODE_CONFIG, MAX_MODE_RESET);
	//TODO: add delay to wait for reset
}

uint8_t MAX30100_ReadStatus(){
	return I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_INT_STATUS);
}

//Check if the device has a HR only measurement available
uint8_t MAX30100_IsHRReady(){
	return (MAX30100_ReadStatus() & MAX_INT_HR_RDY) == MAX_INT_HR_RDY;
}

void MAX30100_SetHRMode(){
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_MODE_CONFIG, MAX_MODE_HR_ONLY);
}

void MAX30100_SetSampleRate(uint8_t sampleRate){ //TODO: add check for valid rate
	uint8_t oldValue = I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_SPO2_CONFIG) & ~0x1c;
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_SPO2_CONFIG, oldValue | sampleRate);
}

void MAX30100_SetPulseWidth(uint8_t width){//TODO: add check for valid width
	uint8_t oldValue = I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_SPO2_CONFIG) & ~0x03;
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_SPO2_CONFIG, oldValue | width);
}

void MAX30100_SetIRCurrent(uint8_t current){//TODO: add check for valid current value
	uint8_t oldValue = I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_LED_CONFIG) & ~0x0f;
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_LED_CONFIG, oldValue | current);
}

void MAX30100_SetRedCurrent(uint8_t current){//TODO: add check for valid current value
	uint8_t oldValue = I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_LED_CONFIG) & ~0xf0;
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_LED_CONFIG, oldValue | current);
}

void MAX30100_Shutdown(){
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_MODE_CONFIG, MAX_MODE_SHDN);
}

void MAX30100_EnableInterrupt(uint8_t int_en){
	uint8_t oldValue = I2C_readReg(&MAX30100_i2cTransaction, MAX_REG_INT_EN) & ~(int_en);
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_INT_EN, oldValue | int_en);
}

void MAX30100_FlushFIFO(){ //TODO: doesnt seem to work
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_FIFO_RD_PTR, 0x00);
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_FIFO_WR_PTR, 0x00);
	I2C_writeReg(&MAX30100_i2cTransaction, MAX_REG_FIFO_OVF_CNTR, 0x00);
}

uint16_t MAX30100_ReadHROnly(){
	I2C_readBuff(&MAX30100_i2cTransaction, MAX_REG_FIFO_DATA, 4);
	return (MAX30100_rxBuffer[MAX_FIFO_IR_UPPER] << 8) | MAX30100_rxBuffer[MAX_FIFO_IR_LOWER]; //return IR value
}

/*********************************************************************
*********************************************************************/


