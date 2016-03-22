/*
 * DRV2605L.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef DRV2605L_H_
#define DRV2605L_H_

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
#define DRV_ADDR				0x5A		//7 bit read address
#define DRV_DEV_ID				0x07		//Corresponds to DRV2605L version with ROM library

// I2C register map
#define DRV_REG_STATUS	 		0x00		//Status
#define DRV_REG_MODE 			0x01		//Mode
#define DRV_REG_RTP_INPUT		0x02		//RTP Input
#define DRV_REG_LIBRARY_SELECT	0x03		//Library Selection
#define DRV_REG_WAFEFORM_FIRST	0x04		//Waveform Sequencer (first)
#define DRV_REG_WAFEFORM_LAST	0x0B		//Waveform Sequencer (last)
#define DRV_REG_GO				0x0C		//GO
#define DRV_REG_FEEDBACK		0x1A		//Feedback Control
#define DRV_REG_CONTROL_1		0x1B		//Control 1
#define DRV_REG_CONTROL_2		0x1C		//Control 2
#define DRV_REG_CONTROL_3		0x1D		//Control 3
#define DRV_REG_CONTROL_4		0x1E		//Control 4
#define DRV_REG_CONTROL_5		0x1F		//Control 5
#define DRV_REG_VOLTAGE			0x21		//Voltage Monitor

// Mode register values
#define DRV_MODE_RESET			0x80		//Shutdown
#define DRV_MODE_STANDBY		0x40		//Reset
#define DRV_MODE_INTERNAL		0x00		//Waveforms triggered by go bit (0x0C)
#define DRV_MODE_RTP			0x05		//Real time playback mode
#define DRV_MODE_DIAGNOSTICS	0x06		//Test actuator when GO bit set
#define DRV_MODE_CALIBRATE		0x07		//Auto calibrate actuator after GO bit set

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

void DRV2605L_Initialise();
void DRV2605L_Reset();
uint8_t DRV2605L_ReadStatus();
void DRV2605L_Standby();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DRV2605L_H_ */
