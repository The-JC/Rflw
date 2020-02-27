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

#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f1xx_hal.h"

extern uint32_t Kp, Ki, Kd;

void updatePIDValues();
void setTemperature(uint16_t w);
uint16_t getSetTemperature();
uint8_t reachedTemperature();
void controlTask(void const * argument);

#endif /* CONTROL_H_ */
