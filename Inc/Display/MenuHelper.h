/**
 * @file ProfileControl.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Controls the oven in Reflow mode
 * 
 * @copyright
 * This file is part of Rflw (https://github.com/The-JC/Rflw).\n
 * Copyright (C) 2020 Julian Hellner <julian@hellner.cc>\n
 * \n
 * This program is free software: you can redistribute it and/or modify\n
 * it under the terms of the GNU General Public License as published by\n
 * the Free Software Foundation, either version 3 of the License, or\n
 * any later version.\n
 * 
 * This program is distributed in the hope that it will be useful,\n
 * but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 * GNU General Public License for more details.\n
 * \n
 * You should have received a copy of the GNU General Public License\n
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.\n
 */

#ifndef DISPLAY_MENUHELPER_H_
#define DISPLAY_MENUHELPER_H_

#include "Display/SSD1306.h"
#include "config.h"
#include "ProfileControl.h"

typedef enum {
	MENU_LAST=0,
	MENU_SUB,
	MENU_EXEC,
	MENU_OPTION,
	MENU_VAL,
} MENU_ITEM_TYPE_t;

/**
 * @brief Menu type
 * 
 */
typedef enum {
	MENU_TYPE_MENU=0, ///< normal menu or submenu
	MENU_TYPE_OPTION ///< Option menu, lets you choose from a list
} MENU_TYPE_t;

/**
 * @brief menu item
 * 
 */
struct MENUITEM_t {
	char* text; ///< text of item
	uint8_t type; ///< item type @see MENU_ITEM_TYPE_t
	void (*callback)(void); ///< callback function
	union {
		const struct MENU_t *subMenu; ///< Sub menu @see MENU_t
		const struct OPTION_t *option; ///< Option sub menu @see OPTION_t
		uint32_t *val; ///< Value to change
		uint32_t padding[REFLOW_MAX_POINTS]; ///< Padding to have same size as Option Item. @see OPTIONITEM_t
	};
};

/**
 * @brief Menu holding menu items
 * 
 */
struct MENU_t {
	uint8_t type; ///< type of menu @see MENU_TYPE_t
	char* name; ///< name of menu
	const uint32_t num; ///< number of menu items @see MENUITEM_t
	const struct MENUITEM_t contents[MENU_MAX_CONTENTS];
};

/**
 * @brief Option Item, specificly for profiles
 * 
 */
struct OPTIONITEM_t {
	char* text; ///< text of option
	uint8_t length; ///< number of points
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
