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

#ifndef OVENMODE_H_
#define OVENMODE_H_

#include "cmsis_os.h"

EventGroupHandle_t modeEventGroup;

enum DisplayMode {
	EVENT_NONE	=0,
	EVENT_MENU	=1 << 0,
	EVENT_BAKE	=1 << 1,
	EVENT_REFLOW=1 << 2,
	EVENT_UPADTE=1 << 3,
	EVENT_ALL	=0xF
};

EventBits_t getMode();
EventBits_t setMode(EventBits_t mode);
EventBits_t setUpdate();
EventBits_t clearUpdate();

#endif /* OVENMODE_H_ */
