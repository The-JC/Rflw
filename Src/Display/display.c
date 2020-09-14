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

#include <OvenMode.h>
#include "Display/display.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "memory.h"
#include "Display/SSD1306.h"
#include "Display/fonts.h"
#include "Display/MenuHelper.h"
#include "stdio.h"
#include "math.h"
#include "sensor.h"
#include "control.h"

void displayBake();
void displayReflow();

void checkDisplayEvent(uint32_t event);
void toTempratureBuffer(char *buf, uint8_t len, uint16_t temp);

void LCDInit() {
	xSemaphoreTake(xLCDMutex, portMAX_DELAY);
	SSD1306_Init();
	SSD1306_Fill(WHITE);
	SSD1306_UpdateScreen();
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	SSD1306_Fill(BLACK);
	SSD1306_UpdateScreen();
	xSemaphoreGive(xLCDMutex);

//	display.mode = DISPLAY_MENU;
}

void vLCDTask(void * argument) {
	EventBits_t event;

	LCDInit();

	const portTickType xDelay = 1 / portTICK_RATE_MS;
	while(1) {
		vTaskDelay(xDelay);

		event = xEventGroupWaitBits(modeEventGroup, DISPLAY_MENU | DISPLAY_BAKE | DISPLAY_REFLOW | DISPLAY_VALCHANGE | DISPLAY_UPADTE, pdFALSE, pdFALSE, portMAX_DELAY);
		checkDisplayEvent(event);
	}
}

void displayBake() {
	uint32_t event;
	char buffer[10];

	xSemaphoreTake(xLCDMutex, portMAX_DELAY);

	const portTickType xDelay = 100 / portTICK_RATE_MS;
	while(1) {
		vTaskDelay(xDelay);
		SSD1306_Fill(BLACK);

		sprintf(buffer, "%lu°C", getSetTemperature());
		SSD1306_GotoY(33);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		toTempratureBuffer(buffer, 10, getTemperature1());
		SSD1306_GotoY(43);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		toTempratureBuffer(buffer, 10, getTemperature2());
		SSD1306_GotoY(53);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		SSD1306_UpdateScreen();

		event = getDisplayMode();
		if(event & ~(DISPLAY_BAKE | DISPLAY_UPADTE)) {
			break; // Cancel the while loop to check the display event for new instructions
		} else if(!(event & DISPLAY_UPADTE)) {
			xEventGroupWaitBits(modeEventGroup, DISPLAY_UPADTE, pdFALSE, pdFALSE, portMAX_DELAY);
			clearDisplayUpdate();
		}
	}

	xSemaphoreGive(xLCDMutex);
}

void displayReflow() {
	uint32_t event;
	char buffer[10];

	xSemaphoreTake(xLCDMutex, portMAX_DELAY);


	const TickType_t xDelay = 100 / portTICK_PERIOD_MS; // 100ms
	TickType_t startTick = xTaskGetTickCount();
	while(1) {
		vTaskDelay(xDelay);
		SSD1306_Fill(BLACK);

		if(currentPointPtr != NULL) {
			if(profileState == 1) {
				sprintf(buffer, "Heating");
				SSD1306_GotoY(0);
				SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);
				memset(buffer, 0, 10);

				sprintf(buffer, "-> %lu°C", currentPointPtr->temperature);
				SSD1306_GotoY(33);
				SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);
				memset(buffer, 0, 10);
				startTick = xTaskGetTickCount();
			} else if(profileState == 3) {
				sprintf(buffer, "%lus -> %lus", (xTaskGetTickCount() - startTick)/1000, currentPointPtr->time);
				SSD1306_GotoY(0);
				SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);
				memset(buffer, 0, 10);

				sprintf(buffer, "%lu°C", currentPointPtr->temperature);
				SSD1306_GotoY(33);
				SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);
				memset(buffer, 0, 10);
			}
		}

		toTempratureBuffer(buffer, 10, getTemperature1());
		SSD1306_GotoY(43);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		toTempratureBuffer(buffer, 10, getTemperature2());
		SSD1306_GotoY(53);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		SSD1306_UpdateScreen();

		event = getDisplayMode();
		if(event & ~(DISPLAY_REFLOW | DISPLAY_UPADTE)) {
			break; // Cancel the while loop to check the display event for new instructions
		} else if(!(event & DISPLAY_UPADTE)) {
			// xEventGroupWaitBits(modeEventGroup, EVENT_UPADTE, pdFALSE, pdFALSE, portMAX_DELAY);
			clearDisplayUpdate();
		}
	}

	xSemaphoreGive(xLCDMutex);
}

void checkDisplayEvent(EventBits_t event) {
	switch (event) {
		case DISPLAY_BAKE | DISPLAY_UPADTE:
			clearDisplayUpdate();
			displayBake();
			break;
		case DISPLAY_REFLOW | DISPLAY_UPADTE:
			clearDisplayUpdate();
			displayReflow();
			break;
		case DISPLAY_VALCHANGE | DISPLAY_UPADTE:
			clearDisplayUpdate();
			valChangerDraw();
			break;
		case DISPLAY_MENU | DISPLAY_UPADTE:
		default:
			clearDisplayUpdate();
			menuDraw();
			break;
	}
}

void toTempratureBuffer(char *buf, uint8_t len, volatile uint16_t temp) {

	int tmpInt1 = temp/4;
	int tmpInt2 = (temp-tmpInt1*4)*25;

	sprintf(buf, "%d.%02d°C", tmpInt1, tmpInt2);
}
