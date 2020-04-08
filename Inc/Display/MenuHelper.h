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

#ifndef DISPLAY_MENUHELPER_H_
#define DISPLAY_MENUHELPER_H_

#include "Display/SSD1306.h"
#include "config.h"
#include "ProfileControl.h"

enum {
	MENU_LAST=0,
	MENU_SUB,
	MENU_EXEC,
	MENU_OPTION,
	MENU_VAL,
};

enum {
	MENU_TYPE_MENU=0,
	MENU_TYPE_OPTION
};

enum {
	MENU_MODE_SCROLL=0,
	MENU_MODE_VAL
};

struct MENUITEM_t {
	char* text;
	uint8_t type;
	void (*callback)(void);
	union {
		const struct MENU_t *subMenu;
		const struct OPTION_t *option;
		uint32_t *val;
		uint32_t padding[REFLOW_MAX_POINTS];
	};
};

struct MENU_t {
	uint8_t type;
	char* name;
	const uint32_t num;
	const struct MENUITEM_t contents[MENU_MAX_CONTENTS];
};

struct OPTIONITEM_t {
	char* text;
	uint8_t length;
	void (*callback)(uint32_t, uint32_t);
	union {
		const uint32_t id;
		const DATAPOINT_t *points[REFLOW_MAX_POINTS];
	};
};

struct OPTION_t {
	uint8_t type;
	char* name;
	const uint32_t length;
	const struct OPTIONITEM_t options[MENU_MAX_CONTENTS];
};

void menuTask(void const * argument);

void menuDraw();
void menuAction();

#endif /* DISPLAY_MENUHELPER_H_ */
