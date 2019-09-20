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

    // YOU CANNOT USE THE BUILT IN LED > SPI SCK0 on pin 13!
    // pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, HIGH);

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    encoder.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t count = encoder.counterRead();
  SerialUSB.print("Counter Return:  ");
  SerialUSB.println(count);

  delay(10);

}
