/*
 * Debugger Source
 *
 * @file    debug.cpp
 * @author  Carbon Video Systems 2019
 * @description   Debugging the RZ120 over serial connection.
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
        SerialUSB.println("Power ON");
        rz120_.PowerOn();
        delay(100);
        break;
    case 'q':
        SerialUSB.println("Power Query");
        rz120_.PowerQuery();
        delay(100);
        break;
    case 'P':
        SerialUSB.println("Power Off");
        rz120_.PowerOff();
        delay(100);
        break;

    case 'i':
        SerialUSB.println("Zoom in slow");
        rz120_.ZoomInSlow();
        // delay(100);
        break;
    case 'I':
        SerialUSB.println("Zoom in normal");
        rz120_.ZoomInNormal();
        // delay(100);
        break;
    case 'o':
        SerialUSB.println("Zoom out slow");
        rz120_.ZoomOutSlow();
        // delay(100);
        break;
    case 'O':
        SerialUSB.println("Zoom out normal");
        rz120_.ZoomOutNormal();
        // delay(100);
        break;
    case 'z':
        SerialUSB.println("Zoom in fast");
        rz120_.ZoomInFast();
        // delay(100);
        break;
    case 'Z':
        SerialUSB.println("Zoom out fast");
        rz120_.ZoomOutFast();
        // delay(100);
        break;

    case 'n':
        SerialUSB.println("Focus in slow");
        rz120_.FocusInSlow();
        // delay(100);
        break;
    case 'N':
        SerialUSB.println("Focus in normal");
        rz120_.FocusInNormal();
        // delay(100);
        break;
    case 'm':
        SerialUSB.println("Focus out slow");
        rz120_.FocusOutSlow();
        // delay(100);
        break;
    case 'M':
        SerialUSB.println("Focus out normal");
        rz120_.FocusOutNormal();
        // delay(100);
        break;
    case 'f':
        SerialUSB.println("Focus in fast");
        rz120_.FocusInFast();
        // delay(100);
        break;
    case 'F':
        SerialUSB.println("Focus out fast");
        rz120_.FocusOutFast();
        // delay(100);
        break;

    case 's':
        SerialUSB.println("Shutter Off");
        rz120_.ShutterOff();
        delay(100);
        break;
    case 'S':
        SerialUSB.println("Shutter On");
        rz120_.ShutterOn();
        delay(100);
        break;

    case 'a':
        SerialUSB.println("Auto Iris Off");
        rz120_.AutoIrisOff();
        // delay(100);
        break;
    case 'A':
        SerialUSB.println("Auto Iris 255");
        rz120_.AutoIris(255);
        // delay(100);
        break;
    case '\n':
        break;
    case '\r':
        break;
    default:
        SerialUSB.println("Invalid command please try again");
        break;
    }
}
