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

#include "OvenMode.h"

EventBits_t getMode() {
	return xEventGroupGetBits(modeEventGroup);
}

EventBits_t setMode(EventBits_t mode) {
	xEventGroupClearBits(modeEventGroup, EVENT_ALL);
	return xEventGroupSetBits(modeEventGroup, mode);
}

EventBits_t setUpdate() {
	return setMode(getMode() | EVENT_UPADTE);
}

EventBits_t clearUpdate() {
	if(getMode() & EVENT_UPADTE)
		setMode(getMode() & (EVENT_ALL ^ EVENT_UPADTE));
}
