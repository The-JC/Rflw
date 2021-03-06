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

#include "control.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "stdlib.h"
#include "config.h"
#include "memory.h"
#include "OvenMode.h"
#include "input.h"
#include "sensor.h"
#include "ProfileControl.h"

uint32_t Kp = CONTROL_Kp*100;
uint32_t Ki = CONTROL_Ki*10;
uint32_t Kd = CONTROL_Kd*100;

volatile uint32_t w=0;

uint8_t control(uint16_t x);
uint32_t calculate_dt();

extern void vReflowControlTask(void * argument);
void vControlInputTask(void * argument);
void createControlInputTask();

void controlBake();
void controlReflow();

typedef struct {

} CONTROL_t;

typedef struct {
	uint32_t w;
	float Kp;
	float Ki;
	float Kd;

	int previousError;
	int integral;
	float derivative;
	float dt;
	uint32_t lastControlTime;

} PID_t;

static PID_t pid;

uint32_t getSetTemperature() {
	return w;
}

void setTemperature(uint32_t set_w) {
	if(set_w<0) set_w = 0;
	if(set_w>CONTROL_MAX_TEMP) set_w = CONTROL_MAX_TEMP;

	pid.w = set_w;

}

volatile uint16_t getTemperature() {
	if(getTemperature1() != 0 && getTemperature2() != 0) {
		return (getTemperature1() + getTemperature2()) /2;
	} else if(getTemperature1() != 0) {
		return getTemperature1();
	} else if(getTemperature2() != 0) {
		return getTemperature2();
	} else {
		return -1;
	}
}

uint8_t reachedTemperature() {
	return getTemperature()/4 > pid.w-CONTROL_TOLERANCE && getTemperature()/4 < pid.w+CONTROL_TOLERANCE;
}

uint8_t control(uint16_t x) {
	int output;
	int e = pid.w-x;
	pid.dt = calculate_dt();

	// Calculate the integral part of the PID controller
	pid.integral += e * (pid.dt/1000);

	// Limit integral portion
	if(pid.integral > CONTROL_MAX_INTEGRAL) pid.integral = CONTROL_MAX_INTEGRAL;
	if(pid.integral < -CONTROL_MAX_INTEGRAL) pid.integral = -CONTROL_MAX_INTEGRAL;


	// Calculate the derivative part of the PID controller
	pid.derivative = (e - pid.previousError) * (1000 / pid.dt);

	pid.previousError = e;

	// Calculate proportional & add all parts together with their respective gain
	output = pid.Kp*e + pid.Ki*pid.integral + pid.Kd*pid.derivative;

	if(output>100) {
		output=100;
	} else if(output<0) {
		output=0;
	}
	char buf[20];

	itoa(xTaskGetTickCount(), buf, 16);
	ITM_SendString(buf);
	ITM_SendChar(' ');

	itoa(getTemperature(), buf, 16);
	ITM_SendString(buf);
	ITM_SendChar(' ');

	itoa(pid.w, buf, 16);
	ITM_SendString(buf);
	ITM_SendString("\n");

	return output;
}

/**
 * @brief update internal PID values
 * 
 */
void updatePIDValues() {
	pid.Kp = ((float) Kp)/100;
	pid.Ki = ((float) Ki)/100;
	pid.Kd = ((float) Kd)/100;
}

/**
 * @brief calculate time delta from last execution
 * 
 * @return uint32_t dt
 */
uint32_t calculate_dt() {
	uint32_t dt = HAL_GetTick()-pid.lastControlTime;
	pid.lastControlTime = HAL_GetTick();
	return dt;
}

void vControlTask(void * argument) {
	updatePIDValues();
	createControlInputTask();
	vTaskSuspend(xControlInputTask);

	const TickType_t xDelay = 1 / portTICK_PERIOD_MS; // 1ms
	while(1) {
		vTaskDelay(xDelay);
		EventBits_t event = xEventGroupWaitBits(modeEventGroup, DISPLAY_BAKE | DISPLAY_REFLOW, pdFALSE, pdFALSE, portMAX_DELAY);

		switch (event & (DISPLAY_BAKE | DISPLAY_REFLOW)) {
			case DISPLAY_BAKE:
				controlBake();
				break;
			case DISPLAY_REFLOW:
				controlReflow();
				break;
			default:
				// BOTH bits must be high
				Error_Handler();
				break;
		}

	}
}

void vControlInputTask(void * argument) {
	const portTickType xDelay = 100 / portTICK_RATE_MS;

	while(1) {
		vTaskDelay(xDelay);

		uint8_t event = inputGetEvent();
		switch (event) {
			case PRESS_UP:
				if(getDisplayMode() & DISPLAY_REFLOW)
					break;
				if(w<CONTROL_MAX_TEMP) w+=10;
				setDisplayUpdate();
				break;
			case PRESS_DOWN:
				if(getDisplayMode() & DISPLAY_REFLOW)
					break;
				if(w>0) w-=10;
				setDisplayUpdate();
				break;
			case PRESS_SELECT:
				setDisplayMode(DISPLAY_MENU | DISPLAY_UPADTE); // Set back to menu mode
				vTaskSuspend(xControlInputTask);
				break;
			default:
				break;
		}

	}
//	vTaskDelete(xControlInputTask); // Delete itself
}

void controlBake() {
	vTaskResume(xControlInputTask);

	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1000ms
	while(1) {
		if(pid.w != w) pid.w = w;

		HAL_GPIO_TogglePin(LD_Power_GPIO_Port, LD_Power_Pin);
		readTemperature();
		uint8_t p = control(getTemperature()/4);
		setDisplayUpdate();

		const TickType_t xDelayOffset = (xDelay * p)/100;

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, !HEATER_POLARITY);

		vTaskDelay(xDelayOffset);

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);

		vTaskDelay(xDelay - xDelayOffset);

		EventBits_t event = xEventGroupGetBits(modeEventGroup);
		if(event & DISPLAY_BAKE) {
			clearDisplayUpdate();
			vTaskResume(xControlInputTask);
		}
		if(event & DISPLAY_REFLOW) {
			HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);
			HAL_GPIO_WritePin(LD_Power_GPIO_Port, LD_Power_Pin, GPIO_PIN_SET);
			break;
		}

		if(getOvenState() != 0b01) {
			HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);
			HAL_GPIO_WritePin(LD_Power_GPIO_Port, LD_Power_Pin, GPIO_PIN_SET);
			break;
		}

	}
}

void controlReflow() {
	vTaskResume(xControlInputTask);

	xReflowControlTask = xTaskCreateStatic(vReflowControlTask, REFLOW_CONTROL_NAME, REFLOW_CONTROL_STACK_SIZE, NULL, REFLOW_CONTROL_PRIORITY, xReflowControlStack, &xReflowControlBuffer);

	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1000ms
	while(1) {
		HAL_GPIO_TogglePin(LD_Power_GPIO_Port, LD_Power_Pin);
		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);

		readTemperature();
		
		uint8_t p = control(getTemperature()/4);
		setDisplayUpdate();

		const TickType_t xDelayOffset = (xDelay * p)/100;

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, !HEATER_POLARITY);

		vTaskDelay(xDelayOffset);

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);

		vTaskDelay(xDelay - xDelayOffset);

		EventBits_t event = xEventGroupGetBits(modeEventGroup);
		if(event & DISPLAY_REFLOW) {
			clearDisplayUpdate();
			vTaskResume(xControlInputTask);
		}
		if(event & DISPLAY_BAKE) {
			HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);
			HAL_GPIO_WritePin(LD_Power_GPIO_Port, LD_Power_Pin, GPIO_PIN_SET);
			break;
		}

		if(getOvenState() != 0b10) {
			HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);
			HAL_GPIO_WritePin(LD_Power_GPIO_Port, LD_Power_Pin, GPIO_PIN_SET);
			break;
		}
	}
}

void createControlInputTask() {
	xControlInputTask = xTaskCreateStatic(vControlInputTask, CONTROL_INPUT_NAME, CONTROL_INPUT_STACK_SIZE, NULL, CONTROL_INPUT_PRIORITY, xControlInputStack, &xControlInputBuffer);
}

void ITM_SendString(char *ptr) {
	while(*ptr) {
		ITM_SendChar(*ptr);
		ptr++;
	}
}

