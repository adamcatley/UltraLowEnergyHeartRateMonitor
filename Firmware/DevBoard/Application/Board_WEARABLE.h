/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       Board.h
 *
 *  @brief      CC2650EM_7ID Board Specific header file.
 *              The project options should point to this file if this is the
 *              CC2650EM you are developing code for.
 *
 *  The CC2650 header file should be included in an application as follows:
 *  @code
 *  #include <Board.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __CC2650EM_7ID_H__
#define __CC2650EM_7ID_H__

#ifdef __cplusplus
extern "C" {
#endif

/** ============================================================================
 *  Symbol by generic Board.c to include the correct kit specific Board.c
 *  ==========================================================================*/
#define CC2650EM_7ID


/** ============================================================================
 *  Includes
 *  ==========================================================================*/
#include <ti/drivers/PIN.h>
#include <driverlib/ioc.h>

/** ============================================================================
 *  Externs
 *  ==========================================================================*/
extern PIN_Config BoardGpioInitTable[];

/** ============================================================================
 *  Defines
 *  ==========================================================================*/

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                <pin mapping>
 */
/* Leds */
#define Board_LED_ON                        1               /* LEDs are active high */
#define Board_LED_OFF                       0

//SPI
#define Board_SPI_CLK                		IOID_2
#define Board_SPI_MOSI                  	IOID_3
#define Board_SPI_MISO                  	IOID_4

/* I2C */
#define Board_I2C_SCL                       IOID_9
#define Board_I2C_SDA                       IOID_10

/* Buttons */
#define Board_BUTTON	                    IOID_12
#define Board_BUTTON_2	                    IOID_11

/* Display */
#define Board_DISPLAY_EXTCOMIN              IOID_1
#define Board_DISPLAY_ENABLE                PIN_UNASSIGNED
#define Board_DISPLAY_EXTMODE               PIN_UNASSIGNED
#define Board_DISPLAY_CS                    IOID_0
#define Board_DISPLAY_MOSI                  Board_SPI_MOSI
#define Board_DISPLAY_CLK                   Board_SPI_CLK

//Temporary
#define Board_AFE_MOSI                      Board_SPI_MISO
#define Board_AFE_CS                        Board_SPI_MISO
#define Board_AFE_MISO                      Board_SPI_MISO
#define Board_AFE_SCLK                      Board_SPI_MISO

//Other
#define Board_ADXL_CS                      	IOID_5
#define Board_FLASH_CS						IOID_6
#define Board_ADXL_INT                      IOID_7
#define Board_MAX_INT                       IOID_8
#define Board_Si11XX_INT                    IOID_13
#define Board_BATTERY_SENSE                 IOID_14

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
/* Generic I2C instance identifiers */
#define Board_I2C                   CC2650_I2C0
/* Generic SPI instance identifiers */
#define Board_AFE_SPI               CC2650_SPI0
#define Board_DISPLAY_SPI 			CC2650_SPI1
/* Generic UART instance identifiers */
#define Board_UART                  CC2650_UART0

/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC2650_I2CName
 *  @brief  Enum of I2C names on the CC2650 dev board
 */
typedef enum CC2650_I2CName {
    CC2650_I2C0 = 0,

    CC2650_I2CCOUNT
} CC2650_I2CName;

/*!
 *  @def    CC2650_SPIName
 *  @brief  Enum of SPI names on the CC2650 dev board
 */
typedef enum CC2650_SPIName {
    CC2650_SPI0 = 0,
    CC2650_SPI1,
    CC2650_SPICOUNT
} CC2650_SPIName;

/*!
 *  @def    CC2650_UARTName
 *  @brief  Enum of UARTs on the CC2650 dev board
 */
typedef enum CC2650_UARTName {
    CC2650_UART0 = 0,
    CC2650_UARTCOUNT
} CC2650_UARTName;

/*!
 *  @def    CC2650_UdmaName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2650_UdmaName {
    CC2650_UDMA0 = 0,
    CC2650_UDMACOUNT
} CC2650_UdmaName;

#ifdef __cplusplus
}
#endif

#endif /* __CC2650EM_H__ */
