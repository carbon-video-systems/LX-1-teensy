/* Options Header */
#ifndef OPTIONS_H
#define OPTIONS_H

/* Includes --------------------------------------------------------*/
#include "Arduino.h"
#include "ODriveLib.h"

#include "HardwareSerial.h"

/* Constants --------------------------------------------------------------------------------------*/
// #define TESTING

#define SerialUSB       Serial
#define odrive_serial   Serial1
#define pi_serial       Serial2

#define NUM_MOTORS      2
#define PI_SERIAL_BAUD  921600

#endif //OPTIONS_H
