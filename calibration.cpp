/* @file    calibration.cpp
 * @author  Carbon Video Systems LX-1
 * @description   O-drive calibration script
 */

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>

#include "calibration.h"

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
void odrive_startup_sequence(ODriveClass& odrive){

    bool calibration_status[NUM_MOTORS];
    odrive_startup_check(odrive, calibration_status);

    for (int axis = 0; axis < NUM_MOTORS; axis ++){
        if (calibration_status[axis] == false){

            #ifdef TESTING
                SerialUSB.print("Configuring axis ");
                SerialUSB.println(axis);
            #endif

            if (odrive.MotorCalibrationStatus(axis)){
                if (odrive.EncoderReadyStatus(axis)){
                    reconfigure_startup(odrive, axis);
                }
                else{
                    encoder_calibrate(odrive, axis);
                    reconfigure_startup(odrive, axis);
                }

            }
            else {
                // FULL CALIBRATION SEQUENCE
                set_axis_limits(odrive, axis);
                motor_calibrate(odrive, axis);
                encoder_calibrate(odrive, axis);
                reconfigure_startup(odrive, axis);
            }
        }
    }

    // save calibration and reboot if either axis needs reconfiguration
    if (!calibration_status[0] || !calibration_status[NUM_MOTORS -1]){
        #ifdef TESTING
            SerialUSB.println("Saving calibration and rebooting");
        #endif

        odrive.SaveConfiguration();
        delay(100);

        odrive.Reboot();
        delay(1000);
    }

    return;
}

/**
  * @brief  Startup checking for the ODrive
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @param  bool calibration_check [] to hold the calibration results
  * @return void
  */
void odrive_startup_check(ODriveClass& odrive, bool calibration_status[]){

    #ifdef TESTING
        SerialUSB.println("Searching for and waiting for ODrive");
    #endif
    
    int32_t current_state;
    // search for post-startup ODrive
    for (int axis = 0; axis < NUM_MOTORS; axis++) {

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
        }
        else{
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
void reconfigure_startup(ODriveClass& odrive, int axis){
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
void encoder_calibrate(ODriveClass& odrive, int axis){
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
void motor_calibrate(ODriveClass& odrive, int axis){
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
void set_axis_limits(ODriveClass& odrive, int axis){
    #ifdef TESTING
        SerialUSB.print("Setting axis limits for axis ");
        SerialUSB.println(axis);
    #endif

    odrive.ConfigureBrakingResistance(BRAKING_RESISTANCE);
    odrive.ConfigureCurrentLimit(axis, CURRENT_LIM);
    odrive.ConfigureCalibrationCurrent(axis, CALIBRATION_CURRENT);
    odrive.ConfigureVelLimit(axis, VEL_LIMIT);
    odrive.ConfigurePolePairs(axis, POLE_PAIRS);
    odrive.ConfigureMotorType(axis, MOTOR_TYPE);
    odrive.ConfigureCPR(axis, CPR);
    odrive.ConfigureEncoderMode(axis, ENCODER_MODE);
}