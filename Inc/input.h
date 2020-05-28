/**
 * @file input.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Handles the button inputs
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

#ifndef INPUT_H_
#define INPUT_H_

#include "stdint.h"

/**
 * @brief Press event
 * 
 */
enum PRESS_EVENT_t{
	PRESS_NONE=0,
	PRESS_UP,
	PRESS_DOWN,
	PRESS_SELECT
};

void inputTask(void const * argument);
/**
 * @brief gets event from queue
 * 
 * @return uint8_t 
 * @see PRESS_EVENT_t
 */
uint8_t inputGetEvent();

#endif /* INPUT_H_ */
