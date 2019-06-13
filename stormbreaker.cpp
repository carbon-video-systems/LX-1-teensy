/* StormBreaker Source */

/* Includes-------------------------------------------------------------*/
#include "stormbreaker.h"
#include "calibration.h"

/* Constants -----------------------------------------------------------*/
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
            receiveArtNetBody();
            serviceArtNetBody();
            break;
        case ARTNETHEAD:
            receiveArtNetHead();
            serviceArtNetHead();
            break;
        case IDENTIFY:
            serviceIdentify();
            break;
        default:
            break;
    }
}

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
        SerialUSB.print(ArtNetBody.pan_control);
        SerialUSB.print(ArtNetBody.pan_tilt_speed);
        SerialUSB.println(ArtNetBody.power_special_functions);
    #endif
}

void StormBreaker::receiveArtNetHead()
{
    while(pi_serial.available() < Header.size){}

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
        SerialUSB.print(ArtNetHead.iris);
        SerialUSB.print(ArtNetHead.zoom);
        SerialUSB.print(ArtNetHead.focus);
        SerialUSB.print(ArtNetHead.tilt);
        SerialUSB.print(ArtNetHead.tilt_control);
        SerialUSB.print(ArtNetHead.pan_tilt_speed);
        SerialUSB.println(ArtNetHead.power_special_functions);
    #endif
}

void StormBreaker::serviceArtNetBody()
{
    ArtNetPan();
    ArtNetPanTiltSpeed();
    ArtNetPowerSpecialFunctions();
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

void StormBreaker::serviceIdentify()
{
    #ifdef BODY
        pi_serial.println(IDENTIFIER);
    #else
        pi_serial.println(IDENTIFIER);
    #endif
}

void StormBreaker::ArtNetPan()
{
    switch(ArtNetBody.pan_control){
        case 0:
            odrive_.SetControlModeTraj(AXIS_BODY);
            odrive_.TrapezoidalMove(AXIS_BODY, ArtNetBody.pan * ARTNET_PAN_TILT_SCALING_FACTOR_540);
            break;
        case 1:
            odrive_.SetControlModeTraj(AXIS_BODY);
            odrive_.TrapezoidalMove(AXIS_BODY, ArtNetBody.pan * ARTNET_PAN_TILT_SCALING_FACTOR_360);
            break;
        case 128:
            break;
        case 129:
            break;
        default:
            odrive_.SetControlModeVel(AXIS_BODY);
            if((ArtNetBody.pan_control >= 2) && (ArtNetBody.pan_control <= 127)){
                odrive_.SetVelocity(AXIS_BODY, (TRAJ_VEL_LIMIT - ((ArtNetBody.pan_control - 2) * ARTNET_VELOCITY_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
            } else if((ArtNetBody.pan_control >= 130) && (ArtNetBody.pan_control <= 255)){
                odrive_.SetVelocity(AXIS_BODY, (TRAJ_VEL_LIMIT - ((ArtNetBody.pan_control - 130) * ARTNET_VELOCITY_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
            }
            break;
    }        
}

void StormBreaker::ArtNetStrobeShutter()
{
    //control shutter stepper motor
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

void StormBreaker::ArtNetTilt()
{
    switch(ArtNetHead.tilt_control){
        case 0:
            odrive_.TrapezoidalMove(AXIS_HEAD, ArtNetHead.tilt * ARTNET_PAN_TILT_SCALING_FACTOR_540);
            break;
        case 1:
            odrive_.TrapezoidalMove(AXIS_HEAD, ArtNetHead.tilt * ARTNET_PAN_TILT_SCALING_FACTOR_360);
            break;
        case 128:
            break;
        case 129:
            break;
        default:
            odrive_.SetControlModeVel(AXIS_HEAD);
            if((ArtNetHead.tilt_control >= 2) && (ArtNetHead.tilt_control <= 127)){
                odrive_.SetVelocity(AXIS_HEAD, (TRAJ_VEL_LIMIT - ((ArtNetHead.tilt_control - 2) * ARTNET_VELOCITY_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
            } else if((ArtNetHead.tilt_control >= 130) && (ArtNetHead.tilt_control <= 255)){
                odrive_.SetVelocity(AXIS_HEAD, (TRAJ_VEL_LIMIT - ((ArtNetHead.tilt_control - 130) * ARTNET_VELOCITY_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
            }
            break;
    }    
}

void StormBreaker::ArtNetPanTiltSpeed()
{
    #ifdef BODY
        odrive_.ConfigureTrajVelLimit(AXIS_BODY, (TRAJ_VEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
    #else
        odrive_.ConfigureTrajVelLimit(AXIS_HEAD, (TRAJ_VEL_LIMIT - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SCALING_FACTOR(TRAJ_VEL_LIMIT)))); //note velocity can never be zero
    #endif
}

void StormBreaker::ArtNetPowerSpecialFunctions()
{
    #ifdef BODY

    #else

    #endif
}
