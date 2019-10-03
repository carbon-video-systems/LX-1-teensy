/*
 * Fan Header
 *
 * @file    fan.h
 * @author  Carbon Video Systems 2019
 * @description   Cooling Fan Driver.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef FAN_H
#define FAN_H

/* Includes-------------------------------------------------------------*/
#include <i2c_t3.h>
#include "TMP102.h"
#include "options.h"

/* Functions------------------------------------------------------------*/
void initFans(void);
void runFan1(void);
void runFan2(void);

#endif //FAN_H
