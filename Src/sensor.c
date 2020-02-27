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

#include "sensor.h"

#include "cmsis_os.h"

extern osMutexId SPIMutexHandle;

uint8_t currentSensor;
float temperature1;
float temperature2;
volatile uint16_t rxBuffer[1];

uint8_t isDataValid(volatile uint16_t *buffer);
float convertToTemprature(volatile uint16_t *buffer);
void __handleSPI_RxCallback(SPI_HandleTypeDef *hspi);

float getTemperature1(void) {
	return temperature1;
}

float getTemperature2(void) {
	return temperature2;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
	__handleSPI_RxCallback(hspi);
	HAL_SPI_IRQHandler(hspi);
}

void __handleSPI_RxCallback(SPI_HandleTypeDef *hspi) {
	if(currentSensor < 1 || currentSensor > 2) {
		Error_Handler();
		return;
	}

	if(currentSensor == 1) {
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

		if(!isDataValid(rxBuffer)) {
			temperature1 = -1;
			return;
		}

		temperature1 = convertToTemprature(rxBuffer);
		*rxBuffer = 0x0;
		return;
	} else if(currentSensor == 2) {
		currentSensor = 0;

		if(!isDataValid(rxBuffer)) {
			temperature2 = -1;
			return;
		}

		temperature2 = convertToTemprature(rxBuffer);
		*rxBuffer = 0x0;
		return;
	}
}

void readTemperature() {
	// Check if a sensor reading is in progress
	if(currentSensor == 0) {
		currentSensor = 1; // Set to read first sensor

		xSemaphoreTake(SPIMutexHandle, portMAX_DELAY);

		// Set chip select low to start the data transmission from MAX6675 Sensor
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

		if(HAL_SPI_Receive_IT(hspi2, (uint8_t*)rxBuffer, 2) != HAL_OK) {
			Error_Handler();
		}

		osDelay(1);
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

		xSemaphoreGive(SPIMutexHandle);
		return;
	} else if (currentSensor == 1) {
		currentSensor = 2; // Set to read second sensor

		xSemaphoreTake(SPIMutexHandle, portMAX_DELAY);

		// Set chip select low to start the data transmission from MAX6675 Sensor
		HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_RESET);

		if(HAL_SPI_Receive_IT(hspi2, (uint8_t*)rxBuffer, 2) != HAL_OK) {
			Error_Handler();
		}

		osDelay(1);
		HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_SET);

		xSemaphoreGive(SPIMutexHandle);
		return;
	}
}

uint8_t isDataValid(volatile uint16_t *buffer) {
	// Check if third last bit is not zero
	return ((*rxBuffer) >> 2) & 0b0000000000000001;
}

float convertToTemprature(volatile uint16_t *buffer) {
	/*Read data out of first sensor
	 * First bit always 0 last 2 too
	 * Third last bit is 1 when no sensor is connected
	 * Shift 3 right and mask first 4 bits to read value
	 */
	return ((float)(((*rxBuffer) >> 3) & 0b0000111111111111))/4;
}
