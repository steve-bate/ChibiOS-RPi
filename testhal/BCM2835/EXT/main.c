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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *p)
{
	(void) p;
	chRegSetThreadName("blinker");
	while (1)
	{
		chThdSleepMilliseconds(1000);
		palClearPad(ONBOARD_LED_PORT, ONBOARD_LED_PAD);
		chThdSleepMilliseconds(1000);
		palClearPad(GPIO4_PORT, GPIO4_PAD);
	}
	return 0;
}


static void GPIO25Callback(EXTDriver *extp, expchannel_t channel)
{
	UNUSED(extp);
	UNUSED(channel);
	uart_sendhex(channel, TRUE);
	palSetPad(GPIO4_PORT, GPIO4_PAD);
	palSetPad(ONBOARD_LED_PORT, ONBOARD_LED_PAD);
}

static EXTConfig extconf =
	{
		{
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_FALLING, GPIO25Callback },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL },
			{ DETECT_NONE, NULL } }

	};
/*
 * Application entry point.
 */
int main(void)
{
	halInit();
	chSysInit();

	sdStart(&SD1, NULL );
	extStart(&EXTD1, &extconf);

	chprintf((BaseSequentialStream *) &SD1, "BCM2835 GPIO Demonstration\r\n");

	palSetPadMode(ONBOARD_LED_PORT, ONBOARD_LED_PAD, PAL_MODE_OUTPUT);

	palSetPadMode(GPIO4_PORT, GPIO4_PAD, PAL_MODE_OUTPUT); palSetPad(GPIO4_PORT, GPIO4_PAD);
	chprintf((BaseSequentialStream *) &SD1, "BCM2835 GPIO Ext set\r\n");

	/*
	 * Creates the blinker thread.
	 */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL );

	/*
	 * Events servicing loop.
	 */
	chThdWait(chThdSelf());

	return 0;
}
