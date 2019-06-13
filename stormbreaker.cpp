#include "stormbreaker.h"

#define ARTNET_PAN_TILT_SPEED_SCALING_FACTOR 30 //converts ArtNet 0-255 to 0-(255*factor) counts/s

void StormBreaker::serviceStormBreaker()
{
    Header.type = (StormBreaker::MessageType_t)pi_serial.read();
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
        default:
            break;
    }
}

void StormBreaker::receiveArtNetBody()
{
    while(pi_serial.available() < Header.size){}
    
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
    ArtNetPanControl();
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
    ArtNetTiltControl();
    ArtNetPanTiltSpeed();
    ArtNetPowerSpecialFunctions();
}

void StormBreaker::ArtNetPan()
{
    odrive_.SetPosition(MOTOR_BODY, ArtNetBody.pan);
}

void StormBreaker::ArtNetPanControl()
{

}

void StormBreaker::ArtNetStrobeShutter()
{

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
    odrive_.SetPosition(MOTOR_HEAD, ArtNetHead.tilt);
}

void StormBreaker::ArtNetTiltControl()
{

}

void StormBreaker::ArtNetPanTiltSpeed()
{
    #ifdef BODY
        odrive_.ConfigureTrajVelLimit(MOTOR_BODY, (7650 - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SPEED_SCALING_FACTOR))); 
        SerialUSB.println((7650 - (ArtNetBody.pan_tilt_speed * ARTNET_PAN_TILT_SPEED_SCALING_FACTOR)));
    #else
        odrive_.ConfigureTrajVelLimit(MOTOR_HEAD, (ArtNetHead.pan_tilt_speed * ARTNET_PAN_TILT_SPEED_SCALING_FACTOR));
    #endif
}

void StormBreaker::ArtNetPowerSpecialFunctions()
{

}
