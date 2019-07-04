/*
 * ODrive Library Header
 *
 * @file    ODriveLib.h
 * @author  Oskar Weigl 2017 / Carbon Video Systems 2019
 * @description   ODrive ASCII Communication protocol.
 * This file was written by Oskar Weigl and extended in 2019 by Carbon Video
 * Systems.
 *
 * @section LICENSE
 * MIT License
 * Copyright (c) 2017 Oskar Weigl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * Copyright (c) 2019, Carbon Video Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef ODRIVELIB_H
#define ODRIVELIB_H

#include <Arduino.h>

#include "options.h"

class ODriveClass {
public:
    enum AxisState_t {
        AXIS_STATE_UNDEFINED = 0,           //<! will fall through to idle
        AXIS_STATE_IDLE = 1,                //<! disable PWM and do nothing
        AXIS_STATE_STARTUP_SEQUENCE = 2, //<! the actual sequence is defined by the config.startup_... flags
        AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,   //<! run all calibration procedures, then idle
        AXIS_STATE_MOTOR_CALIBRATION = 4,   //<! run motor calibration
        AXIS_STATE_SENSORLESS_CONTROL = 5,  //<! run sensorless control
        AXIS_STATE_ENCODER_INDEX_SEARCH = 6, //<! run encoder index search
        AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7, //<! run encoder offset calibration
        AXIS_STATE_CLOSED_LOOP_CONTROL = 8,  //<! run closed loop control
        AXIS_STATE_LOCKIN_SPIN = 9,
        AXIS_STATE_ENCODER_DIR_FIND = 10
    };

    enum MotorType_t {
        MOTOR_TYPE_HIGH_CURRENT = 0,
        MOTOR_TYPE_LOW_CURRENT = 1,
        MOTOR_TYPE_GIMBAL = 2
    };

    enum ControlMode_t {
        CTRL_MODE_VOLTAGE_CONTROL = 0,
        CTRL_MODE_CURRENT_CONTROL = 1,
        CTRL_MODE_VELOCITY_CONTROL = 2,
        CTRL_MODE_POSITION_CONTROL = 3,
        CTRL_MODE_TRAJECTORY_CONTROL = 4
    };

    enum EncoderMode_t {
        ENCODER_MODE_INCREMENTAL = 0,
        ENCODER_MODE_HALL = 1
    };

    struct Feedback_t {
        float position;
        float velocity;
    } Feedback;

    ODriveClass(Stream& serial);

    // Commands
    void SetPosition(int motor_number, float position);
    void SetPosition(int motor_number, float position, float velocity_feedforward);
    void SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward);
    void SetVelocity(int motor_number, float velocity);
    void SetVelocity(int motor_number, float velocity, float current_feedforward);
    void SetCurrent(int motor_number, float current);
    void TrapezoidalMove(int motor_number, float position);
    void ReadFeedback(int motor_number);

    // Control Mode
    void SetControlModeTraj(int axis);
    void SetControlModeVel(int axis);

    // Motor Configuration Commands
    int MotorCalibrationStatus(int axis);
    void MotorPreCalibrated(int axis, bool request);

    // Encoder Configuration Commands
    int EncoderReadyStatus(int axis);
    void EncoderUseIndex(int axis, bool request);
    void EncoderPreCalibrated(int axis, bool request);

    // Startup Configuration Commands
    void StartupMotorCalibration(int axis, bool request);
    void StartupEncoderIndexSearch(int axis, bool request);
    void StartupEncoderOffsetCalibration(int axis, bool request);
    void StartupClosedLoop(int axis, bool request);
    void StartupSensorless(int axis, bool request);

    // Axis Limit Commands
    void ConfigureBrakingResistance(float braking_resistance);
    void ConfigureCurrentLimit(int axis, float current_limit);
    void ConfigureCalibrationCurrent(int axis, float current_calib);
    void ConfigureVelLimit(int axis, float velocity);
    void ConfigurePolePairs(int axis, int pole_pairs);
    void ConfigureMotorType(int axis, int motor_type);
    void ConfigureCPR(int axis, int cpr);
    void ConfigureEncoderMode(int axis, int mode);

    // Trajectory Limit Commands
    void ConfigureTrajVelLimit(int axis, float velocity);
    void ConfigureTrajAccelLimit(int axis, float acceleration);
    void ConfigureTrajDecelLimit(int axis, float deceleration);

    // PID Calibration Commands
    void ConfigurePosGain(int axis, float pos_gain);
    void ConfigureVelGain(int axis, float vel_gain);
    void ConfigureVelIntGain(int axis, float vel_int_gain);

    // System Commands
    float BusVoltage(void);
    void SaveConfiguration(void);
    void EraseConfiguration(void);
    void Reboot(void);

    // General params
    float readFloat();
    int32_t readInt();
    int32_t readState(int axis);

    // State helper
    bool run_state(int axis, int requested_state, bool wait);
private:
    String readString();

    Stream& serial_;
};

#endif //ODRIVELIB_H
