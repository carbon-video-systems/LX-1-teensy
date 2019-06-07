/* StormBreaker Header */
#ifndef STORMBREAKER_H
#define STORMBREAKER_H

#include <stdint.h>

class StormBreaker {
public:
    enum MessageType_t {
        ERROR = -2,
        WARNING = -1,
        OK = 0,
        ARTNETBODY = 1,
        ARTNETHEAD = 2
    };

    struct Header_t {
        MessageType_t type;
        uint8_t size; //in bytes
    };

    void servicePiMessage();
    void receiveArtNetBody();

private:
    struct ArtNetBody_t {
        uint16_t pan;
        uint8_t pan_control;
        uint8_t pan_tilt_speed;
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
};

#endif //STORMBREAKER_H