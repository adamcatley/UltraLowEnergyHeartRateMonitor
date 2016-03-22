/*
 * MX25.c
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

#include "MX25.h"

#include "SPI.h"
#include <ti/drivers/SPI.h>

#include <ti/sysbios/knl/Clock.h>
#include <ICall.h>
#include "util.h"

///* Example/Board Header files */
#include "Board_WEARABLE.h"

/*********************************************************************
 * MACROS
 */

#define MX25_SPI_SET_CS()			PIN_setOutputValue(MX25pinHandle, Board_FLASH_CS, 0)
#define MX25_SPI_CLEAR_CS()		PIN_setOutputValue(MX25pinHandle, Board_FLASH_CS, 1)

/*********************************************************************
 * CONSTANTS
 */

#define MX25_SPI_BUFFER_LENGTH 				8 //Needs to be 256 for read/write commands? Allocation issues?

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t         MX25_txBuffer[MX25_SPI_BUFFER_LENGTH];
static uint8_t         MX25_rxBuffer[MX25_SPI_BUFFER_LENGTH];
SPI_Transaction MX25_spiTransaction;

//Pins
const PIN_Config MX25pinList[] = {
		Board_FLASH_CS	| PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		PIN_TERMINATE
};
PIN_State  MX25pinState;
PIN_Handle MX25pinHandle;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void MX25_Initialise(){ //TODO: add status return value
	SPI_Initialise(); //Should be initialised previously

	MX25_spiTransaction.arg = NULL;
	MX25_spiTransaction.rxBuf = MX25_rxBuffer;
	MX25_spiTransaction.txBuf = MX25_txBuffer;
	//MX25_spiTransaction.count must be set for each transfer

	//Initialise pin for manual control of CS line
	MX25pinHandle = PIN_open(&MX25pinState, MX25pinList);
	if (!MX25pinHandle) {
	   System_abort("Error opening pins for flash memory\n");
	}

	MX25_SPI_CLEAR_CS(); //Set CS high (deassert)

	//Check for device
	if (MX25_CheckID()) System_printf("Found MX25 device\n");
	else System_printf("Error finding MX25 device\n");
	System_flush();

	//MX25_Reset(); //Reset device to a known state. Wait after
}

void MX25_Reset(){
	MX25_txBuffer[0] = MX25_CMD_RSTEN;
	MX25_spiTransaction.count = 1;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();


	MX25_txBuffer[0] = MX25_CMD_RST;
	MX25_spiTransaction.count = 1;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();
}

uint8_t MX25_ReadStatus(){
	MX25_txBuffer[0] = MX25_CMD_RDSR;
	MX25_spiTransaction.count = 3;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();

	return MX25_rxBuffer[1];
}

uint16_t MX25_ReadConfig(){
	MX25_txBuffer[0] = MX25_CMD_RDCR;
	MX25_spiTransaction.count = 3;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();

	return ((uint16_t)(MX25_rxBuffer[1]) << 8) | MX25_rxBuffer[2];
}

uint8_t MX25_CheckID(){
	MX25_txBuffer[0] = MX25_CMD_REMS;
	MX25_txBuffer[1] = 0;
	MX25_txBuffer[2] = 0;
	MX25_txBuffer[3] = 0;//Man ID first
	MX25_spiTransaction.count = 6;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();

	if ((MX25_rxBuffer[4] == MX25_MAN_ID) && (MX25_rxBuffer[5] == MX25_DEV_ID)){
		return 1;
	}
	else{
		return 0;
	}
}

void MX25_Powerdown(){
	MX25_txBuffer[0] = MX25_CMD_DP;
	MX25_spiTransaction.count = 1;

	MX25_SPI_SET_CS();
	SPI_SendReceive(&MX25_spiTransaction);
	MX25_SPI_CLEAR_CS();
}


/*********************************************************************
*********************************************************************/


