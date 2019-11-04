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
#include "RZ120.h"
#include "options.h"

/* Functions------------------------------------------------------------*/
class Debug {
public:
    Debug(RZ120Class& rz120) : rz120_(rz120) {}

    void serviceDebug();

private:
    RZ120Class& rz120_;
};

#endif //DEBUG_H
