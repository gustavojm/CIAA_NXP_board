/*
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
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
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "string.h"

#include "retarget.h"

/** @ingroup BOARD_NGX_XPLORER_18304330
 * @{
 */

/* SDIO Data pin configuration bits */
#define SDIO_DAT_PINCFG (SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_PULLUP | SCU_MODE_FUNC7)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System configuration variables used by chip driver */
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;

typedef struct {
	uint8_t port;
	uint8_t pin;
} io_port_t;

void Board_UART_Init(LPC_USART_T *pUART)
{
	/* UART2 directly wired to FT2232 IC; P7_1: U2_TXD, P7_2: U2_RXD */
	Chip_SCU_PinMuxSet(7, 1, (SCU_MODE_INACT | SCU_MODE_FUNC6));
	Chip_SCU_PinMuxSet(7, 2, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC6));
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaudFDR(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);
	Chip_UART_SetupFIFOS(DEBUG_UART, UART_FCR_FIFO_EN | UART_FCR_RX_RS| UART_FCR_TX_RS | UART_FCR_TRG_LEV0 | UART_FCR_DMAMODE_SEL);

	/* Enable UART Transmit */
	Chip_UART_TXEnable(DEBUG_UART);

#endif
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	/* Wait for space in FIFO */
	while ((Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE) == 0) {}
	Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
		return (int) Chip_UART_ReadByte(DEBUG_UART);
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(const char *str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	Chip_ENET_RMIIEnable(LPC_ETHERNET);
}

void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP1) {
		Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P1.3 => MISO1 */
		Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P1.4 => MOSI1 */
		Chip_SCU_PinMuxSet(0xF, 4, (SCU_PINIO_FAST | SCU_MODE_FUNC0));  									   /* PF.4 => SCK1  */
	} else {
		return;
	}
}

/* Initialize DAC interface for the board */
void Board_DAC_Init(LPC_DAC_T *pDAC)
{
	Chip_SCU_DAC_Analog_Config();
}

/**
 * @}
 */
