/**
 * @file memory.h
 * @author Julian Hellner <julian@hellner.cc>
 * @brief Allocates memory for Tasks and Mutexes in FreeRTOS
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

#ifndef TASKS_H_
#define TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

/** 
 * vSystemMonitorTask
 * @brief The vSystemMonitor Task monitors the system health and feeds the Window Watchdog timer
 * 
 */
#define SYSTEM_MONITOR_NAME "SystemMonitor"
#define SYSTEM_MONITOR_PRIORITY tskIDLE_PRIORITY+6
#define SYSTEM_MONITOR_STACK_SIZE 128
static StaticTask_t xSystemMonitorBuffer __attribute__((unused));
static StackType_t xSystemMonitorStack [SYSTEM_MONITOR_STACK_SIZE] __attribute__((unused));
TaskHandle_t xSystemMonitorTask;

/** 
 * vInputHandlerTask
 * @brief The input Handler Task handles the keyboard inputs
 * 
 */
#define INPUT_HANDLER_NAME "InputHandler"
#define INPUT_HANDLER_PRIORITY tskIDLE_PRIORITY+1
#define INPUT_HANDLER_STACK_SIZE 128
static StaticTask_t xInputHandlerBuffer __attribute__((unused));
static StackType_t xInputHandlerStack [INPUT_HANDLER_STACK_SIZE] __attribute__((unused));
TaskHandle_t xInputHandlerTask;

/**
 * vLCDTask
 * @brief The LCD task manages the display and allows for asynchronous communication
 * In future it also displays animations
 * 
 */
#define LCD_NAME "LCD"
#define LCD_PRIORITY tskIDLE_PRIORITY+0
#define LCD_STACK_SIZE 128
static StaticTask_t xLCDBuffer __attribute__((unused));
static StackType_t xLCDStack [LCD_STACK_SIZE] __attribute__((unused));
TaskHandle_t xLCDTask;

/**
 * vMenuTask
 * @brief performs menu related operations
 * 
 */
#define MENU_NAME "Menu"
#define MENU_PRIORITY tskIDLE_PRIORITY+1
#define MENU_STACK_SIZE 128
static StaticTask_t xMenuBuffer __attribute__((unused));
static StackType_t xMenuStack [MENU_STACK_SIZE] __attribute__((unused));
TaskHandle_t xMenuTask;

/**
 * vControlTask
 * @brief regulates the heater and runs the PID loop
 * 
 */
#define CONTROL_NAME "Control"
#define CONTROL_PRIORITY tskIDLE_PRIORITY+4
#define CONTROL_STACK_SIZE 128
static StaticTask_t xControlBuffer __attribute__((unused));
static StackType_t xControlStack [CONTROL_STACK_SIZE] __attribute__((unused));
TaskHandle_t xControlTask;

/**
 * vControInputlTask
 * @brief handles the button events when in a mode, normally handled by the @vMenuTask
 * 
 */
#define CONTROL_INPUT_NAME "ControlInput"
#define CONTROL_INPUT_PRIORITY tskIDLE_PRIORITY+1
#define CONTROL_INPUT_STACK_SIZE 64
static StaticTask_t xControlInputBuffer __attribute__((unused));
static StackType_t xControlInputStack [CONTROL_INPUT_STACK_SIZE] __attribute__((unused));
TaskHandle_t xControlInputTask;

/**
 * vReflowControlTask
 * @brief Handles the Reflow mode, it monitors the time and sets the temperature according.
 * 
 */
#define REFLOW_CONTROL_NAME "ReflowControl"
#define REFLOW_CONTROL_PRIORITY tskIDLE_PRIORITY+1
#define REFLOW_CONTROL_STACK_SIZE 128
static StaticTask_t xReflowControlBuffer __attribute__((unused));
static StackType_t xReflowControlStack [REFLOW_CONTROL_STACK_SIZE] __attribute__((unused));
TaskHandle_t xReflowControlTask;

// xI2CMutex
static StaticSemaphore_t xI2CMutexBuffer __attribute__((unused));
SemaphoreHandle_t xI2CMutex;

// xSPIMutex
static StaticSemaphore_t xSPIMutexBuffer __attribute__((unused));
SemaphoreHandle_t xSPIMutex;

// xLCDMutex
static StaticSemaphore_t xLCDMutexBuffer __attribute__((unused));
SemaphoreHandle_t xLCDMutex;

EventGroupHandle_t modeEventGroup;
EventGroupHandle_t stateEventGroup;

#endif /* TASKS_H_ */