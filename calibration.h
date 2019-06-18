/* Calibration Header */
#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include <HardwareSerial.h>

#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
// ODrive Trajectory Control Limits
#define TRAJ_VEL_LIMIT      99840.0f //needs to be a factor of 256 to work nicely with pan_tilt_speed calculations
#define TRAJ_ACCEL_LIMIT    25000.0f
#define TRAJ_DECEL_LIMIT    25000.0f

// ODrive Velocity Control Limits
#define VEL_VEL_LIMIT       99918.0f //needs to be a factor of 126 to work nicely with velocity calculations

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);
void reconfigure_startup(ODriveClass&, int);
void encoder_calibrate(ODriveClass&, int);
void motor_calibrate(ODriveClass&, int);
void parameter_configuration(ODriveClass&, int);

#endif //CALIBRATION_H
