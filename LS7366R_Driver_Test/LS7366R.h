#ifndef _LS7366R_h_
#define _LS7366R_h_

#include <Arduino.h>
#include <SPI.h> // Have to include this in the main sketch too... (Using SPI)

/*Constants------------------------------------------------------------*/

class LS7366R
{
private:
    void load_rst_reg(unsigned char);
    void singleByteWrite(unsigned char, unsigned char);
    unsigned char singleByteRead(unsigned char);
    void multiByteStoreRead(unsigned char op_code, int bytes);


protected:
    union { // Union makes conversion from 4 bytes to an unsigned 32-bit int easy
    uint8_t bytes[4];
    uint32_t uint32;
    } data;

    uint8_t cs;
    uint8_t resolution;

public:
    // Class Constructor and Destructor
    LS7366R(const uint8_t, const uint8_t _resolution = 3, const bool _debug=0);
    ~LS7366R(); // User responsible 4 reassigning pins & stopping SPI

    // If non-zero will turn on serial debugging messages
    uint8_t debug;

    // Pass a pin number to set as CS
    void setCS(int pin);

    int32_t counterRead();

};

#endif
