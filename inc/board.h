/*
 * @brief NGX Xplorer 4330 board file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __BOARD_H_
#define __BOARD_H_

#include "chip.h"
/* board_api.h is included at the bottom of this file after DEBUG setup */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BOARD_NGX_XPLORER_4330 LPC4330 NGX Xplorer board support software API functions
 * @ingroup LPCOPEN_43XX_BOARD_NGX4330
 * The board support software API functions provide some simple abstracted
 * functions used across multiple LPCOpen board examples. See @ref BOARD_COMMON_API
 * for the functions defined by this board support layer.<br>
 * @{
 */

/** @defgroup BOARD_NGX_XPLORER_4330_OPTIONS BOARD: LPC4330 NGX Xplorer board options
 * This board has options that configure its operation at build-time.<br>
 * @{
 */

/** Define DEBUG_SEMIHOSTING along with NDEBUG not defined to enable IO support
    via semihosting. You may need to use a C library that supports
	semihosting with this option.
 */

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
	Board_UARTPutSTR functions. */
#define DEBUG_UART LPC_USART2

/**
 * @}
 */

/* Board name */
#define BOARD_CIAA_NXP


/* Build for RMII interface */
#define USE_RMII
#define BOARD_ENET_PHY_ADDR	1

/* USB1 VBUS Enable GPIO pins */
#define USB1_VBUS_PORT_NUM          2
#define USB1_VBUS_PIN_NUM           5
#define USB1_VBUS_PIN_CFG           (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC4)
#define USB1_VBUS_GPIO_PORT_NUM     5
#define USB1_VBUS_GPIO_BIT_NUM      5

/**
 * @brief	Sets up I2C Fast Plus mode
 * @return	Nothing
 * @note	This function must be called before calling
 *          Chip_I2C_SetClockRate() to set clock rates above
 *          normal range 100KHz to 400KHz. Only I2C0 supports
 *          this mode.
 */
STATIC INLINE void Board_I2C_EnableFastPlus()
{
	Chip_SCU_I2C0PinConfig(I2C0_FAST_MODE_PLUS);
}

/**
 * @brief	Disable I2C Fast Plus mode and enables default mode
 * @return	Nothing
 * @sa		Board_I2C_EnableFastPlus()
 */
STATIC INLINE void Board_I2C_DisableFastPlus()
{
	Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);
}

/**
 * @brief	Enable VBUS to USB1 port in Host mode
 * @return	Nothing
 * @sa		Board_USB1_DisableVbus()
 */
__STATIC_INLINE void Board_USB1_EnableVbus(void)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, USB1_VBUS_GPIO_PORT_NUM, USB1_VBUS_GPIO_BIT_NUM);
	Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, USB1_VBUS_GPIO_PORT_NUM, USB1_VBUS_GPIO_BIT_NUM);
}

/**
 * @brief	Disable VBUS to USB1 port
 * @return	Nothing
 * @sa		Board_USB1_EnableVbus()
 */
__STATIC_INLINE void Board_USB1_DisableVbus(void)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, USB1_VBUS_GPIO_PORT_NUM, USB1_VBUS_GPIO_BIT_NUM);
	Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, USB1_VBUS_GPIO_PORT_NUM, USB1_VBUS_GPIO_BIT_NUM);
}

/**
 * @brief	Initialize pin muxing for SSP interface
 * @param	pSSP	: Pointer to SSP interface to initialize
 * @return	Nothing
 */
void Board_SSP_Init(LPC_SSP_T *pSSP);

/**
 * @brief	Returns the MAC address assigned to this board
 * @param	mcaddr : Pointer to 6-byte character array to populate with MAC address
 * @return	Nothing
 */
void Board_ENET_GetMacADDR(uint8_t *mcaddr);

/**
 * @brief	Initialize pin muxing for a UART
 * @param	pUART	: Pointer to UART register block for UART pins to init
 * @return	Nothing
 */
void Board_UART_Init(LPC_USART_T *pUART);

/**
 * @brief	Initialize DAC
 * @param	pDAC	: Pointer to DAC register interface used on this board
 * @return	Nothing
 */
void Board_DAC_Init(LPC_DAC_T *pDAC);

/**
 * @brief	Initialize ADC
 * @return	Nothing
 */
STATIC INLINE void Board_ADC_Init(void){}

/**
 * @brief	Initialize Pinmuxing for the LCD interface
 * @return	Nothing
 */

#define US_CYCLES    OscRateIn/1000000    /* 1 uS */

static inline void udelay(int32_t us)
{
	volatile uint32_t i;
	while (us--) {
		for (i = 0; i < US_CYCLES; i++) {
			; /* Burn cycles. */
		}
	}
}

/**
 * @}
 */

#include "board_api.h"

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
