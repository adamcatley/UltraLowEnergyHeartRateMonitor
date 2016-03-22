/*
 * SPI.c
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

#include "SPI.h"

///* TI-RTOS Header files */
#include <ti/drivers/SPI.h>
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
SPI_Handle      spi;
SPI_Params      spiParams;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

//TODO: use semaphore
void SPI_SendReceive(SPI_Transaction* transaction){
	if (SPI_transfer(spi, transaction)) {

	}
	else {
		System_printf("SPI transfer error!\n");
		System_flush();
	}
}

void SPI_UpdateParams(){

}

void SPI_Initialise(){ //TODO: add status return value
	if (spi != NULL){
//		System_printf("SPI already initialised!\n");
//		System_flush();
		return;
	}

	SPI_init();

	SPI_Params_init(&spiParams);
	spiParams.bitRate = SPI_BITRATE;
	spiParams.frameFormat = SPI_POL0_PHA0; //clock idle low, data valid rising edge
	spiParams.dataSize = 8;
	spiParams.mode = SPI_MASTER;
	spiParams.transferMode = SPI_MODE_BLOCKING; //TODO: change to callback

	spi = SPI_open(Board_DISPLAY_SPI, &spiParams);

	if (spi == NULL){
		System_abort("Error Initializing SPI\n");
		System_flush();
		return;
	}
	else {
		System_printf("SPI Initialized\n");
		System_flush();
	}
}

/*********************************************************************
*********************************************************************/
