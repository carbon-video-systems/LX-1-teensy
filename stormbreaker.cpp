#include "stormbreaker.h"
#include "options.h"

void StormBreaker::servicePiMessage()
{
    StormBreaker::Header_t header;

    header.type = pi_serial.read();
    header.size = pi_serial.read();

    switch(header.type){
        case ARTNETBODY:
            receiveArtNetBody();
        case ARTNETHEAD:

        default:
    }
}

void StormBreaker::receiveArtNetBody()
{
    ArtNetBody.pan = pi_serial.read();
    ArtNetBody.pan_control = pi_serial.read();
}