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
 * @file    templates/pal_lld.c
 * @brief   PAL subsystem low level driver template.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

gpio_port_t IOPORT0;
gpio_port_t IOPORT1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_gpio_pud(ioportid_t port, ioportmask_t mask, uint32_t pud_mode) {
	// pg. 101 BCM2835 ARM Peripherals Reference
	GPIO_REGS->GPPUD[0].REG = pud_mode;
	bcm2835_delay(150);
	*port->gppudclk = mask;
	bcm2835_delay(150);
	GPIO_REGS->GPPUD[0].REG = 0;
	*port->gppudclk = 0;
}

static void set_gpio_in(ioportid_t port, ioportmask_t mask) {
	int i;
	for (i = 0; i < 32; i++) {
		unsigned int bit = mask & 1;
		if (bit)
			bcm2835_gpio_fnsel(i + port->pin_base, GPFN_IN);
		mask >>= 1;
	}
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void _pal_lld_init(const PALConfig* config) {

	(void) config;

	IOPORT0.gpset = &GPIO_REGS->GPSET[0].REG;
	IOPORT0.gpclr = &GPIO_REGS->GPCLR[0].REG;
	IOPORT0.gplev = &GPIO_REGS->GPLEV[0].REG;
	IOPORT0.gppudclk = &GPIO_REGS->GPPUDCLK[0].REG;

	IOPORT0.gpeds = &GPIO_REGS->GPEDS[0].REG;
	IOPORT0.gpren = &GPIO_REGS->GPREN[0].REG;
	IOPORT0.gpfen = &GPIO_REGS->GPFEN[0].REG;
	IOPORT0.gphen = &GPIO_REGS->GPHEN[0].REG;
	IOPORT0.gplen = &GPIO_REGS->GPLEN[0].REG;
	IOPORT0.gparen = &GPIO_REGS->GPAREN[0].REG;
	IOPORT0.gpfen = &GPIO_REGS->GPAFEN[0].REG;

	set_gpio_in(&IOPORT0, 0xFFFFFFFF);

	IOPORT1.gpset = &GPIO_REGS->GPSET[1].REG;
	IOPORT1.gpclr = &GPIO_REGS->GPCLR[1].REG;
	IOPORT1.gplev = &GPIO_REGS->GPLEV[1].REG;
	IOPORT1.gppudclk = &GPIO_REGS->GPPUDCLK[1].REG;

	IOPORT1.gpeds = &GPIO_REGS->GPEDS[1].REG;
	IOPORT1.gpren = &GPIO_REGS->GPREN[1].REG;
	IOPORT1.gpfen = &GPIO_REGS->GPFEN[1].REG;
	IOPORT1.gphen = &GPIO_REGS->GPHEN[1].REG;
	IOPORT1.gplen = &GPIO_REGS->GPLEN[1].REG;
	IOPORT1.gparen = &GPIO_REGS->GPAREN[1].REG;
	IOPORT1.gpfen = &GPIO_REGS->GPAFEN[1].REG;

	IOPORT1.pin_base = 32;
	set_gpio_in(&IOPORT1, 0xFFFFFFFF);
}

void _pal_lld_setgroupmode(ioportid_t port, ioportmask_t mask, uint32_t mode) {
	int i;
	switch (mode) {
	case PAL_MODE_INPUT:
		set_gpio_in(port, mask);
		set_gpio_pud(port, mask, GPIO_PUD_OFF);
		break;
	case PAL_MODE_INPUT_PULLUP:
		set_gpio_in(port, mask);
		set_gpio_pud(port, mask, GPIO_PUD_PULLUP);
		break;
	case PAL_MODE_INPUT_PULLDOWN:
		set_gpio_in(port, mask);
		set_gpio_pud(port, mask, GPIO_PUD_PULLDOWN);
		break;
	case PAL_MODE_OUTPUT:
	case PAL_MODE_OUTPUT_PUSHPULL:
	case PAL_MODE_OUTPUT_OPENDRAIN:
		for (i = 0; i < 32; i++) {
			unsigned int bit = mask & 1;
			if (bit)
				bcm2835_gpio_fnsel(i + port->pin_base, GPFN_OUT);
			mask >>= 1;
		}
		break;
	}
}

void _pal_lld_writeport(ioportid_t port, ioportmask_t bits) {
	*(port->gpset) = port->latch = bits;
	*(port->gpclr) = ~bits;
}

#endif /* HAL_USE_PAL */

/** @} */
