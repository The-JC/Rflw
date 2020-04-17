/**
 * @file config.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Project configuration file
 * 
 * This file is part of Rflw (https://github.com/The-JC/Rflw).
 * Copyright (C) 2020 Julian Hellner <julian@hellner.cc>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define LCD_ADDRESS 0x78
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define MENU_MAX_CONTENTS 10
#define MENU_MAX_DEPTH 3

#define INPUT_QUEUE_SIZE   3

#define CONTROL_TOLERANCE 5
#define CONTROL_MAX_INTEGRAL 100

#define CONTROL_MAX_TEMP 220

#define CONTROL_Kp 1.8
#define CONTROL_Ki 0.25
#define CONTROL_Kd 25

#define REFLOW_MAX_POINTS 10

// 0 normally low 1 normally high
#define HEATER_POLARITY 1

#endif /* CONFIG_H_ */
