/*******************************************************************************
 * Copyright (C) 2019 Julian Hellner - All Rights Reserved
 * 
 * The file input.h is part of Reflow2.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Julian Hellner <hellnerjulian@gmail.com>, Feb 23, 2020
 *
 ******************************************************************************/

#ifndef INPUT_H_
#define INPUT_H_

#include "stdint.h"

enum {
	PRESS_NONE=0,
	PRESS_UP,
	PRESS_DOWN,
	PRESS_SELECT
};

void inputTask(void const * argument);
uint8_t inputGetEvent();

#endif /* INPUT_H_ */
