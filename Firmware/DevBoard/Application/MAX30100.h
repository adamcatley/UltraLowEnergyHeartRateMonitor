/*
 * MAX30100.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef MAX30100_H_
#define MAX30100_H_

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

// I2C definitions
#define MAX_ADDR				0x57		//7 bit read address (0xAE = 8 bit read)

// I2C register map
#define MAX_DEV_ID				0x11		//Default value of revision ID register
#define MAX_REG_INT_STATUS 		0x00		//Interrupt status (read only)
#define MAX_REG_INT_EN 			0x01		//Interrupt enable
#define MAX_REG_FIFO_WR_PTR		0x02		//FIFO write pointer
#define MAX_REG_FIFO_OVF_CNTR	0x03		//Over flow counter
#define MAX_REG_FIFO_RD_PTR		0x04		//FIFO read pointer
#define MAX_REG_FIFO_DATA		0x05		//FIFO data register
#define MAX_REG_MODE_CONFIG		0x06		//Mode configuration
#define MAX_REG_SPO2_CONFIG		0x07		//SPO2 configuration
#define MAX_REG_LED_CONFIG		0x09		//LED configuration
#define MAX_REG_TEMP_INT		0x16		//Temperature Integer
#define MAX_REG_TEMP_FRAC		0x17		//Temperature Fraction
#define MAX_REG_REV_ID			0xFE		//Revision ID (read only)
#define MAX_REG_DEV_ID			0xFF		//Part ID (0x11)

// I2C Mode register values
#define MAX_MODE_SHDN			0x80		//Shutdown
#define MAX_MODE_RESET			0x40		//Reset
#define MAX_MODE_TEMP_EN		0x08		//Entable temperature measurement
#define MAX_MODE_HR_ONLY		0x02		//Enable Heart Rate only mode (IR LED only, not RED)
#define MAX_MODE_SPO2			0x03		//Enable SPO2 mode (IR & RED LED)

//I2C Interrupt register bit fields
#define MAX_INT_A_FULL			0x80		//FIFO almost full (15/16 samples)
#define MAX_INT_TEMP_RDY		0x40		//Temperature measurement ready
#define MAX_INT_HR_RDY			0x20		//Heart rate measurement ready
#define MAX_INT_SPO2_RDY		0x10		//SPO2 measurement ready
#define MAX_INT_PWR_RDY			0x01		//Power ready

// FIFO index
#define MAX_FIFO_IR_UPPER		0x0
#define MAX_FIFO_IR_LOWER		0x1
#define MAX_FIFO_RED_UPPER		0x2
#define MAX_FIFO_RED_LOWER		0x3

// SPO2 register values
#define MAX_SPO2_HIRES_EN		0x40		//Enable high resolution mode (16bit, 1600us)
#define MAX_SPO2_50SPS			0x00		//50 samples per second
#define MAX_SPO2_100SPS			0x04		//100 samples per second
#define MAX_SPO2_1000SPS		0x1C		//1000 samples per second
#define MAX_SPO2_200US			0x00 		// 200us = 13 bit resolution
#define MAX_SPO2_400US			0x01 		// 400us = 14 bit resolution
#define MAX_SPO2_800US			0x02 		// 800us = 15 bit resolution
#define MAX_SPO2_1600US			0x03 		//1600us = 16 bit resolution

// LED current values
#define MAX_IR_LED_0MA			0x00
#define MAX_IR_LED_11MA			0x03
#define MAX_IR_LED_21MA			0x06
#define MAX_IR_LED_31MA			0x09
#define MAX_IR_LED_40MA			0x0C
#define MAX_IR_LED_50MA			0x0F
#define MAX_RED_LED_0MA			(MAX_IR_LED_0MA  << 4)
#define MAX_RED_LED_11MA		(MAX_IR_LED_11MA << 4)
#define MAX_RED_LED_21MA		(MAX_IR_LED_21MA << 4)
#define MAX_RED_LED_31MA		(MAX_IR_LED_31MA << 4)
#define MAX_RED_LED_40MA		(MAX_IR_LED_40MA << 4)
#define MAX_RED_LED_50MA		(MAX_IR_LED_50MA << 4)



/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

void MAX30100_Initialise();
void MAX30100_Reset();
uint8_t MAX30100_ReadStatus();
uint8_t MAX30100_IsHRReady();
void MAX30100_SetHRMode();
void MAX30100_SetSampleRate(uint8_t sampleRate);
void MAX30100_SetPulseWidth(uint8_t width);
void MAX30100_SetIRCurrent(uint8_t current);
void MAX30100_SetRedCurrent(uint8_t current);
void MAX30100_Shutdown();
uint16_t MAX30100_ReadHROnly();
void MAX30100_EnableInterrupt(uint8_t int_en);
void MAX30100_FlushFIFO();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MAX30100_H_ */
