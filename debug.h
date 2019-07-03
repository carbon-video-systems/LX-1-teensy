/*
 * Debugger Header
 *
 * @file    debug.h
 * @author  Carbon Video Systems 2019
 * @description   Debugging the Teensy/ODrive system over serial connection.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef DEBUG_H
#define DEBUG_H

/* Includes-------------------------------------------------------------*/
#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
class Debug {
public:
    Debug(ODriveClass& odrive) : odrive_(odrive) {}

    void serviceDebug();

private:
    ODriveClass& odrive_;
};

#endif //DEBUG_H
