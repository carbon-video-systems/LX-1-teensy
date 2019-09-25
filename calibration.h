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
#include "LS7366R.h"
#include "stormbreaker.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
// ODrive Trajectory Control Limits
#define TRAJ_VEL_LIMIT      128000.0f //needs to be a factor of 256 to work nicely with pan_tilt_speed calculation
#define TRAJ_ACCEL_LIMIT    202400.0f
#define TRAJ_DECEL_LIMIT    202400.0f

// ODrive Velocity Control Limits
#define VEL_VEL_LIMIT       103320.0f //needs to be a factor of 126 to work nicely with velocity calculations

/* Functions------------------------------------------------------------*/
void odrive_startup_sequence(ODriveClass& );
void odrive_startup_check(ODriveClass& , bool*);
void reconfigure_startup(ODriveClass&, int);
void encoder_calibrate(ODriveClass&, int);
void motor_calibrate(ODriveClass&, int);
void parameter_configuration(ODriveClass&, int);

void lx1_startup_sequence(ODriveClass&, LS7366R&, StormBreaker&);
void startup_index_search(ODriveClass&, int);
void system_direction(LS7366R&, StormBreaker&);
void startup_index(ODriveClass&, LS7366R&, StormBreaker&, int);

int32_t system_reindex(float, int32_t, int32_t, bool);
void homing_system(ODriveClass&, int32_t, int, bool);

#endif //CALIBRATION_H
