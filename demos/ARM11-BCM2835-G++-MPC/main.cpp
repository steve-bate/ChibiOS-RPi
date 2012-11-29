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
#include "MPC"
#include "KF"

using namespace chibios_rt;
using namespace PMPC;

extern BaseSequentialStream * MSD1;

template<typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}
/*
 * Brushed DC motor MPC controller
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *p) {
	(void) p;
	/*
	 * Defining the variables
	 */
	//Control horizon
	int nc = 4;
	// Prediction horizon
	int np = 4;
	//Control signal
	Matrix<double> u(1, 1);
	// Sampling time
	double Ts = 5e-5;

	/*
	 * Initializing the MPC
	 */
	MPC mySysMat(2, 1, 1);

	Matrix<double> DeltaU(np, 1);

	Matrix<double> R = 1e-5
			* Matrix<double>::Identity(mySysMat.C.rows() * nc, mySysMat.C.rows() * nc);
	Matrix<double> Q = 1e4 * Matrix<double>::Identity(np, np);

	Matrix<double> x0(2, 1);
	Matrix<double> x00(2, 1);
	//Reference signal
	Matrix<double> y_ref(100000, 1);
	y_ref.rblock(0, 0, 50000, 1) = 100 * Matrix<double>::Ones(50000, 1);
	y_ref.rblock(50000, 0, 100000, 1) = -100 * Matrix<double>::Ones(50000, 1);

	//double start = now();

	//DC motor parameters
	double Rm = 0.35;
	double Km = 0.0296;
	double Ke = 0.0296;
	double b = 6.7 * 10e-5;
	double J = 2.9 * 10e-6;
	//double Fc = 0.02;
	double L = 25 * 10e-6;

	//Initializing the system matrices
	mySysMat.Fi << 1 - Ts * (Rm / L), -Ts * (Ke / L), Ts * (Km / J), 1 - Ts * (b / J);

	mySysMat.Ga << Ts * 1 / L, 0;

	mySysMat.C << 0, 1;

	mySysMat.calcMPCFi(np);
	mySysMat.calcMPCGa(np);
	mySysMat.calcMPCGy(np, nc);

	/*
	 * Initializing the Kalman filter
	 */

	Matrix<double> kQ = 1e-5 * Matrix<double>::Identity(mySysMat.Ga.rows(), mySysMat.Ga.rows());
	Matrix<double> kR = 1e4 * Matrix<double>::Identity(mySysMat.C.rows(), mySysMat.C.rows());
	Matrix<double> kP = 1e-10 * Matrix<double>::Identity(mySysMat.Ga.rows(), mySysMat.Ga.rows());
	KF myKalman(x0, mySysMat.C, kQ, kR, kP);

#ifdef CHIBIOS_DEBUG
	ERRORH::print("Calculation Fi,Ga,Gy took : ");
	//ERRORH::print((double) (now() - start));
#endif

	Matrix<double> calcT(1, y_ref.rows());
	Matrix<double> u_hist(1, y_ref.rows());
	Matrix<double> w_hist(1, y_ref.rows());
	Matrix<double> w_est_hist(1, y_ref.rows());

	for (unsigned i = 0; i < y_ref.rows() - np; i++) {

		//start = now();
		//Calculating the error

		DeltaU = mySysMat.calcContSig(mySysMat.calcError(y_ref.block(i, 0, i + np, 1), x00, u), Q,
				R);

		u(0, 0) += DeltaU(0, 0);

		//We are not including this in the time measurement
		if (abs(u(0, 0)) > 5)
			u(0, 0) = sgn(u(0, 0)) * 5;

		u_hist(0, i) = u(0, 0);

		myKalman.updateState(mySysMat.Fi, mySysMat.Ga, u);

		x00 = myKalman.estimate(mySysMat.Fi, x0.block(0, 0, 1, 1));

		//Simulating the system
		x0 = mySysMat.Fi * x0 + mySysMat.Ga * u;

		w_hist(0, i) = x0(1, 0);
		w_est_hist(0, i) = x00(1, 0);
		//Storing the calculation time
		//calcT(0, i) = (double) (now() - start);

	}
#ifdef CHIBIOS_DEBUG
	ERRORH::print("Minimum : ");
	//ERRORH::print((double) calcT.min());
	ERRORH::print("Maximum : ");
	//ERRORH::print((double) calcT.max());
#endif
	return 0;
}

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

	/*
	 * Redirect the output to the serial port
	 */
	MSD1 = (BaseSequentialStream *) &SD1;

	/*
	 * Creates the blinker thread.
	 */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

	/*
	 * Events servicing loop.
	 */
	chThdWait(chThdSelf());

	return 0;
}
