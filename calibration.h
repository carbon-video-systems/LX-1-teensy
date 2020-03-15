/*
 * Calibration Header
 *
 * @file    calibration.h
 * @author  Carbon Video Systems 2019
 * @description   O=Drive calibration script.
 * This file reads the current state of the ODrive system
 * and calibrates the motors if required.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

/* Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include <HardwareSerial.h>

#include "ODriveLib.h"
#include "stormbreaker.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
// ODrive Trajectory Control Limits
#define TRAJ_VEL_LIMIT      40960.0f //needs to be a factor of 256 to work nicely with pan_tilt_speed calculation
#define TRAJ_ACCEL_LIMIT    25000.0f
#define TRAJ_DECEL_LIMIT    20000.0f

// ODrive Velocity Control Limits
#define VEL_VEL_LIMIT       81900.0f //needs to be a factor of 126 to work nicely with velocity calculations

// Homing
#define HALL_SENSOR 35
#define HALL_SENSOR_OFFSET 2

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);
void reconfigure_startup(ODriveClass&, int);
void encoder_calibrate(ODriveClass&, int);
void motor_calibrate(ODriveClass&, int);
void parameter_configuration(ODriveClass&, int);

void lx1_startup_sequence(ODriveClass&, StormBreaker&);
void startup_index_search(ODriveClass&, StormBreaker&, int);
void startup_index(ODriveClass&, StormBreaker&, int);

float system_reindex(float, int);
void homing_system(ODriveClass&, float, int, bool);

#endif //CALIBRATION_H
