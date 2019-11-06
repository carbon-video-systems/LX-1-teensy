/*
 * RZ120 Source
 *
 * @file    RZ120.cpp
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

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>
#include "RZ120.h"

/* Constants------------------------------------------------------------*/
#define ZOOM_FOCUS_MAX  10

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Functions------------------------------------------------------------*/
RZ120Class::RZ120Class(Stream& serial)
    : serial_(serial) {}

void RZ120Class::initialize(void)
{
    PowerOn();
    delay(500);
    PowerQuery();
    // RECEIVE AND look for QPW response
    delay(500);
    ShutterOn();
    delay(100);

    for (int i = 0; i <= ZOOM_FOCUS_MAX; i++){
        ZoomOutFast();
        delay(50);
        FocusOutFast();
        delay(50);
    }
}

/*
*   Serial Command Structure:
*       STX character (0x02 in hex)
*       Identifier:  AD (ASCII)
*       Address:    ZZ (ASCII) to broadcast to all connected projectors
*       ; (ASCII)
*       Command:    List of commands can be found at
*       https://business.panasonic.com.au/visual-system/system/files/files/fields/field_file/psd/2019/09/23/rz120_control_commands_1569200088.5256.pdf
*       ETX character (0x03 in hex)
*/

// RZ120 Power Commands
void RZ120Class::PowerQuery(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;QPW";
    serial_.write(0x03);
}

void RZ120Class::PowerOn(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;PON";
    serial_.write(0x03);
}

void RZ120Class::PowerOff(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;POF";
    serial_.write(0x03);
}

// RZ120 Shutter Commands
void RZ120Class::ShutterOff(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;OSH:0";
    serial_.write(0x03);
}

void RZ120Class::ShutterOn(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;OSH:1";
    serial_.write(0x03);
}

void RZ120Class::ShutterToggle(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;OSH";
    serial_.write(0x03);
}

void RZ120Class::ShutterQuery(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;QSH";
    serial_.write(0x03);
}

// RZ120 Zoom Commands
void RZ120Class::ZoomInSlow(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00000";
    serial_.write(0x03);
}

void RZ120Class::ZoomInNormal(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00100";
    serial_.write(0x03);
}

void RZ120Class::ZoomInFast(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00200";
    serial_.write(0x03);
}

void RZ120Class::ZoomOutSlow(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00001";
    serial_.write(0x03);
}

void RZ120Class::ZoomOutNormal(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00101";
    serial_.write(0x03);
}

void RZ120Class::ZoomOutFast(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI5=+00201";
    serial_.write(0x03);
}

// RZ120 Focus Commands
void RZ120Class::FocusInSlow(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00000";
    serial_.write(0x03);
}

void RZ120Class::FocusInNormal(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00100";
    serial_.write(0x03);
}

void RZ120Class::FocusInFast(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00200";
    serial_.write(0x03);
}

void RZ120Class::FocusOutSlow(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00001";
    serial_.write(0x03);
}

void RZ120Class::FocusOutNormal(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00101";
    serial_.write(0x03);
}

void RZ120Class::FocusOutFast(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;VXX:LNSI4=+00201";
    serial_.write(0x03);
}

// RZ120 Auto Iris Commands
void RZ120Class::AutoIris(uint8_t iris_setting)
{
    serial_.write(0x02);
    serial_ << "ADZZ:OAI:A" << iris_setting;
    serial_.write(0x03);
}

void RZ120Class::AutoIrisOff(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;OAI:A000";
    serial_.write(0x03);
}

void RZ120Class::AutoIrisMax(void)
{
    serial_.write(0x02);
    serial_ << "ADZZ;OAI:A255";
    serial_.write(0x03);
}

void RZ120Class::AutoIrisQuery(void){
    serial_.write(0x02);
    serial_ << "ADZZ;QAI:A";
    serial_.write(0x03);
}

String RZ120Class::readString() {
    String str = "";
    static const unsigned long timeout = 1000;
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}
