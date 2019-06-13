/* Calibration Header */
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include <HardwareSerial.h>

#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
// ODrive Trajectory Limits
#define TRAJ_VEL_LIMIT      99840.0f 
#define TRAJ_ACCEL_LIMIT    25000.0f
#define TRAJ_DECEL_LIMIT    25000.0f

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);
void reconfigure_startup(ODriveClass&, int);
void encoder_calibrate(ODriveClass&, int);
void motor_calibrate(ODriveClass&, int);
void parameter_configuration(ODriveClass&, int);

#endif //CALIBRATION_H
