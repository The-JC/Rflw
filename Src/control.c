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

#include "control.h"

#include "cmsis_os.h"
#include "config.h"
#include "OvenMode.h"
#include "input.h"
#include "sensor.h"
#include "ProfileControl.h"


uint32_t Kp = CONTROL_Kd*100;
uint32_t Ki = CONTROL_Ki*100;
uint32_t Kd = CONTROL_Kd*100;

volatile uint32_t w=0;

uint8_t control(uint16_t x);
uint32_t calculate_dt();

extern void profileControlTask(void * argument);
void controlInputTask();

void controlBake();
void controlReflow();

osThreadId controlInputHandle;
osThreadId controlReflowHandle;

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

uint16_t getSetTemperature() {
	return w;
}

void setTemperature(uint16_t w) {
	if(w<0) w = 0;
	if(w>CONTROL_MAX_TEMP) w = CONTROL_MAX_TEMP;

	pid.w = w;
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
	return getTemperature() < pid.w+CONTROL_TOLERANCE && getTemperature() > pid.w+CONTROL_TOLERANCE;
}

uint8_t control(uint16_t x) {
	int output;
	int e = pid.w-x;
	pid.dt = calculate_dt();

	// Calculate the integral part of the PID controller
	pid.integral += e*pid.dt/1000;

	// Limit integral portion
	if(pid.integral > CONTROL_MAX_INTEGRAL) pid.integral = CONTROL_MAX_INTEGRAL;
	if(pid.integral < -CONTROL_MAX_INTEGRAL) pid.integral = -CONTROL_MAX_INTEGRAL;


	// Calculate the derivative part of the PID controller
	pid.derivative = (e - pid.previousError)*1000 / pid.dt;

	pid.previousError = e;

	// Calculate proportional & add all parts together with their respective gain
	output = pid.Kp*e + pid.Ki*pid.integral + pid.Kd*pid.derivative;

	if(output>100) {
		output=100;
	} else if(output<0) {
		output=0;
	}
	return output;
}

void updatePIDValues() {
	pid.Kp = ((float) Kp)/100;
	pid.Ki = ((float) Ki)/100;
	pid.Kd = ((float) Kd)/100;
}

uint32_t calculate_dt() {
	uint32_t dt = HAL_GetTick()-pid.lastControlTime;
	pid.lastControlTime = HAL_GetTick();
	return dt;
}

void controlTask(void const * argument) {
	updatePIDValues();
	xTaskCreate(controlInputTask, "CONTROL_IN", 128, NULL, osPriorityLow, &controlInputHandle);
	vTaskSuspend(controlInputHandle);

	while(1) {
		osDelay(1);
		EventBits_t event = xEventGroupWaitBits(modeEventGroup, EVENT_BAKE | EVENT_REFLOW, pdFALSE, pdFALSE, portMAX_DELAY);

		switch (event & (EVENT_BAKE | EVENT_REFLOW)) {
			case EVENT_BAKE:
				controlBake();
				break;
			case EVENT_REFLOW:
				controlReflow();
				break;
			default:
				// BOTH bits must be high
				Error_Handler();
				break;
		}

	}
}

void controlInputTask() {
	const portTickType xDelay = 100 / portTICK_RATE_MS;

	while(1) {
		vTaskDelay(xDelay);

		uint8_t event = inputGetEvent();
		switch (event) {
			case PRESS_UP:
				if(getMode() & EVENT_REFLOW)
					break;
				if(w<CONTROL_MAX_TEMP) w+=10;
				setUpdate();
				break;
			case PRESS_DOWN:
				if(getMode() & EVENT_REFLOW)
					break;
				if(w>0) w-=10;
				setUpdate();
				break;
			case PRESS_SELECT:
				setMode(EVENT_MENU | EVENT_UPADTE); // Set back to menu mode
				vTaskSuspend(controlInputHandle);
				break;
			default:
				break;
		}

	}
//	vTaskDelete(controlInputHandle); // Delete itself
}

void controlBake() {
	vTaskResume(controlInputHandle);

	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1000ms
	while(1) {
		if(pid.w != w) pid.w = w;

		HAL_GPIO_TogglePin(LD_Power_GPIO_Port, LD_Power_Pin);
		readTemperature();
		uint8_t p = control(getTemperature()/4);
		setUpdate();

		const TickType_t xDelayOffset = (xDelay * p)/100;

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, !HEATER_POLARITY);

		vTaskDelay(xDelayOffset);

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);

		vTaskDelay(xDelay - xDelayOffset);

		EventBits_t event = xEventGroupGetBits(modeEventGroup);
		if(event & EVENT_BAKE) {
			clearUpdate();
			vTaskResume(controlInputHandle);
		}
		if(event & EVENT_REFLOW) {
			break;
		}
	}
}

void controlReflow() {
	vTaskResume(controlInputHandle);

	if(xTaskCreate(profileControlTask, "CONTROL_REFLOW", 128, NULL, osPriorityLow, &controlReflowHandle) != pdPASS) {
		Error_Handler();
	}

	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; // 1000ms
	while(1) {
		if(pid.w != w) pid.w = w;

		HAL_GPIO_TogglePin(LD_Power_GPIO_Port, LD_Power_Pin);
		readTemperature();
		uint8_t p = control(getTemperature()/4);
		setUpdate();

		const TickType_t xDelayOffset = (xDelay * p)/100;

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, !HEATER_POLARITY);

		vTaskDelay(xDelayOffset);

		HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, HEATER_POLARITY);

		vTaskDelay(xDelay - xDelayOffset);

		EventBits_t event = xEventGroupGetBits(modeEventGroup);
		if(event & EVENT_REFLOW) {
			clearUpdate();
			vTaskResume(controlInputHandle);
		}
		if(event & EVENT_BAKE) {
			break;
		}
	}
}
