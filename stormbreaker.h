/*
 * StormBreaker Header
 *
 * @file    stormbreaker.h
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

#ifndef STORMBREAKER_H
#define STORMBREAKER_H

/* Includes-------------------------------------------------------------*/
#include <stdint.h>

#include "ODriveLib.h"
#include "LS7366R.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
#define TENSION_SCALING_FACTOR  4.2   // scaling factor between one motor revolution and one system revolution

/* Functions------------------------------------------------------------*/
class StormBreaker {
public:
    StormBreaker(ODriveClass& odrive, LS7366R& encoder) : odrive_(odrive), encoder_(encoder) {}

    enum MessageType_t {
        ERROR = -2,
        WARNING = -1,
        OK = 0,
        ARTNETBODY = 1,
        ARTNETHEAD = 2,
        IDENTIFY = 99
    };

    enum MessageSize_t{
        SIZE_IDENT = 0,
        SIZE_BODY = 5,
        SIZE_HEAD = 11
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

    struct SystemIndex_t {
        int32_t pan_index;
        int32_t tilt_index;
        bool encoder_direction;
    } SystemIndex;

    void serviceStormBreaker();

private:
    ODriveClass& odrive_;
    LS7366R& encoder_;

    // body functions
    void receiveArtNetBody();
    void serviceArtNetBody();
    void ArtNetPan();
    // head functions
    void receiveArtNetHead();
    void serviceArtNetHead();
    void ArtNetStrobeShutter();
    void ArtNetIris();
    void ArtNetZoom();
    void ArtNetFocus();
    void ArtNetTilt();
    // common functions
    void ArtNetPanTiltSpeed();
    void ArtNetPowerSpecialFunctions();
    void serviceIdentify();
};

#endif //STORMBREAKER_H
