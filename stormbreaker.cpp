/* StormBreaker Source */

/* Includes-------------------------------------------------------------*/
#include "stormbreaker.h"
#include "calibration.h"

/* Constants -----------------------------------------------------------*/
#define MOTOR_ENCODER_COUNT 8192 //depends on the DIP switches inside the AMT102
#define OUTER_ENCODER_COUNT 65536 //the counts per revolution of the motor encoder
#define ARTNET_PAN_TILT_SCALING_FACTOR_360   1 //converts ArtNet 0-65,536 to 0-(65,536*factor)count where the max value is 360 degrees
#define ARTNET_PAN_TILT_SCALING_FACTOR_540   1.5 //converts ArtNet 0-65,536 to 0-(65,536*factor)count where the max value is 540 degrees
#define ARTNET_PAN_TILT_SCALING_FACTOR(VEL_LIMIT) (VEL_LIMIT/256) //converts ArtNet 0-255 to 0-(255*factor)counts/s where the max value is the velocity limit
#define ARTNET_VELOCITY_SCALING_FACTOR(VEL_LIMIT) (VEL_LIMIT/126) //converts ArtNet 2-127 or 130-255 to 0-(126*factor)counts/s where the max value is the velocity limit

/* Functions------------------------------------------------------------*/
void StormBreaker::serviceStormBreaker()
{
    Header.type = (StormBreaker::MessageType_t)pi_serial.read(); //TODO: check if read returns -1 and then throw an error (do this for all occurrences)
    Header.size = pi_serial.read();

    #ifdef TESTING
        SerialUSB.println("Received StormBreaker message");
        SerialUSB.print("Type: ");
        SerialUSB.print(Header.type);
        SerialUSB.print("   Size: ");
        SerialUSB.println(Header.size);
    #endif

    switch(Header.type){
    case ERROR:
        break;
    case WARNING:
        break;
    case OK:
        break;
    case ARTNETBODY:
        #if defined BODY || defined BOTH_FOR_TESTING
            receiveArtNetBody();
            serviceArtNetBody();
        #endif        
        break;

    case ARTNETHEAD:
        #if defined HEAD || defined BOTH_FOR_TESTING
            receiveArtNetHead();
            serviceArtNetHead();
        #endif
        break;

    case IDENTIFY:
        serviceIdentify();
        break;
    default:
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
    ArtNetPan();
    ArtNetPanTiltSpeed();
    ArtNetPowerSpecialFunctions();
}

// Handles both pan and pan control functions
void StormBreaker::ArtNetPan()
{
    switch(ArtNetBody.pan_control){
    case 0: //pan with 540 range
        odrive_.SetControlModeTraj(AXIS_BODY);
        //offset by half a rotation (to allow for panning in both directions) and scale for 540 degree range
        odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - (OUTER_ENCODER_COUNT / 2)) * ARTNET_PAN_TILT_SCALING_FACTOR_540);
        break;
    case 1: //pan with 360 range
        odrive_.SetControlModeTraj(AXIS_BODY);
        //offset by half a rotation (to allow for panning in both directions) and scale for 360 degree range
        odrive_.TrapezoidalMove(AXIS_BODY, (ArtNetBody.pan - (OUTER_ENCODER_COUNT / 2)) * ARTNET_PAN_TILT_SCALING_FACTOR_360);
        break;
    case 128: //stop in place
        odrive_.SetVelocity(AXIS_BODY, 0); //TODO: investigate why motors are "looser" in this state
        break;
    case 129: //stop and return to index position
        odrive_.SetControlModeTraj(AXIS_BODY); //TODO: investigate why setting this mode causes the motors to spin to the index position at max speed
        // odrive_.TrapezoidalMove(AXIS_BODY, 0);
        break;
    default: //continuous cw or ccw rotation
        odrive_.SetControlModeVel(AXIS_BODY);
        if((ArtNetBody.pan_control >= 2) && (ArtNetBody.pan_control <= 127)){
            //scale based on the velocity limit
            odrive_.SetVelocity(AXIS_BODY, (VEL_VEL_LIMIT - ((ArtNetBody.pan_control - 2) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
        } else if((ArtNetBody.pan_control >= 130) && (ArtNetBody.pan_control <= 255)){
            //scale based on the velocity limit (note how the -1 causes the equation to wrap around to negative values thus changing the direction of rotation)
            odrive_.SetVelocity(AXIS_BODY, (VEL_VEL_LIMIT - ((ArtNetBody.pan_control - 2 - 1) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
        }
        break;
    }        
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
    #endif
}

void StormBreaker::serviceArtNetHead()
{
    ArtNetStrobeShutter();
    ArtNetIris();
    ArtNetZoom();
    ArtNetFocus();
    ArtNetTilt();
    ArtNetPanTiltSpeed();
    ArtNetPowerSpecialFunctions();
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
}

void StormBreaker::ArtNetZoom()
{
    //control zoom stepper motor
}

void StormBreaker::ArtNetFocus()
{
    //control focus stepper motor
}

// Handles both tilt and tilt control functions
void StormBreaker::ArtNetTilt()
{
    switch(ArtNetHead.tilt_control){
    case 0: //tilt with 540 range
        odrive_.SetControlModeTraj(AXIS_HEAD);
        //offset by half a rotation (to allow for tilting in both directions) and scale for 540 degree range
        odrive_.TrapezoidalMove(AXIS_HEAD, (ArtNetHead.tilt - (OUTER_ENCODER_COUNT / 2)) * ARTNET_PAN_TILT_SCALING_FACTOR_540);
        break;
    case 1: //tilt with 360 range
        odrive_.SetControlModeTraj(AXIS_HEAD);
        //offset by half a rotation (to allow for tilting in both directions) and scale for 360 degree range
        odrive_.TrapezoidalMove(AXIS_HEAD, (ArtNetHead.tilt - (OUTER_ENCODER_COUNT / 2)) * ARTNET_PAN_TILT_SCALING_FACTOR_360);
        break;
    case 128: //stop in place
        odrive_.SetVelocity(AXIS_HEAD, 0); //TODO: investigate why motors are "looser" in this state
        break;
    case 129: //stop and return to index position
        odrive_.SetControlModeTraj(AXIS_HEAD); //TODO: investigate why setting this mode causes the motors to spin to the index position at max speed
        // odrive_.TrapezoidalMove(AXIS_HEAD, 0);
        break;
    default: //continuous cw or ccw rotation
        odrive_.SetControlModeVel(AXIS_HEAD);
        if((ArtNetHead.tilt_control >= 2) && (ArtNetHead.tilt_control <= 127)){
            //scale based on the velocity limit
            odrive_.SetVelocity(AXIS_HEAD, (VEL_VEL_LIMIT - ((ArtNetHead.tilt_control - 2) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
        } else if((ArtNetHead.tilt_control >= 130) && (ArtNetHead.tilt_control <= 255)){
            //scale based on the velocity limit (note how the -1 causes the equation to wrap around to negative values thus changing the direction of rotation)
            odrive_.SetVelocity(AXIS_HEAD, (VEL_VEL_LIMIT - ((ArtNetHead.tilt_control - 2 - 1) * ARTNET_VELOCITY_SCALING_FACTOR(VEL_VEL_LIMIT)))); //note velocity can never be zero
        }
        break;
    }
}
//
#endif

void StormBreaker::ArtNetPanTiltSpeed()
{
    #if defined BODY || defined BOTH_FOR_TESTING
        odrive_.ConfigureTrajVelLimit(AXIS_BODY, (TRAJ_VEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
    #endif
    #if defined HEAD || defined BOTH_FOR_TESTING
        odrive_.ConfigureTrajVelLimit(AXIS_HEAD, (TRAJ_VEL_LIMIT - (ArtNetHead.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
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
    pi_serial.println(IDENTIFIER);
}