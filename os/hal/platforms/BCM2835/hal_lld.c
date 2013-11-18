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
 * @file    templates/hal_lld.c
 * @brief   HAL Driver subsystem low level driver source template.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Start the system timer
 *
 * @notapi
 */
static void systimer_init(void) {
	// 1 MHz clock, Counter=1000, 1 ms tick
	ARM_TIMER_CTL->REG = 0x003E0000;
	ARM_TIMER_LOD->REG = 1000 - 1;
	ARM_TIMER_RLD->REG = 1000 - 1;
	ARM_TIMER_DIV->REG = 0x000000F9;
	ARM_TIMER_CLI->REG = 0;
	ARM_TIMER_CTL->REG = 0x003E00A2;

	INTC_REGS->IRQBasic.BIT.BIT0 = 1;
}

/**
 * @brief Process system timer interrupts, if present.
 *
 * @notapi
 */
static void systimer_serve_interrupt(void) {

	if (INTC_REGS->IRQBasic.BIT.BIT0) {
		// Update the system time
		chSysLockFromIsr()
		;
		chSysTimerHandlerI();
		chSysUnlockFromIsr();

		// Clear timer interrupt
		ARM_TIMER_CLI->REG = 0;
	}
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief Interrupt handler
 *
 */CH_IRQ_HANDLER(IrqHandler) {
	CH_IRQ_PROLOGUE();

	systimer_serve_interrupt();

#if HAL_USE_SERIAL
	sd_lld_serve_interrupt(&SD1);
#endif

#if HAL_USE_I2C
	i2c_lld_serve_interrupt(&I2C0);
#endif

#if HAL_USE_SPI
	spi_lld_serve_interrupt(&SPI0);
#endif

#if HAL_USE_GPT
	gpt_lld_serve_interrupt();
#endif

#if HAL_USE_EXT
	ext_lld_serve_interrupt(&EXTD1);
#endif

	CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Synchronize function for short delays.
 *
 */
void delayMicroseconds(uint32_t n) {
	uint32_t compare = SYSTIMER_CLO->REG + n;
	while (SYSTIMER_CLO->REG < compare)
		;
}

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {
	systimer_init();
}

/**
 * @brief Start watchdog timer
 */
void watchdog_start(uint32_t timeout) {
	/* Setup watchdog for reset */
	uint32_t pm_rstc = PM_RSTC->REG;

	//* watchdog timer = timer clock / 16; need password (31:16) + value (11:0) */
	uint32_t pm_wdog = PM_PASSWORD | (timeout & PM_WDOG_TIME_SET);
	pm_rstc = PM_PASSWORD | (pm_rstc & PM_RSTC_WRCFG_CLR)
			| PM_RSTC_WRCFG_FULL_RESET;
	PM_WDOG->REG = pm_wdog;
	PM_RSTC->REG = pm_rstc;
}

/**
 * @brief Start watchdog timer
 */
void watchdog_stop(void) {
	PM_RSTC->REG = PM_PASSWORD | PM_RSTC_RESET;
}

/**
 * @brief Get remaining watchdog time.
 */
uint32_t watchdog_get_remaining(void) {
	return PM_WDOG->REG & PM_WDOG_TIME_SET;
}

/** @} */
