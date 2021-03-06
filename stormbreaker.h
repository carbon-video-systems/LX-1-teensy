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
#include "options.h"

/* Constants -----------------------------------------------------------*/
#define TENSION_SCALING_FACTOR  5   // scaling factor between one motor revolution and one system revolution
#define REINDEX_FACTOR          3   // TENSION_SCALING_FACTOR / 2 > rounded up

/* Functions------------------------------------------------------------*/
class StormBreaker {
public:
    StormBreaker(ODriveClass& odrive) : odrive_(odrive){}

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
        // SIZE_HEAD = 11
        SIZE_HEAD = 14
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
        uint8_t led_ring_red;
        uint8_t led_ring_green;
        uint8_t led_ring_blue;
    } ArtNetHead;

    struct SystemIndex_t {
        float pan_index;
        float tilt_index;
        int start_index;
        bool encoder_direction;
    } SystemIndex;

    void serviceStormBreaker();

private:
    ODriveClass& odrive_;

    // body functions
    void receiveArtNetBody();
    void serviceArtNetBody();
    void ArtNetPan();
    void pan_reindex();
    // head functions
    void receiveArtNetHead();
    void serviceArtNetHead();
    void ArtNetStrobeShutter();
    void ArtNetIris();
    void ArtNetZoom();
    void ArtNetFocus();
    void ArtNetLEDRing();
    void ArtNetTilt();
    void tilt_reindex();
    // common functions
    void ArtNetPanTiltSpeed();
    void ArtNetPowerSpecialFunctions();
    void serviceIdentify();
};

#endif //STORMBREAKER_H
