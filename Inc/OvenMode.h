/**
 * @file OvenMode.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Handles the diffrent states of the oven
 * 
 * @copyright
 * This file is part of Rflw (https://lab.katurisoft.com/Julian/Rflw).\n
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

#ifndef OVENMODE_H_
#define OVENMODE_H_

#include "FreeRTOS.h"
#include "event_groups.h"
#include "memory.h"

/**
 * Modus of display
 */
enum DisplayMode {
	DISPLAY_NONE		=0,			///< Nothing is displayed
	DISPLAY_MENU		=1 << 0,	///< The menu is displayed
	DISPLAY_BAKE		=1 << 1,	///< The Bake overlay is shown
	DISPLAY_REFLOW		=1 << 2,	///< The Reflow overlay is shown
	DISPLAY_VALCHANGE	=1 << 3,	///< Value change overlay
	DISPLAY_UPADTE		=1 << 4,	///< Signalise the mode was updated
	DISPLAY_ALL			= 0xFF		///< all events
};

enum OvenState {
	STATE_NONE			=0,			///< Oven is currently doing nothing/displaying Menu
	STATE_BAKE			=1 << 0,	///< Bake operation is running
	STATE_REFLOW		=1 << 1,	///< Reflow operation is running
	STATE_ALL			= 0xFF		///< all states
};

/**
 * Get the current display mode without waiting
 * 
 * @return EventBits_t 
 */
EventBits_t getDisplayMode();
/**
 * Override all dipslay modes and display events
 * @note clears the update flag
 * 
 * @param mode 
 * @return EventBits_t 
 */
EventBits_t setDisplayMode(EventBits_t mode);
/**
 * Sets the update flag
 * 
 * @return EventBits_t 
 */
EventBits_t setDisplayUpdate();
/**
 * Clears the update flag without clearing any others
 * 
 * @return EventBits_t 
 */
EventBits_t clearDisplayUpdate();

/**
 * Get the current oven state without waiting
 * 
 * @return EventBits_t 
 */
EventBits_t getOvenState();
/**
 * Override the oven state
 * @note clears the update flag
 * 
 * @param mode 
 * @return EventBits_t 
 */
EventBits_t setOvenState(EventBits_t mode);

#endif /* OVENMODE_H_ */
