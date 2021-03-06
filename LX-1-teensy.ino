/*
 * LX-1-teensy Source
 *
 * @file    LX-1-teensy.ino
 * @author  Carbon Video Systems 2019
 * @description   The main file used to receive, transmit, and service
 * the StormBreaker protocol.  In addition, this file is the staging ground
 * for calibrating and communicating with an ODrive System.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes ---------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>

#include "options.h"
#include "calibration.h"
#include "debug.h"
#include "fan.h"
#include "ODriveLib.h"
#include "stormbreaker.h"
#include "led.h"

/*Errors-------------------------------------------------------------------------------------------*/
#if (!(((defined BODY) == (defined HEAD)) == (defined BOTH_FOR_TESTING))) || (defined BODY && defined HEAD)
    #error Check your system settings in options.h!  Only one of BODY, HEAD, or BOTH_FOR_TESTING may be defined!
#endif
#if defined BOTH_FOR_TESTING
    #warning BOTH_FOR_TESTING is defined! Both motors will spin.  Do not flash this in production.
#endif
#if defined TESTING
    #warning TESTING is defined! Do not flash this in production.
#endif

#if (defined HEAD && !(defined LED_RING))
    #warning LED_RING is not defined!  The LED Ring will not initialize.
#endif
#if !defined FANS
    #warning FANS is not defined!  Cooling fans will not spin.
#endif

/* Constants --------------------------------------------------------------------------------------*/

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Variables --------------------------------------------------------------------------------------*/
ODriveClass odrive(odrive_serial);
#ifdef TESTING
    Debug debugger(odrive);
#endif
StormBreaker thor(odrive);

#ifdef FANS
    elapsedMillis temperatureCheckTiming;
#endif

#if defined LED_RING
    elapsedMillis rainbowTiming;
#endif

/* Functions --------------------------------------------------------------------------------------*/
/**
 * @brief   The Arduino setup function
 * @param   None
 * @return  None
 */
void setup()
{
    // power-on status LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(HALL_SENSOR, INPUT);

    #if defined HEAD && defined LED_RING
        // generates LED rainbow values & initializes LED Ring
        generateRainbow();
        led_begin();
        rainbow();
    #endif

    // ODrive uses 115200 baud
    odrive_serial.begin(ODRIVE_SERIAL_BAUD);
    while(!odrive_serial);

    odrive_serial.flush();

    // Pi uses 115200 baud, 8 data bits, 1 stop bit, no parity
    pi_serial.begin(PI_SERIAL_BAUD, SERIAL_8N1);
    while(!pi_serial);

    #ifdef TESTING
        // USB uses 115200 baud
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB); // wait for Arduino Serial Monitor to open

        SerialUSB.println("Hi computer, how are you today? :D");
        pi_serial.println("Hi Raspberry Pi how are you today? :D");
    #endif

    #if defined HEAD && defined LED_RING
        rainbow(RAINBOW_DELAY);
    #endif

    odrive_startup_sequence(odrive);

    #if defined HEAD && defined LED_RING
        rainbowTiming = 0;
        if (rainbowTiming < 100)
            rainbow(RAINBOW_DELAY);
    #else
        delay(100);
    #endif

    lx1_startup_sequence(odrive, thor);

    #ifdef FANS
        initFans();
        temperatureCheckTiming = 0;
    #endif

    #if defined HEAD && defined LED_RING
        #if defined TESTING
            while(SerialUSB.available() < 2 && pi_serial.available() < 2){
                rainbow(RAINBOW_DELAY);
            }
            setAllColour(GREEN);
        #else
            while(pi_serial.available() < 2){
                rainbow(RAINBOW_DELAY);
            }
            setAllColour(GREEN);
        #endif
    #endif
}

/**
 * @brief   The Arduino loop function
 * @param   None
 * @return  None
 */
void loop()
{
    if(pi_serial.available() >= 2)
        thor.serviceStormBreaker();

    #ifdef TESTING
        if(SerialUSB.available())
            debugger.serviceDebug();
    #endif

    #ifdef FANS
        if (temperatureCheckTiming >= temperatureTimingThreshold){
            runFans();
            temperatureCheckTiming = 0;
        }
    #endif
}
