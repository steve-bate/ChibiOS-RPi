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

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

void sd_lld_serve_interrupt(SerialDriver *sdp) {

	//Checking whether is it UART IRQ request
	if (IRQ_PEND2 & BIT(24)) {
		//Checking where does it come from the IRQ
		if (UART0_FR & BIT(4)) {
			chSysLockFromIsr()
			;
			while (!UART0_FR.bit.BIT4) {
				//FIXME: The size of the buffer need to be checked
				sdIncomingDataI(sdp, uart_recv());
			}
			//Clearing the RX interruption flag
			UART0_ICR ^= 0x0020;
			chSysUnlockFromIsr();
		}

		//Checking where does it come from the IRQ
		if ((UART0_FR & BIT(5))) {
			chSysLockFromIsr()
			;

			//Sending the data until the FIFO is full
			while (!UART0_FR.bit.BIT5) {
				msg_t data = sdRequestDataI(sdp);
				if (data < Q_OK) {
					/* Disable tx interrupts.*/
					break;
				} else {
					uart_send((uint8_t) data);
				}
			}
			//Clear the TX interrupt flag
			UART0_ICR ^= 0x0010;
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

	IRQ_DISABLE2 |= BIT(24);

	UART0_CR = 0;

	ra = GPIO_GPFSEL1;
	ra &= ~(7 << 12);
	ra |= 4 << 12;
	GPIO_GPFSEL1 = ra;

	//bcm2835_gpio_fnsel(14, GPFN_IN);
	//bcm2835_gpio_fnsel(15, GPFN_IN);

	GPIO_GPPUD = 0;

	bcm2835_delay(150);
	GPIO_GPPUDCLK0 = (3 << 14);
	bcm2835_delay(150);

	GPIO_GPPUDCLK0 = 0;

	//Clearing the interruptions
	UART0_ICR = 0x7FF;

	//Setting the baud rate
	ra = (UART_CLOCK << 2) / (config->baud_rate);
	UART0_IBRD = ra >> 6;
	UART0_FBRD = ra & 0x3F;

	//4bit is enabling the FIFO, 6:5bits-8bit data
	UART0_LCRH = 0x70;

	//Enable the RX interruption. (4,5 is the RX,TX interruption enable bits)
	UART0_IMSC = 0x0030;

	//8,9bits enable TX,RX, 0bit-enable UART

	UART0_CR = 0x301;

	//Enable UART IRQS
	IRQ_ENABLE2 |= BIT(24);

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
	IRQ_DISABLE2 |= BIT(24);
	UART0_IMSC = 0x0000;
}

uint8_t uart_recv(void) {
	while (1) {
		if (!(UART0_FR & BIT(4)))
			break;
	}
	return UART0_DR;
}

void uart_send(uint8_t c) {
	while (1) {
		if (!(UART0_FR & BIT(5)))
			break;
	}
	UART0_DR = c;
}

void mini_uart_sendstr(const char *s) {
	char c;
	while ((c = *s++))
		mini_uart_send(c);
}

void mini_uart_sendhex(uint32_t d, bool_t newline) {
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
		mini_uart_send(rc);
		if (rb == 0)
			break;
	}

	mini_uart_send(0x20);

	if (newline) {
		mini_uart_send(0x0D);
		mini_uart_send(0x0A);
	}
}

#endif /* HAL_USE_SERIAL */

/** @} */
