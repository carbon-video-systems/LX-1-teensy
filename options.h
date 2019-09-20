/*
 * Options Header
 *
 * @file    options.h
 * @author  Carbon Video Systems 2019
 * @description   The main header where preprocessor directives and
 * system settings and variables are housed.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

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
#define BODY
// #define HEAD
// #define BOTH_FOR_TESTING

#if defined BODY
    /* Body specific stuff */
    #define NUM_MOTORS      1
    #define AXIS_BODY       1
    #define IDENTIFIER      0xAF
#elif defined HEAD
    /* Head specific stuff */
    #define NUM_MOTORS      1
    #define AXIS_HEAD       0
    #define IDENTIFIER      0x50
#elif defined BOTH_FOR_TESTING
    /* Stuff that makes testing two motors on a single ODrive work properly */
    #define NUM_MOTORS      2
    #define AXIS_BODY       1
    #define AXIS_HEAD       0
    #define IDENTIFIER      0xB7
#endif

#define SerialUSB       Serial
#define odrive_serial   Serial1
#define pi_serial       Serial2

// LS7366R SPI constants
#define SPI_counter         SPI
#define COUNTER_SELECT_PIN  15
#define counterBytes        4

#define USB_SERIAL_BAUD     115200
#define ODRIVE_SERIAL_BAUD  115200
#define PI_SERIAL_BAUD      115200

#endif //OPTIONS_H
