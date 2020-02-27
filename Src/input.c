/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file input.c is part of Reflow2.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Feb 23, 2020
 *
 ******************************************************************************/

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
