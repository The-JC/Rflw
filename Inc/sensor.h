/**
 * @file sensor.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Reads the sensors
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

#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f1xx_hal.h"
#include "main.h"

enum {
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};

extern SPI_HandleTypeDef hspi2;

/**
 * @brief Iniate a sensor reading
 * 
 * @return void
 */
void readTemperature(void);

/**
 * @brief Returns sensor value
 * @note @note return value is times 4
 * 
 * @return uint16_t temprature
 */
///@{
uint16_t getTemperature1(void);
uint16_t getTemperature2(void);
///@}

#endif /* SENSOR_H_ */
