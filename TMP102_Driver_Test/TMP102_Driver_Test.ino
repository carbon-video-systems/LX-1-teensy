/* Includes-------------------------------------------------------------*/
#include <i2c_t3.h>
#include "fan.h"
#include "options.h"

/*Variables------------------------------------------------------------*/
#define temperatureTimingThreshold  1500

elapsedMillis temperatureCheckTiming;

/* Functions------------------------------------------------------------*/
void setup() {

    // Status LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    initFans();

    temperatureCheckTiming = 0;
}

void loop() {
    // put your main code here, to run repeatedly:
    static bool fan_select = true;

    if (temperatureCheckTiming >= temperatureTimingThreshold){
        if (fan_select)
            runFan1();
        else
            runFan2();

        fan_select = (!fan_select);
        temperatureCheckTiming = 0;
    }
}
