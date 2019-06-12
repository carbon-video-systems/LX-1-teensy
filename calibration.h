/* Calibration Header */
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include <HardwareSerial.h>

#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);
void reconfigure_startup(ODriveClass&, int);
void encoder_calibrate(ODriveClass&, int);
void motor_calibrate(ODriveClass&, int);
void set_axis_limits(ODriveClass&, int);

#endif //CALIBRATION_H
