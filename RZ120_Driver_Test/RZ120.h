
#ifndef RZ120_H
#define RZ120_H

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>
#include "options.h"

/* Functions------------------------------------------------------------*/
class RZ120Class {
public:
    RZ120Class(Stream& serial);

    // Commands
    void PowerOn(void);
    void PowerOn1(void);
    void PowerOn2(void);
    void PowerOff(void);
    void PowerOff1(void);
    void PowerOff2(void);
    void ZoomIn(void);

private:
    String readString();
    Stream& serial_;
};

#endif //RZ120_H
