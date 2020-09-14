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


#include "Display/MenuHelper.h"

#include "stdio.h"
#include "input.h"
#include "control.h"
#include "Display/display.h"
#include "config.h"
#include "OvenMode.h"

void menuValChanger(uint32_t *ptr);

uint16_t calculateMenuLength(const struct MENU_t *menu, uint8_t menuDepth);

static void Menu_RunBake();
static void Menu_RunReflow(uint32_t curve, uint32_t length);
static void Menu_StopReflow();

uint32_t *valChangerOld;
uint32_t valChangerNew;

#define ISREFLOWSTATE getOvenState() & STATE_REFLOW
#define ISBAKESTATE getOvenState() & STATE_BAKE

static const struct OPTION_t reflowCurve = {
		.type = MENU_TYPE_OPTION,
		.name = "Reflow",
		.length = 2,
		{
			{
				.text = "Basic",
				.length = 2,
				.callback = Menu_RunReflow,
				.points = basicPoints
			},
			{
				.text = "Advanced",
				.length = 3,
				.callback = Menu_RunReflow,
				.points = advancedPoints
			}
		}
};

static const struct MENU_t programmMenu = {
		.type = MENU_TYPE_MENU,
		.name = "Programs",
		.num = 2,
		{
			{
				.text = "Bake",
				.type = MENU_EXEC,
				.callback = Menu_RunBake,
			},
			{
				.text = "Reflow",
				.type = MENU_OPTION,
				.callback = 0,
				{
					.option = &reflowCurve
				}
			}
		}
};

static const struct MENU_t settingsMenu = {
		.type = MENU_TYPE_MENU,
		.name = "Settings",
		.num = 3,
		{
			{
				.text = "P",
				.type = MENU_VAL,
				.callback = updatePIDValues,
				{
					.val = &Kp
				},
			},
			{
				.text = "I",
				.type = MENU_VAL,
				.callback = updatePIDValues,
				{
					.val = &Ki
				},
			},
			{
				.text = "D",
				.type = MENU_VAL,
				.callback = updatePIDValues,
				{
					.val = &Kd
				},
			}
		}
};

static const struct MENU_t mainMenu = {
		.type = MENU_TYPE_MENU,
		.name = "Main Menu",
		.num = 2,
		{
			{
				.text = "Programs",
				.type = MENU_SUB,
				.callback = 0,
				{
					.subMenu = &programmMenu,
				},
			},
			{
				.text = "Settings",
				.type = MENU_SUB,
				.callback = 0,
				{
					.subMenu = &settingsMenu,
				},
			}
		}
};

const struct MENU_t *menuStack[MENU_MAX_DEPTH];
uint8_t menuPosStack[MENU_MAX_DEPTH];
uint8_t menuDepth;

void vMenuTask(void * argument) {
	uint16_t newPos;
	const portTickType xDelay = 100 / portTICK_RATE_MS;
//	const struct MENU_t *menu;

	menuDepth = 0; // How deep we are in sub menus
	menuPosStack[menuDepth] = 0; // On which line we are with the cursor
	menuStack[menuDepth] = &mainMenu; // Which menu is on the depths

//	menuDraw();
	setDisplayMode(DISPLAY_MENU | DISPLAY_UPADTE);

	while(1) {
		vTaskDelay(xDelay);
		xEventGroupWaitBits(modeEventGroup, DISPLAY_MENU, pdFALSE, pdFALSE, portMAX_DELAY);

		uint8_t event = inputGetEvent();
		switch (event) {
			case PRESS_UP:
				if(menuPosStack[menuDepth] > 0)
					menuPosStack[menuDepth]--; // decrease index of menu contents when higher then 1
				break;
			case PRESS_DOWN:
				newPos = menuPosStack[menuDepth] + 1; // increase index of menu contents
				if(newPos < calculateMenuLength(menuStack[menuDepth], menuDepth)) // Check if new index is within limits of menu
					menuPosStack[menuDepth] = newPos;
				break;
			case PRESS_SELECT:
				menuAction();
				break;
			default:
				break;
		}
		setDisplayUpdate();
//		menuDraw();
	}
}

void menuAction() {
	const struct MENU_t *menu;
	uint8_t pos;

	pos = menuPosStack[menuDepth];
	menu = menuStack[menuDepth];

	if(menuDepth > 0) { // Check if we're in sub menu
		if(pos == 0) {
			--menuDepth; // Go up one menu
			return;
		} else {
			--pos; // Pos-1 to comensate the Back button menu offset
		}
	}
	// Check for the stop button in the reflow menu
	if(menu == (const struct MENU_t*)&reflowCurve && ISREFLOWSTATE) {
		if(pos == menu->num) {
			--menuPosStack[menuDepth]; // decrease pos to compensate that the button will disappear 
			Menu_StopReflow();
			return;
		}
	}

	if(menu->type == MENU_TYPE_OPTION) {
		const struct OPTION_t *option = (struct OPTION_t*) menu;
		const struct OPTIONITEM_t *item = &option->options[pos];
		item->callback((uint32_t) *item->points, (uint32_t) item->length);
		return;
	}

	switch(menu->contents[pos].type) {
	case MENU_OPTION:
	case MENU_SUB:
		if(!(menuDepth + 1 < MENU_MAX_DEPTH)) {
			//*ToDo* implement error
			break;
		}

		++menuDepth;
		menuPosStack[menuDepth] = 0;
		if(menu->contents[pos].type == MENU_SUB)
			menuStack[menuDepth] = menu->contents[pos].subMenu;
		else
			menuStack[menuDepth] = (const struct MENU_t*) menu->contents[pos].option;
		break;
	case MENU_EXEC:
		menu->contents[pos].callback(); // Execute callback function
		break;
	case MENU_VAL:
		menuValChanger(menu->contents[pos].val);
		if(menu->contents[pos].callback != 0)
			menu->contents[pos].callback();
		break;
	}
}

void menuValChanger(uint32_t *ptr) {
	uint8_t done = 0;

	valChangerOld = ptr;
	valChangerNew = *ptr;

	setDisplayMode(DISPLAY_VALCHANGE | DISPLAY_UPADTE);

	const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
	while(!done) {
		vTaskDelay(xDelay);
		uint8_t event = inputGetEvent();
		switch(event) {
			case PRESS_UP:
				++valChangerNew;
				setDisplayUpdate();
				break;
			case PRESS_DOWN:
				--valChangerNew;
				setDisplayUpdate();
				break;
			case PRESS_SELECT:
				*ptr = valChangerNew;
				setDisplayMode(DISPLAY_MENU | DISPLAY_UPADTE);
				done = 1;
				break;
			default:
				break;
		}
	}
}

void valChangerDraw() {
	xSemaphoreTake(xLCDMutex, portMAX_DELAY);

	char oldbuf[14];
	char newbuf[14];

	memset(oldbuf, 0, 14);
	siprintf(oldbuf, "%u",(int) *valChangerOld);

	siprintf(newbuf, "%u", (int) valChangerNew);

	SSD1306_Fill(BLACK);
	SSD1306_GotoY(16);
	SSD1306_PutSAlign(oldbuf, &Font_7x10, WHITE, HORIZONTAL_CENTER);
	SSD1306_GotoY(29);
	SSD1306_PutSAlign(newbuf, &Font_11x18, WHITE, HORIZONTAL_CENTER);
	SSD1306_DrawTriangle(96, 22, 110, 22, 103, 15, WHITE);
	SSD1306_DrawTriangle(96, 49, 110, 49, 103, 56, WHITE);
	SSD1306_UpdateScreen();

	xSemaphoreGive(xLCDMutex);
}

void menuDraw() {
	xSemaphoreTake(xLCDMutex, portMAX_DELAY);
	uint8_t menuPos;
	uint8_t drawPos;
	const struct MENU_t *menu;

	uint8_t i=0;
	uint8_t offset=0;
	uint8_t submenu=0;
	menuPos = menuPosStack[menuDepth];
	menu = menuStack[menuDepth];

	SSD1306_Fill(BLACK);

	// When cursor position is lower then 2 draw from top
	if(menuPos < 3) {
		drawPos=0;
	} else {
		drawPos = menuPos-3;
	}

	// Put the menu name in the first line.
	SSD1306_DrawFilledRectangle(0, 0, 127, 10, WHITE);
	SSD1306_GotoY(1);
	SSD1306_PutSAlign(menu->name, &Font_7x10, BLACK, HORIZONTAL_CENTER);

	// Check if this is a submenu
	if(menu != &mainMenu) {
		// When in a submenu put the back button in the second line.
		if(drawPos==0) {
			SSD1306_GotoXY(5, 12+1);
			if(menuPos == 0) {
				SSD1306_DrawFilledRectangle(0, 12, 127, 10, WHITE);
				SSD1306_PutS("Back", &Font_7x10, BLACK);
			} else {
				SSD1306_PutS("Back", &Font_7x10, WHITE);
			}
			offset = 1;
		}
		// Activate submenu flag, this will offset the menu array read by -1
		submenu = 1;
	}
	
	// Start with one of we are on the top of a sub menu to account for the extra Back button
	for(i = offset; i < 4; i++) {
		if(drawPos + i - submenu >= menu->num) break; // break loop when max number of menu elements is reached
		SSD1306_GotoXY(5, 12+12*i+1);

		if(drawPos+i == menuPos) {
			SSD1306_DrawFilledRectangle(0, 12+12*i, 127, 10, WHITE);
			SSD1306_PutS(menu->contents[drawPos+i-submenu].text, &Font_7x10, BLACK);
		} else {
			SSD1306_PutS(menu->contents[drawPos+i-submenu].text, &Font_7x10, WHITE);
		}
	}

	if(menu == ((const struct MENU_t*)&reflowCurve)){
		if(ISREFLOWSTATE) {
			SSD1306_GotoXY(10, 12+12*i+1);
			if(drawPos+i == menuPos) {
				SSD1306_DrawFilledRectangle(0, 12+12*i, 127, 10, WHITE);
				SSD1306_PutS("Stop", &Font_7x10, BLACK);
			} else {
				SSD1306_PutS("Stop", &Font_7x10, WHITE);
			}
		}
	}

	SSD1306_GotoY(47);
//	SSD1306_PutSAlign("OK", &Font_7x10, WHITE, HORIZONTAL_CENTER);
//	if(menu != &mainMenu) {
//		SSD1306_GotoX(5);
//		SSD1306_PutS("<-", &Font_7x10, WHITE);
//	}
	SSD1306_UpdateScreen();

	xSemaphoreGive(xLCDMutex);
}

uint16_t calculateMenuLength(const struct MENU_t *menu, uint8_t menuDepth) {
	uint8_t offset=0;
	if(menu == (const struct MENU_t*)&reflowCurve && ISREFLOWSTATE) {
		++offset;
	}
	return menu->num + (menuDepth == 0 ? 0:1) + offset;
}

void Menu_RunBake() {
	setDisplayMode(DISPLAY_BAKE | DISPLAY_UPADTE);
	setOvenState(STATE_BAKE);
}

void Menu_RunReflow(uint32_t curve, uint32_t length) {
	currentCurvePtr = (DATAPOINT_t*) curve;
	currentCurveLength = length;
	setDisplayMode(DISPLAY_REFLOW | DISPLAY_UPADTE);
	setOvenState(STATE_REFLOW);
}

void Menu_StopReflow() {
	setDisplayMode(DISPLAY_MENU | DISPLAY_UPADTE);
	setOvenState(STATE_NONE);
	setTemperature(0);
	vTaskSuspend(xControlInputTask);
	vTaskDelete(xReflowControlTask);
	currentCurvePtr = NULL;
	currentCurveLength = 0;
}
