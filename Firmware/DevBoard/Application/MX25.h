/*
 * MX25.h
 *
 *  Created on: 7 Feb 2016
 *      Author: Adam
 */

#ifndef MX25_H_
#define MX25_H_

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

//Identifiers
#define MX25_MAN_ID				0xC2		//Manufacturer ID
#define MX25_DEV_ID				0x17		//Device ID

// Config Commands
#define MX25_CMD_REMS	 		0x90		//Read manufactuere and device ID
#define MX25_CMD_RES	 		0xAB		//Read device ID
#define MX25_CMD_RDID	 		0x9F		//Read JEDEC ID (1byte man ID & 2byte dev ID)
#define MX25_CMD_RST	 		0x99		//Reset memory
#define MX25_CMD_RSTEN	 		0x66		//Reset enable (RSTEN then RST
#define MX25_CMD_NOP	 		0x00		//No operation


#define MX25_CMD_RDSR	 		0x05		//Read satus register
#define MX25_CMD_WRSR	 		0x01		//Write status register
#define MX25_CMD_RDCR	 		0x15		//Read configuration register
#define MX25_CMD_WREN	 		0x06		//Write enable
#define MX25_CMD_WRDI	 		0x04		//Write disable
#define MX25_CMD_DP		 		0xB9		//Deep power down

//read/write command
#define MX25_CMD_READ		 	0x03		//Read
#define MX25_CMD_PP		 		0x02		//Page program (write)
#define MX25_CMD_CE		 		0x60		//Chip erase (or 0xC7)
#define MX25_CMD_BURST	 		0xC0		//Burst mode

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

void MX25_Initialise();
void MX25_Reset();
uint8_t MX25_CheckID();
uint8_t MX25_ReadStatus();
uint16_t MX25_ReadConfig();
void MX25_Powerdown();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MX25_H_ */
