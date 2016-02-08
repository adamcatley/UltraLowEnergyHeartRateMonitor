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
//#include <ti/drivers/PIN.h>
//#include <driverlib/i2c.h>
#include <ti/drivers/I2C.h>
//
///* Example/Board Header files */
#include "Board.h"

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
uint8_t         MAX30100txBuffer[MAX30100_I2C_BUFFER_LENGTH];
uint8_t         MAX30100rxBuffer[MAX30100_I2C_BUFFER_LENGTH];
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void readBuff(uint8_t reg, uint8_t length){

    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = length;
    MAX30100txBuffer[0] = reg;

    if (I2C_transfer(i2c, &i2cTransaction)) {

	}
	else {
		System_printf("I2C bus fault!\n");
	}
}

uint8_t readReg(uint8_t reg){

    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = 1;
    MAX30100txBuffer[0] = reg;

    if (I2C_transfer(i2c, &i2cTransaction)) {
    	return MAX30100rxBuffer[0];
	}
	else {
		System_printf("I2C bus fault!\n");
		return 0;
	}
}

void writeReg(uint8_t reg, uint8_t data){

    i2cTransaction.writeCount = 2;
    i2cTransaction.readCount = 0;
    MAX30100txBuffer[0] = reg;
    MAX30100txBuffer[1] = data;

    if (I2C_transfer(i2c, &i2cTransaction)) {

	}
	else {
		System_printf("I2C bus fault!\n");
	}
}


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void MAX30100_Initialise(){ //TODO: add status return value
	I2C_init();
	I2C_Params_init(&i2cParams);
	i2cParams.bitRate = I2C_400kHz;
	i2c = I2C_open(Board_I2C, &i2cParams);

	if (i2c == NULL) {
		System_abort("Error Initializing I2C\n");
		System_flush();
		return;
	}
	else {
		System_printf("I2C Initialized!\n");
	}

    i2cTransaction.slaveAddress = MAX_ADDR;
	i2cTransaction.writeBuf = MAX30100txBuffer;
	i2cTransaction.readBuf = MAX30100rxBuffer;

	//Check for device
	if (readReg(MAX_REG_DEV_ID) == MAX_DEV_ID) System_printf("Found MAX030100 device\n");
	else System_abort("Error finding MAX30100 device\n");

	MAX30100_Reset(); //Reset device to a known state
	System_flush();
}

void MAX30100_Reset(){
	writeReg(MAX_REG_MODE_CONFIG, MAX_MODE_RESET);
	//TODO: add delay to wait for reset
}

uint8_t MAX30100_ReadStatus(){
	return readReg(MAX_REG_INT_STATUS);
}

//Check if the device has a HR only measurement available
uint8_t MAX30100_IsHRReady(){
	return (MAX30100_ReadStatus() & MAX_INT_HR_RDY) == MAX_INT_HR_RDY;
}

void MAX30100_SetHRMode(){
	writeReg(MAX_REG_MODE_CONFIG, MAX_MODE_HR_ONLY);
}

void MAX30100_SetSampleRate(uint8_t sampleRate){ //TODO: add check for valid rate
	uint8_t oldValue = readReg(MAX_REG_SPO2_CONFIG) & ~0x1c;
	writeReg(MAX_REG_SPO2_CONFIG, oldValue | sampleRate);
}

void MAX30100_SetPulseWidth(uint8_t width){//TODO: add check for valid width
	uint8_t oldValue = readReg(MAX_REG_SPO2_CONFIG) & ~0x03;
	writeReg(MAX_REG_SPO2_CONFIG, oldValue | width);
}

void MAX30100_SetIRCurrent(uint8_t current){//TODO: add check for valid current value
	uint8_t oldValue = readReg(MAX_REG_LED_CONFIG) & ~0x0f;
	writeReg(MAX_REG_LED_CONFIG, oldValue | current);
}

void MAX30100_SetRedCurrent(uint8_t current){//TODO: add check for valid current value
	uint8_t oldValue = readReg(MAX_REG_LED_CONFIG) & ~0xf0;
	writeReg(MAX_REG_LED_CONFIG, oldValue | current);
}

void MAX30100_Shutdown(){
	writeReg(MAX_REG_MODE_CONFIG, MAX_MODE_SHDN);
}

void MAX30100_EnableInterrupt(uint8_t int_en){
	uint8_t oldValue = readReg(MAX_REG_INT_EN) & ~(int_en);
	writeReg(MAX_REG_INT_EN, oldValue | int_en);
}

void MAX30100_FlushFIFO(){ //TODO: doesnt seem to work
	writeReg(MAX_REG_FIFO_RD_PTR, 0x00);
	writeReg(MAX_REG_FIFO_WR_PTR, 0x00);
	writeReg(MAX_REG_FIFO_OVF_CNTR, 0x00);
}

uint16_t MAX30100_ReadHROnly(){
	readBuff(MAX_REG_FIFO_DATA, 4);
	return (MAX30100rxBuffer[MAX_FIFO_IR_UPPER] << 8) | MAX30100rxBuffer[MAX_FIFO_IR_LOWER]; //return IR value
}

/*********************************************************************
*********************************************************************/


