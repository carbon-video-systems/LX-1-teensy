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

/* Define either BODY or HEAD but not both */
#define BODY
// #define HEAD
#ifdef BODY
    /* Body specific stuff */
    #define IDENTIFIER 0xAF
#elif defined HEAD
    /* Head specific stuff */
    #define IDENTIFIER 0x50
#endif

/* Define ONE_MOTOR or TWO_MOTORS depending on the number of motors connected to a single ODrive */
#define ONE_MOTOR
// #define TWO_MOTORS   //this is primarily for testing purposes
#ifdef ONE_MOTOR
    #define NUM_MOTORS      1
    #define AXIS_BODY       1
    #define AXIS_HEAD       1
#elif defined TWO_MOTORS
    #define NUM_MOTORS      2
    #define AXIS_BODY       1
    #define AXIS_HEAD       0
#endif

#define SerialUSB       Serial
#define odrive_serial   Serial1
#define pi_serial       Serial2

#define USB_SERIAL_BAUD     9600
#define ODRIVE_SERIAL_BAUD  115200
#define PI_SERIAL_BAUD      115200

#endif //OPTIONS_H
