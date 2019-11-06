/* Includes-------------------------------------------------------------*/
#include "RZ120.h"
#include "options.h"
#include "debug.h"

/* Constants --------------------------------------------------------------------------------------*/
#define TimingThreshold 10000

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/*Variables------------------------------------------------------------*/
elapsedMillis TestTiming;

RZ120Class rz120(rz120_serial);
#ifdef TESTING
    Debug debugger(rz120);
#endif

/* Functions------------------------------------------------------------*/
void setup() {

    // Status LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    rz120_serial.begin(RZ120_SERIAL_BAUD, SERIAL_8N1);
    while(!rz120_serial);

    TestTiming = 0;

    rz120.initialize();
}

void loop() {
    #ifdef TESTING
        if(SerialUSB.available())
            debugger.serviceDebug();
    #endif

    while (rz120_serial.available()){
        char buffer = rz120_serial.read();

        #ifdef TESTING
            SerialUSB.print("rec: ");
            SerialUSB.println(buffer);
        #endif
    }
}
