/**
 * @file OvenMode.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Handles the diffrent states of the oven
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

#ifndef OVENMODE_H_
#define OVENMODE_H_

#include "cmsis_os.h"

EventGroupHandle_t modeEventGroup;

/**
 * Modus of display
 */
enum DisplayMode {
	EVENT_NONE	=0,			///< Nothing is displayed
	EVENT_MENU	=1 << 0,	///< The menu is displayed
	EVENT_BAKE	=1 << 1,	///< The Bake overlay is shown
	EVENT_REFLOW=1 << 2,	///< The Reflow overlay is shown
	EVENT_UPADTE=1 << 3,	///< Signalise the mode was updated
	EVENT_ALL	=0xF		///< all events
};

/**
 * Get the current mode without waiting
 * 
 * @return EventBits_t 
 */
EventBits_t getMode();
/**
 * Override all modes and display events
 * @note clears the update flag
 * 
 * @param mode 
 * @return EventBits_t 
 */
EventBits_t setMode(EventBits_t mode);
/**
 * Sets the update flag
 * 
 * @return EventBits_t 
 */
EventBits_t setUpdate();
/**
 * Clears the update flag without clearing any others
 * 
 * @return EventBits_t 
 */
EventBits_t clearUpdate();

#endif /* OVENMODE_H_ */
