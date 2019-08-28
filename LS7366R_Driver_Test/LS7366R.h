/*
 * LS7366R Driver Header
 *
 * @file    LS7366R.h
 * @author  Carbon Video Systems 2019
 * @description   Driver for communicating with the LS7366R Quadrature Counter.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Include Guard--------------------------------------------------------*/
#ifndef _LS7366R_h_
#define _LS7366R_h_

/* Includes-------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h> // Have to include this in the main sketch too... (Using SPI)

/*Constants------------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
class LS7366R
{
private:
    void load_rst_reg(unsigned char);
    void singleByteWrite(unsigned char, unsigned char);
    uint8_t singleByteRead(unsigned char);
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

    uint8_t debug;

    void begin(void);
    void setCS(int pin);
    int32_t counterRead();
    uint8_t stringRead();
};

#endif //LS7366R_H
