/**
 *  This file is part of Rflw (https://lab.katurisoft.com/Julian/Rflw).
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

#include "FreeRTOS.h"
#include "task.h"
#include "memory.h"
#include "config.h"
#include "OvenMode.h"

extern uint8_t reachedTemperature();

const DATAPOINT_t basicPoints[2] =  {{60, 80}, {30, 190}};
const DATAPOINT_t advancedPoints[3] =  {{210, 140},  {60, 160}, {90, 200}};

void vReflowControlTask(void * argument) {
	for(uint32_t i=0; i<REFLOW_MAX_POINTS; i++) {
		currentPointPtr = currentCurvePtr + i;

		if(currentPointPtr->temperature==0 || i >= currentCurveLength) {
			currentPointPtr = NULL;
			profileState = NON;
			setTemperature(0);
			setDisplayMode(DISPLAY_MENU | DISPLAY_UPADTE); // Set back to menu mode
			vTaskSuspend(xControlInputTask);	// Suspend Input Handle Task
			vTaskDelete(xReflowControlTask);
			// *ToDo* Fix crashing at Task delete
			break;
		}
		setTemperature(currentPointPtr->temperature);
		setDisplayUpdate();

		profileState = HEATING;

		uint16_t tick = 0;
		TickType_t xDelay = 100 / portTICK_PERIOD_MS;
		// Wait till set temperature is reached then start timer
		// *ToDo* thermal runaway protection
		while(!reachedTemperature()) {
			vTaskDelay(xDelay);
			if(++tick > 1000*60*3) {
				break; // Implement thermal runaway protection here
			}
		}
		profileState = RUNNING;

		xDelay = currentPointPtr->time*1000 / portTICK_PERIOD_MS;
		vTaskDelay(xDelay);
	}
}
