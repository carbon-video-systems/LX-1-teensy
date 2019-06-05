/* @file    calibration.cpp
 * @author  Carbon Video Systems LX-1
 * @description   O-drive calibration script
 */

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>
#include "calibration.h"

/* Variables -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
/**
  * @brief  Startup sequence for the ODrive
  * @param  ODriveClass& odrive - ODriveClass instantiated object
  * @return void
  */
void odrive_startup_sequence(ODriveClass& odrive){

    bool calibration_status[numMotors];
    odrive_startup_check(odrive, calibration_status);

    for (int axis = 0; axis < numMotors; axis ++){
        if (calibration_status[axis] == false){

            #ifdef TESTING
                SerialUSB.print("Configuring axis ");
                SerialUSB.println(axis);
            #endif

            if (odrive.MotorCalibrationStatus(axis)){
                if (odrive.EncoderReadyStatus(axis)){
                    ReconfigureStartup(odrive, axis);
                }
                else{
                    // calibrate encoder
                    ReconfigureStartup(odrive, axis);
                }

            }
            else {
                // FULL CALIBRATION SEQUENCE 
                // set axis limits
                // motor calibration
                // encoder calibration
                ReconfigureStartup(odrive, axis);
            }
        }
    }

    // save calibration and reboot if either axis needs reconfiguration
    if (!calibration_status[0] || !calibration_status[numMotors -1]){
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
    for (int axis = 0; axis < numMotors; axis++) {

        // current_state = odrive.readState(axis);

        // while(current_state != ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL || current_state != ODriveClass::AXIS_STATE_IDLE){
        //     delay(100);
        //     current_state = odrive.readState(axis);
        //     // Add code to handle a disconnected odrive
        // }

        do {
            current_state = odrive.readState(axis);
            delay(100);

        } while (current_state != ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL || current_state != ODriveClass::AXIS_STATE_IDLE);

        #ifdef TESTING
            SerialUSB.print("odrive ");
            SerialUSB.print(axis);
            SerialUSB.print(" state : ");;
            SerialUSB.println(current_state);
            
            SerialUSB.print("Calibration status: ");
        #endif

        if (current_state == ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL){
            calibration_status[axis] = true;

            #ifdef TESTING
                SerialUSB.println("calibrated\n");
            #endif
        }
        else{
            calibration_status[axis] = false;
            
            #ifdef TESTING
                SerialUSB.println("not calibrated\n");
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
void ReconfigureStartup(ODriveClass& odrive, int axis){
    #ifdef TESTING
        SerialUSB.print("Reconfiguring axis ");
        SerialUSB.print(axis);
        SerialUSB.println(" startup sequence");
    #endif

    odrive.StartupMotorCalibration(axis, false);
    odrive.StartupEncoderIndexSearch(axis, true);
    odrive.StartupEncoderOffsetCalibration(axis, false);
    odrive.StartupClosedLoop(axis, true);
    odrive.StartupSensorless(axis, false);
}