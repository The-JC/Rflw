/**
 * @file control.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Regulates the temperature of the oven
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

#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f1xx_hal.h"

///@{
/**
 * PID values
 */
extern uint32_t Kp, Ki, Kd;
///@}

/**
 * Update internal PID values of PID_t object
 */
void updatePIDValues();
/**
 * Set the targeted set Temperature
 * 
 * @param set_w
 * @return void
 */
void setTemperature(uint16_t set_w);
/**
 * Get the Set Temperature
 * 
 * @return uint16_t set temperature
 */
uint16_t getSetTemperature();
/**
 * Average temprature of both sensors in °C
 * @note return value is times 4
 * 
 * @return volatile getTemperature 
 */
volatile uint16_t getTemperature();
/**
 * Returns if temperature is within a tolerance
 * 
 * @return uint8_t temperature reached
 */
uint8_t reachedTemperature();
/**
 * Temperature control task
 * 
 * @param argument none
 * @return void
 */
void controlTask(void const * argument);

#endif /* CONTROL_H_ */
