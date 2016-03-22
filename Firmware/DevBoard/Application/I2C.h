/*
 * I2C.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <ti/drivers/I2C.h>

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

void I2C_Initialise();
void I2C_UpdateParams(); //TODO: Implement

void I2C_writeReg(I2C_Transaction* transaction, uint8_t reg, uint8_t data);
uint8_t I2C_readReg(I2C_Transaction* transaction, uint8_t reg);
void I2C_readBuff(I2C_Transaction* transaction, uint8_t reg, uint8_t length);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
