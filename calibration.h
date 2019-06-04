/* Calibration Header */
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Includes ------------------------------------------------------------*/
#include "Arduino.h"
#include <HardwareSerial.h>

#include "options.h"
#include "odrivelib.h"

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);

#endif //CALIBRATION_H
