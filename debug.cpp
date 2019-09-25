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
#include "calibration.h"
#include "debug.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
void Debug::serviceDebug()
{
    char command = SerialUSB.read();

    switch(command){
    // Configuration Commands
    case 's':
        SerialUSB.println("Saving configuration");
        odrive_.SaveConfiguration();
        delay(100);
        break;
    case 'e':
        SerialUSB.println("Erasing configuration");
        odrive_.EraseConfiguration();
        delay(100);
        break;
    case 'r':
        SerialUSB.println("Rebooting");
        odrive_.Reboot();
        delay(1000);
        odrive_startup_sequence(odrive_);
        break;
    // State read Commands
    case 'c':
    {
        SerialUSB.println("Current state");
        #if defined BODY || defined BOTH_FOR_TESTING
            int32_t body = odrive_.readState(AXIS_BODY);
            SerialUSB.print("BODY: ");
            SerialUSB.println(body);
        #endif  // BODY || BOTH_FOR_TESTING
        #if defined HEAD || defined BOTH_FOR_TESTING
            int32_t head = odrive_.readState(AXIS_HEAD);
            SerialUSB.print("HEAD: ");
            SerialUSB.println(head);
        #endif  // HEAD || BOTH_FOR_TESTING
        break;
    }
    case 'f':
        #if defined BODY || defined BOTH_FOR_TESTING
            odrive_.ReadFeedback(AXIS_BODY);
            SerialUSB.println("BODY Feedback");
            SerialUSB.print("pos: ");
            SerialUSB.print(odrive_.Feedback.position);
            SerialUSB.print("  vel: ");
            SerialUSB.println(odrive_.Feedback.velocity);
        #endif
        #if defined HEAD || defined BOTH_FOR_TESTING
            odrive_.ReadFeedback(AXIS_HEAD);
            SerialUSB.println("HEAD Feedback");
            SerialUSB.print("pos: ");
            SerialUSB.print(odrive_.Feedback.position);
            SerialUSB.print("  vel: ");
            SerialUSB.println(odrive_.Feedback.velocity);
        #endif
        break;
    case 'v':
    {
        SerialUSB.print("BUS VOLTAGE: ");
        float voltage = odrive_.BusVoltage();
        SerialUSB.println(voltage);
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
