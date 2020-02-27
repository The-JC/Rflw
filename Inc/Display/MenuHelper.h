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

enum {
	MENU_LAST=0,
	MENU_SUB,
	MENU_EXEC,
	MENU_VAL,
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
		uint32_t *val;
	};
};

struct MENU_t {
	char* name;
	const uint32_t num;
	const struct MENUITEM_t contents[MENU_MAX_CONTENTS];
};

void menuTask(void const * argument);

void menuDraw();
void menuAction();



#endif /* DISPLAY_MENUHELPER_H_ */
