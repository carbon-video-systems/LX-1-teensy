/*
 * RZ120 Header
 *
 * @file    RZ120.h
 * @author  Carbon Video Systems 2019
 * @description   Panasonic RZ120 RS232 Serial Command Driver.
 *      Communicates over an RS232 serial line and implements
 *      control commands for the projector.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef RZ120_H
#define RZ120_H

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>
#include "options.h"

/* Functions------------------------------------------------------------*/
class RZ120Class {
public:
    RZ120Class(Stream& serial);

    void initialize(void);

    // Commands
    void PowerQuery(void);
    void PowerOn(void);
    void PowerOff(void);

    void ShutterOff(void);
    void ShutterOn(void);
    void ShutterToggle(void);
    void ShutterQuery(void);

    void ZoomInSlow(void);
    void ZoomInNormal(void);
    void ZoomInFast(void);

    void ZoomOutSlow(void);
    void ZoomOutNormal(void);
    void ZoomOutFast(void);

    void FocusInSlow(void);
    void FocusInNormal(void);
    void FocusInFast(void);

    void FocusOutSlow(void);
    void FocusOutNormal(void);
    void FocusOutFast(void);

    void AutoIris(uint8_t);
    void AutoIrisOff(void);
    void AutoIrisMax(void);
    void AutoIrisQuery(void);

private:
    String readString();
    Stream& serial_;
};

#endif //RZ120_H
