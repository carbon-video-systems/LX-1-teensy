/* Options Header */
#ifndef OPTIONS_H
#define OPTIONS_H

/* Includes --------------------------------------------------------*/
#include <Arduino.h>
#include <HardwareSerial.h>

#include "ODriveLib.h"

/* Constants --------------------------------------------------------------------------------------*/
/* Define TESTING during prototyping */
#define TESTING

#ifdef TESTING
    /* Testing specific stuff */
#endif

/* Define either BODY or HEAD for normal operation or BOTH_FOR_TESTING for testing purposes*/
// #define BODY
// #define HEAD
#define BOTH_FOR_TESTING

#if defined BODY
    /* Body specific stuff */
    #define NUM_MOTORS      1
    #define AXIS_BODY       0
    #define IDENTIFIER      0xAF
#elif defined HEAD
    /* Head specific stuff */
    #define NUM_MOTORS      1
    #define AXIS_HEAD       1
    #define IDENTIFIER      0x50
#elif defined BOTH_FOR_TESTING
    /* Stuff that makes testing two motors on a single ODrive work properly */
    #define NUM_MOTORS      2
    #define AXIS_BODY       0
    #define AXIS_HEAD       1
    #define IDENTIFIER      0xB7
#endif

#define SerialUSB       Serial
#define odrive_serial   Serial1
#define pi_serial       Serial2

#define USB_SERIAL_BAUD     9600
#define ODRIVE_SERIAL_BAUD  115200
#define PI_SERIAL_BAUD      115200

#endif //OPTIONS_H
