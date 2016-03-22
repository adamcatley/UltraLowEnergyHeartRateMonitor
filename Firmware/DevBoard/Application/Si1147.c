/*
 * Si1147.c
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

#include "Si1147.h"


///* TI-RTOS Header files */
#include <ti/drivers/I2C.h>
#include "I2C.h"


///* Example/Board Header files */
#include "Board_WEARABLE.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define Si1147_I2C_BUFFER_LENGTH		4

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
uint8_t         Si1147_txBuffer[Si1147_I2C_BUFFER_LENGTH];
uint8_t         Si1147_rxBuffer[Si1147_I2C_BUFFER_LENGTH];
I2C_Transaction Si1147_i2cTransaction;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void Si1147_Initialise(){ //TODO: add status return value
	I2C_Initialise();

    Si1147_i2cTransaction.slaveAddress = SIL_ADDR;
	Si1147_i2cTransaction.writeBuf = Si1147_txBuffer;
	Si1147_i2cTransaction.readBuf = Si1147_rxBuffer;

	//Check for device
	if (I2C_readReg(&Si1147_i2cTransaction, SIL_REG_PART_ID) == SIL_DEV_ID) System_printf("Found Si1147 device\n");
	else System_abort("Error finding Si1147 device\n");

	//Si1147_Reset(); //Reset device to a known state
	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_HW_KEY, SIL_HW_KEY); //Write HW_KEY register to initialise
	System_flush();
}

void Si1147_Reset(){
	Si1147_Command(SIL_CMD_RESET);
	//TODO: add delay to wait for reset
}

uint8_t Si1147_Command(uint8_t command){
	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_COMMAND, 0x00);
	//Should check response register is 0
	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_COMMAND, command);
	return I2C_readReg(&Si1147_i2cTransaction, SIL_REG_RESPONSE);//can check for result and error
}

uint8_t Si1147_ReadStatus(){
	return I2C_readReg(&Si1147_i2cTransaction, SIL_REG_CHIP_STAT);
}

////Check if the device has a HR only measurement available
//uint8_t Si1147_IsHRReady(){
//	return (Si1147_ReadStatus() & SIL_INT_HR_RDY) == SIL_INT_HR_RDY;
//}
//
//void Si1147_SetHRMode(){
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_MODE_CONFIG, SIL_MODE_HR_ONLY);
//}
//
//void Si1147_SetSampleRate(uint8_t sampleRate){ //TODO: add check for valid rate
//	uint8_t oldValue = I2C_readReg(&Si1147_i2cTransaction, SIL_REG_SPO2_CONFIG) & ~0x1c;
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_SPO2_CONFIG, oldValue | sampleRate);
//}
//
//void Si1147_SetPulseWidth(uint8_t width){//TODO: add check for valid width
//	uint8_t oldValue = I2C_readReg(&Si1147_i2cTransaction, SIL_REG_SPO2_CONFIG) & ~0x03;
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_SPO2_CONFIG, oldValue | width);
//}
//
//void Si1147_SetIRCurrent(uint8_t current){//TODO: add check for valid current value
//	uint8_t oldValue = I2C_readReg(&Si1147_i2cTransaction, SIL_REG_LED_CONFIG) & ~0x0f;
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_LED_CONFIG, oldValue | current);
//}
//
//void Si1147_SetRedCurrent(uint8_t current){//TODO: add check for valid current value
//	uint8_t oldValue = I2C_readReg(&Si1147_i2cTransaction, SIL_REG_LED_CONFIG) & ~0xf0;
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_LED_CONFIG, oldValue | current);
//}
//
//void Si1147_Shutdown(){
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_MODE_CONFIG, SIL_MODE_SHDN);
//}
//
//void Si1147_EnableInterrupt(uint8_t int_en){
//	uint8_t oldValue = I2C_readReg(&Si1147_i2cTransaction, SIL_REG_INT_EN) & ~(int_en);
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_INT_EN, oldValue | int_en);
//}
//
//void Si1147_FlushFIFO(){ //TODO: doesnt seem to work
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_FIFO_RD_PTR, 0x00);
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_FIFO_WR_PTR, 0x00);
//	I2C_writeReg(&Si1147_i2cTransaction, SIL_REG_FIFO_OVF_CNTR, 0x00);
//}
//
//uint16_t Si1147_ReadHROnly(){
//	I2C_readBuff(&Si1147_i2cTransaction, SIL_REG_FIFO_DATA, 4);
//	return (Si1147_rxBuffer[SIL_FIFO_IR_UPPER] << 8) | Si1147_rxBuffer[SIL_FIFO_IR_LOWER]; //return IR value
//}

/*********************************************************************
*********************************************************************/


