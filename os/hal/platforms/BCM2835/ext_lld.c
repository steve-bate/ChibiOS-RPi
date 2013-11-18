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
 * @file    templates/ext_lld.c
 * @brief   EXT Driver subsystem low level driver source template.
 *
 * @addtogroup EXT
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "ext_lld.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXTD1 driver identifier.
 */
EXTDriver EXTD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/*
 *
 */
void enable_gpio_detect(uint8_t pin_nr, uint8_t type)
{
	uint32_t mask = (1 << pin_nr);
	uint32_t offset = pin_nr / 32;

	switch (type)
	{
		case DETECT_RISING:
		GPIO_REGS .GPREN[offset] |= mask;
		break;
		case DETECT_FALLING:
		GPIO_REGS .GPFEN[offset] |= mask;
		break;
		case DETECT_HIGH:
		GPIO_REGS .GPHEN[offset] |= mask;
		break;
		case DETECT_LOW:
		GPIO_REGS .GPLEN[offset] |= mask;
		break;
		case DETECT_RISING_ASYNC:
		GPIO_REGS .GPAREN[offset] |= mask;
		break;
		case DETECT_FALLING_ASYNC:
		GPIO_REGS .GPAFEN[offset] |= mask;
		break;
		case DETECT_NONE:
		break;
	}
}
/*
 *
 */
void enable_bank_interrupt(uint8_t pin_nr)
{
	uint32_t offset = pin_nr / 32;
	IRQ_ENABLE2 |= 1 <<(17+offset);
}

/*
 *
 */
void disable_bank_interrupt(uint8_t pin_nr)
{
	uint32_t offset = pin_nr / 32;
	IRQ_ENABLE2 ^= 1 <<(17+offset);

}
/*
 *
 */
void clear_gpio_interrupt(uint8_t pin_nr)
{
	uint32_t mask = (1 << (pin_nr % 32));
	uint32_t offset = pin_nr / 32;

	GPIO_REGS .GPEDS[offset] = mask;
}

/*
 *
 */

/*
 *
 */
uint32_t detect_gpio_interrupt(uint8_t pin_nr)
{
	uint32_t mask = (1 << (pin_nr % 32));
	uint32_t offset = pin_nr / 32;

	return GPIO_REGS .GPEDS[offset] & mask;
}
/*
 *
 */
void _ext_lld_serve_interrupt(EXTDriver *extp)
{
	uint8_t a;
	for(a=0; a<EXT_MAX_CHANNELS; ++a)
	{
		if( detect_gpio_interrupt(a) && extp->config->channels[a].cb != NULL )
		{
			clear_gpio_interrupt(a);
			extp->config->channels[a].cb(extp, a);
		}
	}
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void)
{

	/* Driver initialization.*/
	extObjectInit(&EXTD1);
}

/**
 * @brief   Configures and activates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_start(EXTDriver *extp)
{
	uint8_t a;
	for(a=0; a<EXT_MAX_CHANNELS; ++a)
	{
		if( extp->config->channels[a].mode )
		ext_lld_channel_enable(extp,a);
	}
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_stop(EXTDriver *extp)
{

	uint8_t a;
	for(a=0; a<EXT_MAX_CHANNELS; ++a)
	{
		if( extp->config->channels[a].mode)
		ext_lld_channel_disable(extp, a);
	}
}

/**
 * @brief   Enables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be enabled
 *
 * @notapi
 */
void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel)
{
	set_gpio_direction(channel, GPFN_IN);
	enable_gpio_detect(channel, extp->config->channels[channel].mode);
	enable_bank_interrupt(channel);
}

/**
 * @brief   Disables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be disabled
 *
 * @notapi
 */
void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel)
{
	UNUSED(extp);
	disable_bank_interrupt(channel);
}
/*
 *
 *
 */
void ext_lld_serve_interrupt(EXTDriver *extp)
{
	chSysLockFromIsr();
	_ext_lld_serve_interrupt(extp);
	chSysUnlockFromIsr();
}

#endif /* HAL_USE_EXT */

/** @} */
