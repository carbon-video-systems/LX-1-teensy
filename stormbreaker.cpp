#include "stormbreaker.h"
#include "options.h"

void StormBreaker::serviceStormBreaker()
{
    Header.type = (StormBreaker::MessageType_t)SerialUSB.read();
    Header.size = SerialUSB.read();

    #ifdef TESTING
        SerialUSB.println("Received StormBreaker message");
        SerialUSB.print("Type: ");
        SerialUSB.print(Header.type);
        SerialUSB.print("   Size: ");
        SerialUSB.println(Header.size);
    #endif

    receiveStormBreaker();

    switch(Header.type){
        case ERROR:
            break;
        case WARNING:
            break;
        case OK:
            break;
        case ARTNETBODY:
            serviceArtNetBody();
            break;
        case ARTNETHEAD:
            serviceArtNetHead();
            break;
        default:
            break;
    }
}

void StormBreaker::receiveStormBreaker()
{
    switch(Header.type){
        case ERROR:
            break;
        case WARNING:
            break;
        case OK:
            break;
        case ARTNETBODY:
            receiveArtNetBody();
            break;
        case ARTNETHEAD:
            receiveArtNetHead();
            break;
        default:
            break;
    }
}

void StormBreaker::receiveArtNetBody()
{
    ArtNetBody.pan = (SerialUSB.read() << 8) & SerialUSB.read();
    ArtNetBody.pan_control = SerialUSB.read();
    ArtNetBody.pan_tilt_speed = SerialUSB.read();
    ArtNetBody.power_special_functions = SerialUSB.read();

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
    ArtNetHead.strobe_shutter = SerialUSB.read();
    ArtNetHead.iris = SerialUSB.read();
    ArtNetHead.zoom = (SerialUSB.read() << 8) & SerialUSB.read();
    ArtNetHead.focus = (SerialUSB.read() << 8) & SerialUSB.read();
    ArtNetHead.tilt = (SerialUSB.read() << 8) & SerialUSB.read();
    ArtNetHead.tilt_control = SerialUSB.read();
    ArtNetHead.pan_tilt_speed = SerialUSB.read();
    ArtNetHead.power_special_functions = SerialUSB.read();

    #ifdef TESTING
        SerialUSB.print("ArtNetHead packet: ");
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
    // ODriveClass::SetPosition();
}

void StormBreaker::serviceArtNetHead()
{
    // ODriveClass::SetPosition();
}
