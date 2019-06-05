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

            // my_drive.axis0.motor.is_calibrated == true
            // my_drive.axis0.encoder.is_ready == true
            //      reset startup configuration
            // else
            //      calibrate encoder + startup configuration
            // else 
            //  full calibration : set axis limits + motor calibrate + encoder calibrate + startup config
            // calibration sequence
        }
    }

    // save calibration and reboot if either axis were reconfigured
    if (!calibration_status[0] || !calibration_status[numMotors -1]){
        delay(100);
        //save calibration + reboot
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
