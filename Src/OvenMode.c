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

EventBits_t getDisplayMode() {
	return xEventGroupGetBits(modeEventGroup);
}

EventBits_t setDisplayMode(EventBits_t mode) {
	xEventGroupClearBits(modeEventGroup, DISPLAY_ALL);
	return xEventGroupSetBits(modeEventGroup, mode);
}

EventBits_t setDisplayUpdate() {
	return setDisplayMode(getDisplayMode() | DISPLAY_UPADTE);
}

EventBits_t clearDisplayUpdate() {
	if(getDisplayMode() & DISPLAY_UPADTE)
		setDisplayMode(getDisplayMode() & (DISPLAY_ALL ^ DISPLAY_UPADTE));
	return xEventGroupGetBits(modeEventGroup);
}

EventBits_t getOvenState() {
	return xEventGroupGetBits(stateEventGroup);
}

EventBits_t setOvenState(EventBits_t mode) {
	xEventGroupClearBits(stateEventGroup, STATE_ALL);
	return xEventGroupSetBits(stateEventGroup, mode);
}