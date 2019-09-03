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
    void multiByteWrite(unsigned char op_code, uint32_t data, int bytes);
    uint8_t singleByteRead(unsigned char);
    void multiByteStoreRead(unsigned char op_code, int bytes);
    uint8_t statusRead();

protected:
    union { // Union makes conversion from 4 bytes to an unsigned 32-bit int easy
    uint8_t bytes[4];
    uint32_t uint32;
    } count;

    uint8_t cs;
    uint8_t resolution;

public:
    // Class Constructor and Destructor
    LS7366R(const uint8_t, const uint8_t _resolution = 3, const bool _debug=0);
    ~LS7366R(); // User responsible for reassigning pins & stopping SPI

    uint8_t debug;

    void begin(void);
    void setCS(int pin);
    int32_t counterRead();
};

#endif //LS7366R_H
