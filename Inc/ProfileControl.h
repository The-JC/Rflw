/**
 *  This file is part of Rflw (https://github.com/The-JC/Rflw).
 *  Copyright (C) 2020  Julian Hellner <julian@hellner.cc>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PROFILECONTROL_H_
#define PROFILECONTROL_H_

#include "stm32f1xx_hal.h"

#include "control.h"

typedef struct {
	const uint32_t time;
	const uint16_t temprature;
} DATAPOINT_t;

extern const DATAPOINT_t basicPoints[2];
extern const DATAPOINT_t advancedPoints[3];

volatile DATAPOINT_t *currentCurvePtr;

/**
 * This function sets w of @control at the time according to the Curve
 *
 * @param curve: curve to follow
 * @returns 0 or 1 -> 1 finished; 0 failed
 */
void profileControlTask();

#endif /* PROFILECONTROL_H_ */
