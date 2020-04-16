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

#include <OvenMode.h>
#include "Display/display.h"

#include "Display/SSD1306.h"
#include "Display/fonts.h"
#include "Display/MenuHelper.h"
#include "stdio.h"
#include "math.h"
#include "cmsis_os.h"
#include "sensor.h"
#include "control.h"

extern osMutexId xLCDMutexHandle;

void displayBake();
void displayReflow();

void checkDisplayEvent(uint32_t event);
void toTempratureBuffer(char *buf, uint8_t len, uint16_t temp);

void LCDInit() {
	SSD1306_Init();
	SSD1306_Fill(WHITE);
	SSD1306_UpdateScreen();
	osDelay(1000);
	SSD1306_Fill(BLACK);
	SSD1306_UpdateScreen();

//	display.mode = DISPLAY_MENU;
}

void LCDTask(void const * argument) {
	EventBits_t event;

	LCDInit();

	while(1) {
		osDelay(1);

		event = xEventGroupWaitBits(modeEventGroup, EVENT_MENU | EVENT_BAKE | EVENT_REFLOW | EVENT_UPADTE, pdFALSE, pdFALSE, portMAX_DELAY);
		checkDisplayEvent(event);
	}
}

void displayBake() {
	uint32_t event;
	char buffer[10];

	xSemaphoreTake(xLCDMutexHandle, portMAX_DELAY);

	while(1) {
		osDelay(100);
		SSD1306_Fill(BLACK);

		sprintf(buffer, "%d°C", getSetTemperature());
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

		event = getMode();
		if(event & ~(EVENT_BAKE | EVENT_UPADTE)) {
			break; // Cancel the while loop to check the display event for new instructions
		} else if(!(event & EVENT_UPADTE)) {
			xEventGroupWaitBits(modeEventGroup, EVENT_UPADTE, pdFALSE, pdFALSE, portMAX_DELAY);
			clearUpdate();
		}
	}

	xSemaphoreGive(xLCDMutexHandle);
}

void displayReflow() {
	uint32_t event;
	char buffer[10];

	xSemaphoreTake(xLCDMutexHandle, portMAX_DELAY);

	const TickType_t xDelay = 100 / portTICK_PERIOD_MS; // 100ms
	while(1) {
		vTaskDelay(xDelay);
		SSD1306_Fill(BLACK);

		//sprintf(buffer, );

		toTempratureBuffer(buffer, 10, getTemperature1());
		SSD1306_GotoY(43);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		toTempratureBuffer(buffer, 10, getTemperature2());
		SSD1306_GotoY(53);
		SSD1306_PutSAlign(buffer, &Font_7x10, WHITE, HORIZONTAL_CENTER);

		memset(buffer, 0, 10);

		SSD1306_UpdateScreen();
	}
}

void checkDisplayEvent(EventBits_t event) {
	switch (event) {
		case EVENT_BAKE | EVENT_UPADTE:
			clearUpdate();
			displayBake();
			break;
		case EVENT_REFLOW | EVENT_UPADTE:
			clearUpdate();
			displayReflow();
			break;
		case EVENT_MENU | EVENT_UPADTE:
		default:
			clearUpdate();
			menuDraw();
			break;
	}
}

void toTempratureBuffer(char *buf, uint8_t len, volatile uint16_t temp) {

	int tmpInt1 = temp/4;
	int tmpInt2 = (temp-tmpInt1*4)*25;

	sprintf(buf, "%d.%02d°C", tmpInt1, tmpInt2);
}
