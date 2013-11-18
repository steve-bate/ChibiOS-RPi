/*
 ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
 2011,2012 Giovanni Di Sirio.

 This file is part of ChibiOS/RT.

 ChibiOS/RT is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 ChibiOS/RT is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file    templates/serial_lld.c
 * @brief   Serial Driver subsystem low level driver source template.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

SerialDriver SD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = { 115200 /* default baud rate */
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void output_notify(GenericQueue *qp) {
	UNUSED(qp);
	/* Enable tx interrupts.*/
	UART->IMSC.BIT.BIT5 = 1;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

void sd_lld_serve_interrupt(SerialDriver *sdp) {

	//Checking whether is it UART IRQ request
	if (INTC_REGS->Pending2.BIT.BIT24) {

		//Checking where does it come from the IRQ
		if (UART->MIS.BIT.BIT4) {
			chSysLockFromIsr()
			;
			//Reading data until the FIOF is empty
			while (!(UART0_FR->BIT.BIT4)) {
				//FIXME: The size of the buffer need to be checked
				sdIncomingDataI(sdp, UART0_DR->REG);
			}
			//Clearing the RX interruption flag
			UART->ICR.BIT.BIT4 = 0;
			chSysUnlockFromIsr();
		}

		//Checking where does it come from the IRQ
		if (UART->MIS.BIT.BIT5) {
			chSysLockFromIsr()
			;

			//Sending the data until the FIFO is full or no more data
			while (!(UART0_FR->BIT.BIT5)) {
				msg_t data = sdRequestDataI(sdp);
				//FIXME: What if I am sending Q_OK?
				if (data < Q_OK) {
					/* Disable TX interrupts.*/
					UART->IMSC.BIT.BIT5 = 0;
					//No more date, breaking out
					break;
				} else {
					UART0_DR->REG = data;
				}
			}
			//Clear the TX interrupt flag
			UART->ICR.BIT.BIT5 = 0;
			chSysUnlockFromIsr();
		}
	}
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {
	sdObjectInit(&SD1, NULL, output_notify);
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {
	UNUSED(sdp);

	if (config == NULL)
		config = &default_config;

	uint32_t ra;
	INTC_REGS->Disable2.BIT.BIT24 = 1;

	UART->CR.REG = 0;

	ra = GPIO_REGS->GPFSEL[1].REG;
	ra &= ~(7 << 12);
	ra |= 4 << 12;
	GPIO_REGS->GPFSEL[1].REG = ra;

	//bcm2835_gpio_fnsel(14, GPFN_IN);
	//bcm2835_gpio_fnsel(15, GPFN_IN);

	GPIO_REGS->GPPUD[0].REG = 0;

	bcm2835_delay(150);
	GPIO_REGS->GPPUDCLK[0].REG = (3 << 14);
	bcm2835_delay(150);

	GPIO_REGS->GPPUDCLK[0].REG = 0;

	//Clearing the interruptions
	UART->ICR.REG = 0x7FF;

	//Setting the baud rate
	ra = (UART_CLOCK << 2) / (config->baud_rate);
	UART->IBRD.REG = ra >> 6;
	UART->FBRD.REG = ra & 0x3F;

	//4bit is enabling the FIFO, 6:5bits-8bit data
	UART->LCRH.REG = 0x70;

	//Enable the RX interruption. (4,5 is the RX,TX interruption enable bits)
	UART->IMSC.REG = 0x0030;

	//8,9bits enable TX,RX, 0bit-enable UART

	UART->CR.REG = 0x301;

	//Enable UART IRQS
	INTC_REGS->Enable2.BIT.BIT24 = 1;
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {
	UNUSED(sdp);
	INTC_REGS->Disable2.BIT.BIT24 = 1;
	UART->IMSC.REG = 0x0000;
}

uint8_t uart_recv(void) {
	while (1) {
		if (!(UART0_FR->BIT.BIT4))
			break;
	}
	return UART0_DR->REG;
}

void uart_send(uint8_t c) {
	while (1) {
		if (!(UART0_FR->BIT.BIT5))
			break;
	}
	UART0_DR->REG = c;
}

void uart_sendstr(const char *s) {
	char c;
	while ((c = *s++))
		uart_send(c);
}

void uart_sendhex(uint32_t d, bool_t newline) {
	uint32_t rb;
	uint32_t rc;

	rb = 32;
	while (1) {
		rb -= 4;
		rc = (d >> rb) & 0xF;
		if (rc > 9)
			rc += 0x37;
		else
			rc += 0x30;
		uart_send(rc);
		if (rb == 0)
			break;
	}

	uart_send(0x20);

	if (newline) {
		uart_send(0x0D);
		uart_send(0x0A);
	}
}

#endif /* HAL_USE_SERIAL */

/** @} */
