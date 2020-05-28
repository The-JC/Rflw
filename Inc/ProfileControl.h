/**
 * @file ProfileControl.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Controls the oven in Reflow mode
 * 
 * @copyright
 * This file is part of Rflw (https://lab.katurisoft.com/Julian/Rflw).\n
 * Copyright (C) 2020 Julian Hellner <julian@hellner.cc>\n
 * \n
 * This program is free software: you can redistribute it and/or modify\n
 * it under the terms of the GNU General Public License as published by\n
 * the Free Software Foundation, either version 3 of the License, or\n
 * any later version.\n
 * 
 * This program is distributed in the hope that it will be useful,\n
 * but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 * GNU General Public License for more details.\n
 * \n
 * You should have received a copy of the GNU General Public License\n
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.\n
 */

#ifndef PROFILECONTROL_H_
#define PROFILECONTROL_H_

#include "stm32f1xx_hal.h"

#include "control.h"

/**
 * @brief Datapoint of a curve
 */
typedef struct {
	const uint32_t time; ///< duration to next point
	const uint16_t temperature; ///< temperature to be set to
} DATAPOINT_t;

/// The most basic test curve
extern const DATAPOINT_t basicPoints[2];
/// Slightly advanced curve for better performance, works for most things
extern const DATAPOINT_t advancedPoints[3];

/// Pointer to curve that is currently in use
volatile DATAPOINT_t *currentCurvePtr;
/// Amount of points of the current curve
volatile uint32_t currentCurveLength;
/// Pointer to the current point of the curve
volatile DATAPOINT_t *currentPointPtr;

/**
 * @brief State of profile
 * 1: heating up
 * 3: running timer
 */
volatile uint8_t profileState;

enum {
	NON		= 0x0, /*!< Not running */
	HEATING	= 0x1, /*!< Controller is trying to reach temperature */
	RUNNING	= 0x3 /*!< Hold temperature, while timer is running */
} PROFILE_STATE_t;

/**
 * This function sets w of @control at the time according to the Curve
 *
 * @param curve: curve to follow
 * @returns 0 or 1 -> 1 finished; 0 failed
 */
void profileControlTask(void * argument);

#endif /* PROFILECONTROL_H_ */
