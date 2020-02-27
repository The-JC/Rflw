/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file display.h is part of Reflow2.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Feb 23, 2020
 *
 ******************************************************************************/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "main.h"
#include "stm32f1xx_hal.h"

typedef struct {
	uint8_t mode;
} DISPLAY_t;

static DISPLAY_t display;

void LCDTask(void const * argument);

#endif /* DISPLAY_H_ */
