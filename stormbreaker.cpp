/*
 * StormBreaker Source
 *
 * @file    stormbreaker.cpp
 * @author  Carbon Video Systems 2019
 * @description   StormBreaker Communication Protocol Framework.
 * Sets the framework of the stormbreaker protocol, both reception and
 * transmission.  This file also services received StormBreaker packets.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include "stormbreaker.h"
#include "calibration.h"
#include "led.h"

/* Constants -----------------------------------------------------------*/
#define MAX_STORMBREAKER_LENGTH 14  // maximum size of a stormbreaker message

#define MOTOR_ENCODER_COUNT CPR //depends on the DIP switches inside the AMT102
#define PAN_TILT_COUNT_MAXIMUM 65536 //2 byte resolution for pan/tilt control
#define PAN_TILT_COUNT_MIDPOINT 32768 //half of the 2 byte resolution
#define PAN_TILT_SCALING_FACTOR 8    // PAN_TILT_COUNT_MAXIMUM/MOTOR_ENCODER_COUNT

#define ARTNET_PAN_TILT_SCALING_FACTOR_270   0.75 //converts ArtNet 0-65,536 to 0-(65,536*factor)count where the max value is 270 degrees
#define ARTNET_PAN_TILT_SCALING_FACTOR_360   1 //converts ArtNet 0-65,536 to 0-(65,536*factor)count where the max value is 360 degrees
#define ARTNET_PAN_TILT_SCALING_FACTOR_540   1.5 //converts ArtNet 0-65,536 to 0-(65,536*factor)count where the max value is 540 degrees

#define ARTNET_PAN_TILT_SCALING_FACTOR(VELOCITY_LIMIT) (VELOCITY_LIMIT/256) //converts ArtNet 0-255 to 0-(255*factor)counts/s where the max value is the velocity limit
#define ARTNET_VELOCITY_SCALING_FACTOR(VELOCITY_LIMIT) (VELOCITY_LIMIT/126) //converts ArtNet 2-127 or 130-255 to 0-(126*factor)counts/s where the max value is the velocity limit

/* Functions------------------------------------------------------------*/
void StormBreaker::serviceStormBreaker()
{
    Header.type = (StormBreaker::MessageType_t)pi_serial.read();

    #ifdef TESTING
        SerialUSB.println("Received message");
        SerialUSB.print("Type: ");
        SerialUSB.print(Header.type);
    #endif

    switch(Header.type){
    case ARTNETBODY:
        break;
    case ARTNETHEAD:
        break;
    case IDENTIFY:
        break;
    default:
        #ifdef TESTING
            SerialUSB.println("TYPE ERROR");
        #endif
        return;
        break;
    }

    Header.size = (StormBreaker::MessageSize_t)pi_serial.read();

    #ifdef TESTING
        SerialUSB.print("   Size: ");
        SerialUSB.println(Header.size);
    #endif

    switch(Header.size){
    case SIZE_IDENT:
        if (Header.type == IDENTIFY)
            serviceIdentify();
        else{
            #ifdef TESTING
                SerialUSB.println("SIZE ERROR");
            #endif
        }
        break;
    case SIZE_BODY:
        if (Header.type == ARTNETBODY){
            #if defined BODY || defined BOTH_FOR_TESTING
                receiveArtNetBody();
                serviceArtNetBody();
            #endif
        }
        else{
            #ifdef TESTING
                SerialUSB.println("SIZE ERROR");
            #endif
        }
        break;
    case SIZE_HEAD:
        if (Header.type == ARTNETHEAD){
            #if defined HEAD || defined BOTH_FOR_TESTING
                receiveArtNetHead();
                serviceArtNetHead();
            #endif
        }
        else{
            #ifdef TESTING
                SerialUSB.println("SIZE ERROR");
            #endif
        }
        break;
    default:
        #ifdef TESTING
            SerialUSB.println("SIZE ERROR");
        #endif
        break;
    }
}

#if defined BODY || defined BOTH_FOR_TESTING
//
void StormBreaker::receiveArtNetBody()
{
    while(pi_serial.available() < Header.size){} //TODO: add a timeout (do this for all occurrences)

    ArtNetBody.pan = (pi_serial.read() << 8) | pi_serial.read();
    ArtNetBody.pan_control = pi_serial.read();
    ArtNetBody.pan_tilt_speed = pi_serial.read();
    ArtNetBody.power_special_functions = pi_serial.read();

    #ifdef TESTING
        SerialUSB.print("ArtNetBody packet: ");
        SerialUSB.print(ArtNetBody.pan);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetBody.pan_control);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetBody.pan_tilt_speed);
        SerialUSB.print(" ");
        SerialUSB.println(ArtNetBody.power_special_functions);
    #endif
}

void StormBreaker::serviceArtNetBody()
{
    ArtNetPanTiltSpeed();
    ArtNetPan();
    ArtNetPowerSpecialFunctions();
}

// Handles both pan and pan control functions
void StormBreaker::ArtNetPan()
{
    static uint8_t prev_pan_control = 0;
    static uint16_t prev_pan = 0;

    if (ArtNetBody.pan_control != prev_pan_control || ArtNetBody.pan != prev_pan){
        switch(ArtNetBody.pan_control){
        case 0: //pan with 540 range
            if (prev_pan_control != 0 && prev_pan_control != 1 && prev_pan_control != 129){
                odrive_.SetVelocity(AXIS_BODY, 0);
                pan_reindex();

                odrive_.SetPosition(AXIS_BODY, odrive_.Feedback.position);
                odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_540 + SystemIndex.pan_index);
                odrive_.SetControlModeTraj(AXIS_BODY);
            }
            //offset by half a rotation (to allow for panning in both directions) and scale for 540 degree range
            else
                odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_540 + SystemIndex.pan_index);
            break;
        case 1: //pan with 360 range
            if (prev_pan_control != 0 && prev_pan_control != 1 && prev_pan_control != 129){
                odrive_.SetVelocity(AXIS_BODY, 0);
                pan_reindex();

                odrive_.SetPosition(AXIS_BODY, odrive_.Feedback.position);
                odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_360 + SystemIndex.pan_index);
                odrive_.SetControlModeTraj(AXIS_BODY);
            }
            //offset by half a rotation (to allow for panning in both directions) and scale for 360 degree range
            else
                odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_360 + SystemIndex.pan_index);
            break;
        case 128: //stop in place
            odrive_.SetVelocity(AXIS_BODY, 0); //TODO: investigate why motors are "looser" in this state

            if (prev_pan_control == 0 || prev_pan_control == 1 || prev_pan_control == 129)
                odrive_.SetControlModeVel(AXIS_BODY);
            break;
        case 129: //stop and return to index position
            if (prev_pan_control != 129){
                odrive_.SetVelocity(AXIS_BODY, 0);
                odrive_.ReadFeedback(AXIS_BODY);
                odrive_.SetPosition(AXIS_BODY, odrive_.Feedback.position);
                odrive_.SetControlModePos(AXIS_BODY);
                // Reindex
                pan_reindex();
                homing_system(odrive_, SystemIndex.pan_index, AXIS_BODY, false);
            }
            break;
        default: //continuous cw or ccw rotation
            if((ArtNetBody.pan_control >= 2) && (ArtNetBody.pan_control <= 127)){
                //scale based on the velocity limit CW
                odrive_.SetVelocity(AXIS_BODY, (VEL_VEL_LIMIT - ((ArtNetBody.pan_control - 2) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
            } else if((ArtNetBody.pan_control >= 130) && (ArtNetBody.pan_control <= 255)){
                //scale based on the velocity limit CCW
                // odrive_.SetVelocity(AXIS_BODY, (-VEL_VEL_LIMIT + ((ArtNetBody.pan_control - 130) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
                odrive_.SetVelocity(AXIS_BODY, ((129 - ArtNetBody.pan_control) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT))); //note velocity can never be zero
            }

            if (prev_pan_control == 0 || prev_pan_control == 1 || prev_pan_control == 129)
                odrive_.SetControlModeVel(AXIS_BODY);
            break;
        }
    prev_pan_control = ArtNetBody.pan_control;
    prev_pan = ArtNetBody.pan;
    }
}

// PAN reindexing
void StormBreaker::pan_reindex()
{
    odrive_.ReadFeedback(AXIS_BODY);
    SystemIndex.pan_index = system_reindex(odrive_.Feedback.position, SystemIndex.start_index);
}

//
#endif


#if defined HEAD || defined BOTH_FOR_TESTING
//
void StormBreaker::receiveArtNetHead()
{
    while(pi_serial.available() < Header.size){} //TODO: add a timeout (do this for all occurrences)

    ArtNetHead.strobe_shutter = pi_serial.read();
    ArtNetHead.iris = pi_serial.read();
    ArtNetHead.zoom = (pi_serial.read() << 8) | pi_serial.read();
    ArtNetHead.focus = (pi_serial.read() << 8) | pi_serial.read();
    ArtNetHead.tilt = (pi_serial.read() << 8) | pi_serial.read();
    ArtNetHead.tilt_control = pi_serial.read();
    ArtNetHead.pan_tilt_speed = pi_serial.read();
    ArtNetHead.power_special_functions = pi_serial.read();
    ArtNetHead.led_ring_red = pi_serial.read();
    ArtNetHead.led_ring_green = pi_serial.read();
    ArtNetHead.led_ring_blue = pi_serial.read();

    #ifdef TESTING
        SerialUSB.print("ArtNetHead packet: ");
        SerialUSB.print(ArtNetHead.strobe_shutter);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.iris);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.zoom);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.focus);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.tilt);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.tilt_control);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.pan_tilt_speed);
        SerialUSB.print(" ");
        SerialUSB.println(ArtNetHead.power_special_functions);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.led_ring_red);
        SerialUSB.print(" ");
        SerialUSB.print(ArtNetHead.led_ring_green);
        SerialUSB.print(" ");
        SerialUSB.println(ArtNetHead.led_ring_blue);
    #endif
}

void StormBreaker::serviceArtNetHead()
{
    ArtNetStrobeShutter();
    ArtNetIris();
    ArtNetZoom();
    ArtNetFocus();
    ArtNetPanTiltSpeed();
    ArtNetTilt();
    ArtNetPowerSpecialFunctions();
    #if defined LED_RING
        ArtNetLEDRing();
    #endif
}

void StormBreaker::ArtNetStrobeShutter()
{
    if((ArtNetHead.strobe_shutter >= 0) && (ArtNetHead.strobe_shutter <= 19)){
        //shutter closed
    } else if((ArtNetHead.strobe_shutter >= 20) && (ArtNetHead.strobe_shutter <= 49)){
        //shutter open
    } else if((ArtNetHead.strobe_shutter >= 50) && (ArtNetHead.strobe_shutter <= 200)){
        //strobe slow to fast
    } else if((ArtNetHead.strobe_shutter >= 201) && (ArtNetHead.strobe_shutter <= 210)){
        //shutter open
    } else if((ArtNetHead.strobe_shutter >= 211) && (ArtNetHead.strobe_shutter <= 255)){
        //random strobe slow to fast
    }
}

void StormBreaker::ArtNetIris()
{
    //control iris stepper motor
    // constrain and map input to a range
}

void StormBreaker::ArtNetZoom()
{
    //control zoom stepper motor
    // constrain and map input to a range
}

void StormBreaker::ArtNetFocus()
{
    //control focus stepper motor
    // constrain and map input to a range
}

void StormBreaker::ArtNetLEDRing()
{
    ArtNetLEDUpdate(ArtNetHead.led_ring_red, ArtNetHead.led_ring_green, ArtNetHead.led_ring_blue);
}

// Handles both tilt and tilt control functions
void StormBreaker::ArtNetTilt()
{
    static uint8_t prev_tilt_control = 0;
    static uint16_t prev_tilt = 0;

    if (ArtNetHead.tilt_control != prev_tilt_control || ArtNetHead.tilt != prev_tilt){
        switch(ArtNetHead.tilt_control){
        case 0: //tilt with 270 range
            if (prev_tilt_control != 0 && prev_tilt_control != 128){
                odrive_.SetVelocity(AXIS_HEAD, 0);
                tilt_reindex();

                odrive_.SetPosition(AXIS_HEAD, odrive_.Feedback.position);
                odrive_.TrapezoidalMove(AXIS_HEAD, (ArtNetHead.tilt - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_270 + SystemIndex.tilt_index);
                odrive_.SetControlModeTraj(AXIS_HEAD);
            }
            //offset by half a rotation (to allow for tilting in both directions) and scale for 270 degree range
            else
                odrive_.TrapezoidalMove(AXIS_HEAD, (ArtNetHead.tilt - PAN_TILT_COUNT_MIDPOINT) / PAN_TILT_SCALING_FACTOR * TENSION_SCALING_FACTOR * ARTNET_PAN_TILT_SCALING_FACTOR_270 + SystemIndex.tilt_index);
            break;
        case 127: //stop in place
            odrive_.SetVelocity(AXIS_HEAD, 0);
            if (prev_tilt_control == 0 || prev_tilt_control == 128)
                odrive_.SetControlModeVel(AXIS_HEAD);
            break;
        case 128: //stop and return to index position
            if (prev_tilt_control != 128){
                odrive_.SetVelocity(AXIS_HEAD, 0);
                odrive_.ReadFeedback(AXIS_HEAD);
                odrive_.SetPosition(AXIS_HEAD, odrive_.Feedback.position);
                odrive_.SetControlModePos(AXIS_HEAD);
                // Reindex
                tilt_reindex();
            }
            break;
        case 129: //stop in place
            odrive_.SetVelocity(AXIS_HEAD, 0); //TODO: investigate why motors are "looser" in this state
            if (prev_tilt_control == 0 || prev_tilt_control == 128)
                odrive_.SetControlModeVel(AXIS_HEAD);
            break;
        default: //continuous cw or ccw rotation
            if((ArtNetHead.tilt_control >= 1) && (ArtNetHead.tilt_control <= 126)){
                //scale based on the velocity limit CW
                odrive_.SetVelocity(AXIS_HEAD, (VEL_VEL_LIMIT - ((ArtNetHead.tilt_control - 1) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
            } else if((ArtNetHead.tilt_control >= 130) && (ArtNetHead.tilt_control <= 255)){
                //scale based on the velocity limit CCW
                odrive_.SetVelocity(AXIS_HEAD, ((129 - ArtNetHead.tilt_control) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT))); //note velocity can never be zero
                // odrive_.SetVelocity(AXIS_HEAD, (-VEL_VEL_LIMIT + ((ArtNetHead.tilt_control - 130) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
            }
            if (prev_tilt_control == 0 || prev_tilt_control == 128)
                odrive_.SetControlModeVel(AXIS_HEAD);
            break;
        }
    }
    prev_tilt_control = ArtNetHead.tilt_control;
    prev_tilt = ArtNetHead.tilt;
}

// TILT reindexing
void StormBreaker::tilt_reindex()
{
    odrive_.ReadFeedback(AXIS_HEAD);
    SystemIndex.tilt_index = system_reindex(odrive_.Feedback.position, SystemIndex.start_index);
}

//
#endif  // HEAD || BOTH_FOR_TESTING

void StormBreaker::ArtNetPanTiltSpeed()
{
    static bool startup = true;
    static uint8_t prev_pan_tilt_speed = 0;

    #if defined BODY || defined BOTH_FOR_TESTING
        if (startup == true){
            odrive_.ConfigureTrajAccelLimit(AXIS_BODY, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajDecelLimit(AXIS_BODY, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajVelLimit(AXIS_BODY, TRAJ_VEL_LIMIT);
            prev_pan_tilt_speed = ArtNetBody.pan_tilt_speed;
            startup = false;
        }
        if (ArtNetBody.pan_tilt_speed != prev_pan_tilt_speed){
            odrive_.ConfigureTrajAccelLimit(AXIS_BODY, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajDecelLimit(AXIS_BODY, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            prev_pan_tilt_speed = ArtNetBody.pan_tilt_speed;

        }
    #endif
    #if defined HEAD || defined BOTH_FOR_TESTING
        if (startup == true){
            odrive_.ConfigureTrajAccelLimit(AXIS_HEAD, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajDecelLimit(AXIS_HEAD, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajVelLimit(AXIS_HEAD, TRAJ_VEL_LIMIT); //note velocity can never be zero
            prev_pan_tilt_speed = ArtNetHead.pan_tilt_speed;
            startup = false;
        }
        if (ArtNetHead.pan_tilt_speed != prev_pan_tilt_speed){
            odrive_.ConfigureTrajAccelLimit(AXIS_HEAD, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            odrive_.ConfigureTrajDecelLimit(AXIS_HEAD, (TRAJ_ACCEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_ACCEL_LIMIT)))); //note velocity can never be zero
            prev_pan_tilt_speed = ArtNetHead.pan_tilt_speed;
        }
    #endif
}

void StormBreaker::ArtNetPowerSpecialFunctions()
{
    #if defined BODY || defined BOTH_FOR_TESTING
        if(ArtNetBody.power_special_functions == 1){
            //dimmer curve square
        } else if(ArtNetBody.power_special_functions == 2){
            //dimmer curve inverse square
        } else if(ArtNetBody.power_special_functions == 3){
            //dimmer curve linear
        } else if(ArtNetBody.power_special_functions == 4){
            //dimmer curve S
        } else if(ArtNetBody.power_special_functions == 5){
            //blackout while pan/tilt on
        } else if(ArtNetBody.power_special_functions == 6){
            //blackout while pan/tilt off
        } else if(ArtNetBody.power_special_functions == 7){
            //fixture reset
        } else if(ArtNetBody.power_special_functions == 8){
            //laser power off
        } else if(ArtNetBody.power_special_functions == 9){
            //laser power on
        }
    #endif

    #if defined HEAD || defined BOTH_FOR_TESTING
        if(ArtNetHead.power_special_functions == 1){
            //dimmer curve square
        } else if(ArtNetHead.power_special_functions == 2){
            //dimmer curve inverse square
        } else if(ArtNetHead.power_special_functions == 3){
            //dimmer curve linear
        } else if(ArtNetHead.power_special_functions == 4){
            //dimmer curve S
        } else if(ArtNetHead.power_special_functions == 5){
            //blackout while pan/tilt on
        } else if(ArtNetHead.power_special_functions == 6){
            //blackout while pan/tilt off
        } else if(ArtNetHead.power_special_functions == 7){
            //fixture reset
        } else if(ArtNetHead.power_special_functions == 8){
            //laser power off
        } else if(ArtNetHead.power_special_functions == 9){
            //laser power on
        }
    #endif
}

void StormBreaker::serviceIdentify()
{
    pi_serial.write(IDENTIFIER);
    pi_serial.println();
}
