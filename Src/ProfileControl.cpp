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

#include "ProfileControl.h"

#include "config.h"
#include "cmsis_os.h"

const DATAPOINT_t basicPoints[2] =  {{60, 80}, {30, 190}};
const DATAPOINT_t advancedPoints[3] =  {{210, 140},  {60, 160}, {90, 200}};

void profileControlTask(void const * argument) {
	for(int i=0; i<REFLOW_MAX_POINTS; i++) {
		if((currentCurvePtr+i)->temprature==0) break;
		setTemperature((currentCurvePtr+i)->temprature);

		// Wait till set temperature is reached then start timer
		// *ToDo* thermal runaway protection
		while(!reachedTemperature()) {
			osDelay(1);
		}

		osDelay((currentCurvePtr+i)->time*1000);
	}
}
