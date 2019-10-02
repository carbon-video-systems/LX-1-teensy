/* Includes-------------------------------------------------------------*/
#include <i2c_t3.h>
#include "fan.h"
#include "options.h"

/*Variables------------------------------------------------------------*/
#define temperatureTimingThreshold  2000

elapsedMillis temperatureCheckTiming;

/* Functions------------------------------------------------------------*/
void setup() {

    // YOU CANNOT USE THE BUILT IN LED > SPI SCK0 on pin 13!
    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, HIGH);

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    initFans();

    temperatureCheckTiming = 0;
}

void loop() {
    // put your main code here, to run repeatedly:

    if (temperatureCheckTiming >= temperatureTimingThreshold){
        runFan1();
        runFan2();
        temperatureCheckTiming = 0;
    }
}
