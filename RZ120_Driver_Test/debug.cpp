/*
 * Debugger Source
 *
 * @file    debug.cpp
 * @author  Carbon Video Systems 2019
 * @description   Debugging the Teensy/ODrive system over serial connection.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include "debug.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
void Debug::serviceDebug()
{
    char command = SerialUSB.read();

    switch(command){
    // Configuration Commands
    case 'p':
        SerialUSB.println("Power On");
        rz120_.PowerOn();
        delay(100);
        break;
    case 'x':
        SerialUSB.println("Power Off");
        rz120_.PowerOff();
        delay(100);
        break;
    case 'z':
        SerialUSB.println("Zoom in");
        rz120_.ZoomIn();
        delay(100);
        break;
    // State read Commands
    case 'c':
    {
        // SerialUSB.println("Current state");
        // #if defined BODY || defined BOTH_FOR_TESTING
        //     int32_t body = odrive_.readState(AXIS_BODY);
        //     SerialUSB.print("BODY: ");
        //     SerialUSB.println(body);
        // #endif  // BODY || BOTH_FOR_TESTING
        // #if defined HEAD || defined BOTH_FOR_TESTING
        //     int32_t head = odrive_.readState(AXIS_HEAD);
        //     SerialUSB.print("HEAD: ");
        //     SerialUSB.println(head);
        // #endif  // HEAD || BOTH_FOR_TESTING
        break;
    }
    case '\n':
        break;
    case '\r':
        break;
    default:
        SerialUSB.println("Invalid command please try again");
        break;
    }
}
