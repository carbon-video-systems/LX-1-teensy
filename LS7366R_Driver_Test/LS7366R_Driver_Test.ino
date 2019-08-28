#include <SPI.h>
#include "LS7366R.h"
#include "options.h"

/*Variables------------------------------------------------------------*/
#ifdef TESTING
    LS7366R counter(COUNTER_SELECT_PIN, counterBytes, true);
#else
    LS7366R counter(COUNTER_SELECT_PIN, counterBytes);
#endif

void setup() {
  // put your setup code here, to run once:

    #ifdef TESTING
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB);
    #endif

    LS7366R.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  LS7366R.counterRead();
  delay(500);

}
