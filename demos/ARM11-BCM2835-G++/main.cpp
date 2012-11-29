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

#include "ch.hpp"
#include "hal.h"
#include "chprintf.h"
#include "Matrix"
#include "ERRORH"

//#include "lcomatrix.h"
using namespace chibios_rt;
using namespace PMPC;

extern BaseSequentialStream * MSD1;
/*
 * Application entry point.
 */
int main(void) {
	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	System::Init();

	/*
	 * Activates the serial driver using the driver default configuration.
	 */
	sdStart(&SD1, NULL);
	chprintf((BaseSequentialStream *) &SD1, "MPC test");
	
	MSD1 = (BaseSequentialStream *) &SD1;
	ERRORH::throwerror("asa");


	Matrix<double> DeltaU(3, 1);
	Matrix<double> Delta(DeltaU);
	//MSD1 = (BaseSequentialStream *) &SD1;
	//DeltaU.print();
	/*Matrix<double> R = 1e-5
	 * Matrix<double>::Identity(mySysMat.C.rows() * 3,
	 mySysMat.C.rows() * nc);
	 Matrix<double> Q = 1e4 * Matrix<double>::Identity(np, np);*/

	return 0;
}
