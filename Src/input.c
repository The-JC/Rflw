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

#include "input.h"

#include "main.h"
#include "cmsis_os.h"
#include "config.h"

xQueueHandle xInputQueue;

uint8_t getInput();

uint8_t inputGetEvent() {
	uint8_t ret = PRESS_NONE;

	xQueueReceive(xInputQueue, &ret, 0);

	return ret;
}

void inputTask(void const *argument) {
	xInputQueue = xQueueCreate(INPUT_QUEUE_SIZE, sizeof(uint8_t));

	const portTickType xDelay = 100 / portTICK_RATE_MS;
	uint8_t eventCurrent;
	uint8_t eventLast;

	while(1) {
		vTaskDelay(xDelay);

		eventCurrent = getInput();
		if(eventCurrent != PRESS_NONE) {
			if(eventCurrent !=  eventLast) {
				xQueueSend(xInputQueue, &eventCurrent, portMAX_DELAY);
			}
			eventLast = eventCurrent;
		} else {
			eventLast = PRESS_NONE;
		}
	}
}

uint8_t getInput() {
	// Inverted because we're using pull-up resistors
	if(!HAL_GPIO_ReadPin(SELECT_GPIO_Port, SELECT_Pin)) {
		return PRESS_SELECT;
	}
	if(!HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin)) {
		return PRESS_DOWN;
	}
	if(!HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin)) {
		return PRESS_UP;
	}

	return PRESS_NONE;
}
