/*
 * Si1147.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef Si1147_H_
#define Si1147_H_

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
#define SIL_ADDR				0x60		//7 bit read address
#define SIL_DEV_ID				0x47		//Default value of part ID register for Si1147
#define SIL_HW_KEY				0x17		//Value required to intialise Si1147

// I2C register map
#define SIL_REG_PART_ID			0x00		//Part ID register to identify Si114x
#define SIL_REG_HW_KEY 			0x07		//Write value to initialise system
#define SIL_REG_COMMAND 		0x18		//Command request register
#define SIL_REG_RESPONSE		0x20		//Command response register
#define SIL_REG_CHIP_STAT		0x30		//Device status (operational mode)
//#define SIL_REG_FIFO_WR_PTR		0x02		//FIFO write pointer
//#define SIL_REG_FIFO_OVF_CNTR	0x03		//Over flow counter
//#define SIL_REG_FIFO_RD_PTR		0x04		//FIFO read pointer
//#define SIL_REG_FIFO_DATA		0x05		//FIFO data register
//#define SIL_REG_MODE_CONFIG		0x06		//Mode configuration
//#define SIL_REG_SPO2_CONFIG		0x07		//SPO2 configuration
//#define SIL_REG_LED_CONFIG		0x09		//LED configuration
//#define SIL_REG_TEMP_INT		0x16		//Temperature Integer
//#define SIL_REG_TEMP_FRAC		0x17		//Temperature Fraction
#define SIL_REG_REV_ID			0xFE		//Revision ID (read only)
//#define SIL_REG_DEV_ID			0xFF		//Part ID (0x11)

//Commands
#define SIL_CMD_RESET			0x01		//Reset command

//// I2C Mode register values
//#define SIL_MODE_SHDN			0x80		//Shutdown
//#define SIL_MODE_RESET			0x40		//Reset
//#define SIL_MODE_TEMP_EN		0x08		//Entable temperature measurement
//#define SIL_MODE_HR_ONLY		0x02		//Enable Heart Rate only mode (IR LED only, not RED)
//#define SIL_MODE_SPO2			0x03		//Enable SPO2 mode (IR & RED LED)

////I2C Interrupt register bit fields
//#define SIL_INT_A_FULL			0x80		//FIFO almost full (15/16 samples)
//#define SIL_INT_TEMP_RDY		0x40		//Temperature measurement ready
//#define SIL_INT_HR_RDY			0x20		//Heart rate measurement ready
//#define SIL_INT_SPO2_RDY		0x10		//SPO2 measurement ready
//#define SIL_INT_PWR_RDY			0x01		//Power ready
//
//// FIFO index
//#define SIL_FIFO_IR_UPPER		0x0
//#define SIL_FIFO_IR_LOWER		0x1
//#define SIL_FIFO_RED_UPPER		0x2
//#define SIL_FIFO_RED_LOWER		0x3
//
//// SPO2 register values
//#define SIL_SPO2_HIRES_EN		0x40		//Enable high resolution mode (16bit, 1600us)
//#define SIL_SPO2_50SPS			0x00		//50 samples per second
//#define SIL_SPO2_100SPS			0x04		//100 samples per second
//#define SIL_SPO2_1000SPS		0x1C		//1000 samples per second
//#define SIL_SPO2_200US			0x00 		// 200us = 13 bit resolution
//#define SIL_SPO2_400US			0x01 		// 400us = 14 bit resolution
//#define SIL_SPO2_800US			0x02 		// 800us = 15 bit resolution
//#define SIL_SPO2_1600US			0x03 		//1600us = 16 bit resolution
//
//// LED current values
//#define SIL_IR_LED_0MA			0x00
//#define SIL_IR_LED_11MA			0x03
//#define SIL_IR_LED_21MA			0x06
//#define SIL_IR_LED_31MA			0x09
//#define SIL_IR_LED_40MA			0x0C
//#define SIL_IR_LED_50MA			0x0F
//#define SIL_RED_LED_0MA			(SIL_IR_LED_0MA  << 4)
//#define SIL_RED_LED_11MA		(SIL_IR_LED_11MA << 4)
//#define SIL_RED_LED_21MA		(SIL_IR_LED_21MA << 4)
//#define SIL_RED_LED_31MA		(SIL_IR_LED_31MA << 4)
//#define SIL_RED_LED_40MA		(SIL_IR_LED_40MA << 4)
//#define SIL_RED_LED_50MA		(SIL_IR_LED_50MA << 4)



/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

void Si1147_Initialise();
uint8_t Si1147_Command(uint8_t command);
void Si1147_Reset();
uint8_t Si1147_ReadStatus();
//uint8_t Si1147_IsHRReady();
//void Si1147_SetHRMode();
//void Si1147_SetSampleRate(uint8_t sampleRate);
//void Si1147_SetPulseWidth(uint8_t width);
//void Si1147_SetIRCurrent(uint8_t current);
//void Si1147_SetRedCurrent(uint8_t current);
//void Si1147_Shutdown();
//uint16_t Si1147_ReadHROnly();
//void Si1147_EnableInterrupt(uint8_t int_en);
//void Si1147_FlushFIFO();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* Si1147_H_ */
