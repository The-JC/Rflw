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

#include "Reflow.h"

#include "cmsis_os.h"
#include "control.h"
#include "OvenMode.h"

int main(void) {
	HAL_Init();
	InitSystem();
	modeEventGroup = xEventGroupCreate();
	InitOS(); // Start scheduler
}

//void inputTask(void const * argument) {
//	while(1) {
//		osDelay(1);
//	}
//}

//void menuTask(void const * argument) {
//	while(1) {
//		osDelay(1);
//	}
//}
