/* Includes-------------------------------------------------------------*/
#include <SPI.h>
#include "LS7366R.h"
#include "options.h"

/*Variables------------------------------------------------------------*/
#ifdef TESTING
    LS7366R encoder(COUNTER_SELECT_PIN, counterBytes, true);
#else
    LS7366R encoder(COUNTER_SELECT_PIN, counterBytes);
#endif

/* Functions------------------------------------------------------------*/
void setup() {
  // put your setup code here, to run once:

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    encoder.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  encoder.counterRead();
  delay(500);

}
