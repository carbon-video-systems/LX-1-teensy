/*
 * Calibration Source
 *
 * @file    calibration.cpp
 * @author  Carbon Video Systems 2019
 * @description   O-Drive calibration script.
 * This file reads the current state of the ODrive system
 * and calibrates the motors if required.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>

#include "calibration.h"
#include "stormbreaker.h"

/* Constants -----------------------------------------------------------*/
// ODrive Limits
#define BRAKING_RESISTANCE   2.0f   // ohms
#define CURRENT_LIM         10.0f  // amps
#define CALIBRATION_CURRENT 15.0f  //amps
#define VEL_LIMIT           300000.0f // counts/s
#define POLE_PAIRS          7   // magnet poles / 2
#define MOTOR_TYPE          ODriveClass::MOTOR_TYPE_HIGH_CURRENT
#define CPR                 8192    // counts/revolution
#define ENCODER_MODE        ODriveClass::ENCODER_MODE_INCREMENTAL

#define HOMING_VELOCITY     4096    //counts per second

// Scale from one odrive encoder count to one system encoder count
#define MAGNETIC_ENCODER_TOTAL  97600
#define MAGNETIC_ENCODER_HALF   48800
#define SYSTEM_CORRELATION  0.3525

// ODrive PID Calibration
#define PID_POS_GAIN_BODY        20.0f       //default 20
#define PID_VEL_GAIN_BODY        0.0005f     //default 0.0005
#define PID_VEL_INT_GAIN_BODY    0.001f     //default 0.001

#define PID_POS_GAIN_HEAD        50.0f       //default 20
#define PID_VEL_GAIN_HEAD        0.0005f     //default 0.0005
#define PID_VEL_INT_GAIN_HEAD    0.0025f     //default 0.001

// ODrive startup settings
#define STARTUP_MOTOR_CALIBRATION           false
#define STARTUP_ENCODER_SEARCH              true
#define STARTUP_ENCODER_OFFSET_CALIBRATION  false
#define STARTUP_CLOSED_LOOP                 true
#define STARTUP_SENSORLESS                  false

// ODrive Configuration settings
#define ENCODER_USE_INDEX       true
#define ENCODER_PRE_CALIBRATED  true
#define MOTOR_PRE_CALIBRATED    true

/* Functions------------------------------------------------------------*/
/**
  * @brief  Startup sequence for the ODrive
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @return void
  */
void odrive_startup_sequence(ODriveClass& odrive)
{
    bool calibration_status[2] = {true, true};

    odrive_startup_check(odrive, calibration_status);

    #if defined BOTH_FOR_TESTING
        for (int axis = 0; axis < NUM_MOTORS; axis++) {
    #elif defined BODY
        for (int axis = AXIS_BODY; axis < (NUM_MOTORS + AXIS_BODY); axis++) {
    #elif defined HEAD
        for (int axis = AXIS_HEAD; axis < (NUM_MOTORS + AXIS_HEAD); axis++) {
    #endif

            if (calibration_status[axis] == false){

                #ifdef TESTING
                    SerialUSB.print("Configuring axis ");
                    SerialUSB.println(axis);
                #endif

                if (odrive.MotorCalibrationStatus(axis)){
                    if (odrive.EncoderReadyStatus(axis)){
                        reconfigure_startup(odrive, axis);
                    } else {
                        encoder_calibrate(odrive, axis);
                        reconfigure_startup(odrive, axis);
                    }
                } else {
                    // FULL CALIBRATION SEQUENCE
                    parameter_configuration(odrive, axis);
                    motor_calibrate(odrive, axis);
                    encoder_calibrate(odrive, axis);
                    reconfigure_startup(odrive, axis);
                }
            }

            odrive.SetControlModePos(axis);
        }

    // save calibration and reboot if either axis needs reconfiguration
    if (!calibration_status[0] || !calibration_status[1]){
        #ifdef TESTING
            SerialUSB.println("Saving calibration and rebooting");
        #endif

        odrive.SaveConfiguration();
        delay(100);

        odrive.Reboot();
        delay(2000);
    }

    return;
}

/**
  * @brief  Startup checking for the ODrive
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  bool calibration_check [] to hold the calibration results
  * @return void
  */
void odrive_startup_check(ODriveClass& odrive, bool calibration_status[])
{
    #ifdef TESTING
        SerialUSB.println("Searching for and waiting for ODrive");
    #endif

    int32_t current_state;
    // search for post-startup ODrive

    #if defined BOTH_FOR_TESTING
        for (int axis = 0; axis < NUM_MOTORS; axis++) {
    #elif defined BODY
        for (int axis = AXIS_BODY; axis < (NUM_MOTORS + AXIS_BODY); axis++) {
    #elif defined HEAD
        for (int axis = AXIS_HEAD; axis < (NUM_MOTORS + AXIS_HEAD); axis++) {
    #endif

            do {
                current_state = odrive.readState(axis);
                delay(100);
            } while (current_state != ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL && current_state != ODriveClass::AXIS_STATE_IDLE);

            #ifdef TESTING
                SerialUSB.print("ODrive ");
                SerialUSB.print(axis);
                SerialUSB.print(" state : ");;
                SerialUSB.println(current_state);

                SerialUSB.print("Calibration status: ");
            #endif

            if (current_state == ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL){
                calibration_status[axis] = true;

                #ifdef TESTING
                    SerialUSB.println("calibrated");
                #endif
            } else {
                calibration_status[axis] = false;

                #ifdef TESTING
                    SerialUSB.println("not calibrated");
                #endif
            }
        }

    return;
}

/**
  * @brief  Reconfigure ODrive startup sequence
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  int axis - motor axis to be reconfigured
  * @return void
  */
void reconfigure_startup(ODriveClass& odrive, int axis)
{
    #ifdef TESTING
        SerialUSB.print("Reconfiguring axis ");
        SerialUSB.print(axis);
        SerialUSB.println(" startup sequence");
    #endif

    odrive.StartupMotorCalibration(axis, STARTUP_MOTOR_CALIBRATION);
    odrive.StartupEncoderIndexSearch(axis, STARTUP_ENCODER_SEARCH);
    odrive.StartupEncoderOffsetCalibration(axis, STARTUP_ENCODER_OFFSET_CALIBRATION);
    odrive.StartupClosedLoop(axis, STARTUP_CLOSED_LOOP);
    odrive.StartupSensorless(axis, STARTUP_SENSORLESS);
}

/**
  * @brief  Calibrate odrive encoder
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  int axis - encoder axis to be calibrated
  * @return void
  */
void encoder_calibrate(ODriveClass& odrive, int axis)
{
    #ifdef TESTING
        SerialUSB.print("Calibrating axis ");
        SerialUSB.print(axis);
        SerialUSB.println(" encoder");
    #endif

    odrive.EncoderUseIndex(axis, ENCODER_USE_INDEX);
    odrive.run_state(axis, ODriveClass::AXIS_STATE_ENCODER_INDEX_SEARCH, true);
    odrive.run_state(axis, ODriveClass::AXIS_STATE_ENCODER_OFFSET_CALIBRATION, true);
    odrive.EncoderPreCalibrated(axis, ENCODER_PRE_CALIBRATED);
    delay(50);
}

/**
  * @brief  Calibrate odrive motor
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  int axis - motor axis to be calibrated
  * @return void
  */
void motor_calibrate(ODriveClass& odrive, int axis)
{
    #ifdef TESTING
        SerialUSB.print("Calibrating axis ");
        SerialUSB.print(axis);
        SerialUSB.println(" motor");
    #endif

    odrive.run_state(axis, ODriveClass::AXIS_STATE_MOTOR_CALIBRATION, true);
    odrive.MotorPreCalibrated(axis, MOTOR_PRE_CALIBRATED);
}

/**
  * @brief  Sets ODrive limits
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  int axis - axis to be configured
  * @return void
  */
void parameter_configuration(ODriveClass& odrive, int axis)
{
    #ifdef TESTING
        SerialUSB.print("Configuring parameters for axis ");
        SerialUSB.println(axis);
    #endif

    odrive.SetControlModeTraj(axis);

    odrive.ConfigureBrakingResistance(BRAKING_RESISTANCE);
    odrive.ConfigureCurrentLimit(axis, CURRENT_LIM);
    odrive.ConfigureCalibrationCurrent(axis, CALIBRATION_CURRENT);
    odrive.ConfigureVelLimit(axis, VEL_LIMIT);
    odrive.ConfigurePolePairs(axis, POLE_PAIRS);
    odrive.ConfigureMotorType(axis, MOTOR_TYPE);
    odrive.ConfigureCPR(axis, CPR);
    odrive.ConfigureEncoderMode(axis, ENCODER_MODE);

    odrive.ConfigureTrajVelLimit(axis, TRAJ_VEL_LIMIT);
    odrive.ConfigureTrajAccelLimit(axis, TRAJ_ACCEL_LIMIT);
    odrive.ConfigureTrajDecelLimit(axis, TRAJ_DECEL_LIMIT);

    #if defined BODY || defined BOTH_FOR_TESTING
        if(axis == AXIS_BODY){
            odrive.ConfigurePosGain(axis, PID_POS_GAIN_BODY);
            odrive.ConfigureVelGain(axis, PID_VEL_GAIN_BODY);
            odrive.ConfigureVelIntGain(axis, PID_VEL_INT_GAIN_BODY);
        }
    #elif defined HEAD || defined BOTH_FOR_TESTING
        if(axis == AXIS_HEAD){
            odrive.ConfigurePosGain(axis, PID_POS_GAIN_HEAD);
            odrive.ConfigureVelGain(axis, PID_VEL_GAIN_HEAD);
            odrive.ConfigureVelIntGain(axis, PID_VEL_INT_GAIN_HEAD);
        }
    #endif
}

/**
  * @brief  LX1 system startup homing sequence
  * @param  ODriveClass& odrive - ODriveClass instantiated class object
  * @param  LS7366R& encoder - LS7366R encoder instantiated class object
  * @param  StormBreaker& thor - StormBreaker instantiated class object
  * @return void
  */
void lx1_startup_sequence(ODriveClass& odrive, LS7366R& encoder, StormBreaker& thor){
    // Homing the odrive system
    #if defined HEAD || defined BOTH_FOR_TESTING
        int axis = AXIS_HEAD;
    #else
        int axis = AXIS_BODY;
    #endif

    startup_index_search(odrive, axis);
    system_direction(encoder, thor, axis);
    startup_index(odrive, encoder, thor, axis);
    homing_system(odrive, thor.SystemIndex.pan_index, axis, true);

    #ifdef TESTING
        odrive.ReadFeedback(axis);
        int32_t encoder_count = encoder.counterRead();
        SerialUSB.print("ODrive encoder count: ");
        SerialUSB.println(odrive.Feedback.position);
        SerialUSB.print("Homed Encoder Count: ");
        SerialUSB.println(encoder_count);
        SerialUSB.println();
    #endif
}

/**
  * @brief  Searches for system index
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  int axis - axis to be configured
  * @return void
  */
 void startup_index_search(ODriveClass& odrive, int axis){
    // Set up odrive for homing spin
    odrive.ConfigureTrajVelLimit(axis, HOMING_VELOCITY);
    odrive.SetControlModeTraj(axis);
    odrive.TrapezoidalMove(axis, (CPR * (TENSION_SCALING_FACTOR)));
    delay(4000);
    do {
        delay(250);
        odrive.ReadFeedback(axis);
    }
    while (abs(odrive.Feedback.velocity) >= 2);
 }

/**
  * @brief  Identifies the system spin direction on startup
  * @param  LS7366R& encoder - LS7366R encoder instantiated class object
  * @param  StormBreaker& thor - StormBreaker instantiated class object
  * @param  int axis - axis to be configured
  * @return void
  */
 void system_direction(LS7366R& encoder, StormBreaker& thor, int axis){

    int32_t encoder_count = encoder.counterRead();

    if (encoder_count < 0)
        thor.SystemIndex.encoder_direction = false;
    else
        thor.SystemIndex.encoder_direction = true;
 }

/**
  * @brief  Calculates the system index on startup
  * @param  ODriveClass& odrive - ODriveClass instantiated class object
  * @param  LS7366R& encoder - LS7366R encoder instantiated class object
  * @param  StormBreaker& thor - StormBreaker instantiated class object
  * @param  int axis - axis to be indexed
  * @return void
  */
void startup_index(ODriveClass& odrive, LS7366R& encoder, StormBreaker& thor, int axis){

    odrive.ReadFeedback(axis);
    int32_t encoder_count = encoder.counterRead();

    #if defined HEAD || defined BOTH_FOR_TESTING
        thor.SystemIndex.pan_index = system_reindex(odrive.Feedback.position, encoder_count, thor.SystemIndex.encoder_direction);
        #ifdef TESTING
            SerialUSB.print("Pan Index: ");
            SerialUSB.println(thor.SystemIndex.pan_index);
        #endif
    #endif

    #if defined BODY
        thor.SystemIndex.tilt_index = system_reindex(odrive.Feedback.position, encoder_count, thor.SystemIndex.encoder_direction);
        #ifdef TESTING
            SerialUSB.print("Tilt Index: ");
            SerialUSB.println(thor.SystemIndex.tilt_index);
        #endif
    #endif
 }

/**
  * @brief  Recalculates the system index
  * @param  float odrive_position - position of the odrive encoder
  * @param  int32_t encoder_count - position of the system magnetic encoder
  * @param  bool direction - direction that the system is installed
  * @return int32_t index - new calculated system index
  */
int32_t system_reindex(float odrive_position, int32_t encoder_count, bool direction){

    int32_t index;
    int32_t abs_encoder_count = abs(encoder_count);

    if (direction){
        if (abs_encoder_count <= MAGNETIC_ENCODER_HALF)
            index = odrive_position - (encoder_count * SYSTEM_CORRELATION);
        else
            index = odrive_position + ((- MAGNETIC_ENCODER_TOTAL - encoder_count) * SYSTEM_CORRELATION);
    } else{
        if (abs_encoder_count <= MAGNETIC_ENCODER_HALF)
            index = odrive_position + (encoder_count * SYSTEM_CORRELATION);
        else
            index = odrive_position - ((MAGNETIC_ENCODER_TOTAL - encoder_count) * SYSTEM_CORRELATION);
    }

    return index;
}

/**
  * @brief  Homes one system axis
  * @param  ODriveClass& odrive - ODriveClass instantiated class object
  * @param  int32_t index - index to home the system to
  * @param  int axis - axis to be configured
  * @param  bool startup - startup delay status
  * @return void
  */
 void homing_system(ODriveClass& odrive, int32_t index, int axis, bool startup = false){

    odrive.ConfigureTrajVelLimit(axis, HOMING_VELOCITY);
    odrive.SetControlModeTraj(axis);

    odrive.TrapezoidalMove(axis, index);

    if (startup){
        delay(4000);
        do {
            delay(250);
            odrive.ReadFeedback(axis);
        }
        while (abs(odrive.Feedback.velocity) >= 2);
    }
 }
