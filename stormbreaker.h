/* StormBreaker Header */

#ifndef STORMBREAKER_H
#define STORMBREAKER_H

/* Includes-------------------------------------------------------------*/
#include <stdint.h>

#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
class StormBreaker {
public:
    StormBreaker(ODriveClass& odrive) : odrive_(odrive) {}

    void serviceStormBreaker();

    enum MessageType_t {
        ERROR = -2,
        WARNING = -1,
        OK = 0,
        ARTNETBODY = 1,
        ARTNETHEAD = 2,
        IDENTIFY = 99
    };

    struct Header_t {
        MessageType_t type;
        uint8_t size; //in bytes
    } Header;

    struct ArtNetBody_t {
        uint16_t pan;
        uint8_t pan_control;
        uint8_t pan_tilt_speed;
        uint8_t power_special_functions;
    } ArtNetBody;

    struct ArtNetHead_t {
        uint8_t strobe_shutter;
        uint8_t iris;
        uint16_t zoom;
        uint16_t focus;
        uint16_t tilt;
        uint8_t tilt_control;
        uint8_t pan_tilt_speed;
        uint8_t power_special_functions;
    } ArtNetHead;

private:
    ODriveClass& odrive_;

    void receiveArtNetBody();
    void receiveArtNetHead();
    void serviceArtNetBody();
    void serviceArtNetHead();
    void serviceIdentify();
    void ArtNetPan();
    void ArtNetStrobeShutter();
    void ArtNetIris();
    void ArtNetZoom();
    void ArtNetFocus();
    void ArtNetTilt();
    void ArtNetPanTiltSpeed();
    void ArtNetPowerSpecialFunctions();
};

#endif //STORMBREAKER_H
