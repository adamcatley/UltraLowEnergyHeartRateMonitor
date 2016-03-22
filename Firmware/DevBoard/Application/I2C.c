/*
 * I2C.c
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

#include "I2C.h"

///* TI-RTOS Header files */
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
I2C_Handle      i2c;
I2C_Params      i2cParams;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
 
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

//TODO: use semaphore
void I2C_readBuff(I2C_Transaction* transaction, uint8_t reg, uint8_t length){

	(*transaction).writeCount = 1;
	(*transaction).readCount = length;
    ((uint8_t*) (*transaction).writeBuf)[0] = reg;

    if (I2C_transfer(i2c, transaction)) {

	}
	else {
		System_printf("I2C bus fault!\n");
		System_flush();
	}
}

uint8_t I2C_readReg(I2C_Transaction* transaction, uint8_t reg){

	(*transaction).writeCount = 1;
    (*transaction).readCount = 1;
    ((uint8_t*) (*transaction).writeBuf)[0] = reg;

    if (I2C_transfer(i2c, transaction)) {
    	return ((uint8_t*) (*transaction).readBuf)[0];
	}
	else {
		System_printf("I2C bus fault!\n");
		System_flush();
		return 0;
	}
}

void I2C_writeReg(I2C_Transaction* transaction, uint8_t reg, uint8_t data){

	(*transaction).writeCount = 2;
	(*transaction).readCount = 0;
    ((uint8_t*) (*transaction).writeBuf)[0] = reg;
    ((uint8_t*) (*transaction).writeBuf)[1] = data;

    if (I2C_transfer(i2c, transaction)) {

	}
	else {
		System_printf("I2C bus fault!\n");
		System_flush();
	}
}

void I2C_UpdateParams(){

}

void I2C_Initialise(){ //TODO: add status return value
	if (i2c != NULL){
//		System_printf("I2C already initialised!\n");
//		System_flush();
		return;
	}

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
		System_printf("I2C Initialized\n");
	}

	System_flush();
}

/*********************************************************************
*********************************************************************/
