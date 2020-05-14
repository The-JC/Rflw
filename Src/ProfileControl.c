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
#include "OvenMode.h"
#include "cmsis_os.h"

extern osThreadId controlInputHandle;
osThreadId controlReflowHandle;

extern uint8_t reachedTemperature();

const DATAPOINT_t basicPoints[2] =  {{60, 80}, {30, 190}};
const DATAPOINT_t advancedPoints[3] =  {{210, 140},  {60, 160}, {90, 200}};

void profileControlTask(void * argument) {
	for(uint32_t i=0; i<REFLOW_MAX_POINTS; i++) {
		currentPointPtr = currentCurvePtr + i;

		if(currentPointPtr->temperature==0 || i >= currentCurveLength) {
			currentPointPtr = NULL;
			setTemperature(0);
			setMode(EVENT_MENU | EVENT_UPADTE); // Set back to menu mode
			vTaskSuspend(controlInputHandle);	// Suspend Input Handle Task
			vTaskDelete(&controlReflowHandle);
			// *ToDo* Fix crashing at Task delete
			break;
		}
		setTemperature(currentPointPtr->temperature);
		setUpdate();

		profileState = 1;

		uint16_t tick = 0;
		// Wait till set temperature is reached then start timer
		// *ToDo* thermal runaway protection
		while(!reachedTemperature()) {
			osDelay(1);
			if(++tick > 1000) {
				break; // Implement thermal runaway protection here
			}
		}
		profileState = 3;

		osDelay(currentPointPtr->time*1000);
	}
}
