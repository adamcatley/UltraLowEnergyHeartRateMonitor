/*
 * SPI.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

///* TI-RTOS Header files */
#include <ti/drivers/SPI.h>

/*********************************************************************
 * CONSTANTS
 */

#define SPI_BITRATE				1000000 //1MHz

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */


void SPI_Initialise();
void SPI_UpdateParams(); //TODO: Implement

void SPI_SendReceive(SPI_Transaction* transaction);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SPI_H_ */
