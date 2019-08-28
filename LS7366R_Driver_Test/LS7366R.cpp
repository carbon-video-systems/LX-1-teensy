/*
 * LS7366R Driver Source
 *
 * @file    LS7366R.cpp
 * @author  Carbon Video Systems 2019
 * @description   Driver for communicating with the LS7366R Quadrature Counter.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include "LS7366R.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/
/* MDR0 configuration data */
//Count modes
#define NQUAD 0x00 //non-quadrature mode
#define QUADRX1 0x01    //X1 quadrature mode
#define QUADRX2 0x02    //X2 quadrature mode
#define QUADRX4 0x03    //X4 quadrature mode
//Running modes
#define FREE_RUN 0x00
#define SINGE_CYCLE 0x04
#define RANGE_LIMIT 0x08
#define MODULO_N 0x0C
//Index modes
#define DISABLE_INDX 0x00   //index_disabled
#define INDX_LOADC 0x10     //index_load_CNTR
#define INDX_RESETC 0x20    //index_rest_CNTR
#define INDX_LOADO 0x30     //index_load_OL
#define ASYNCH_INDX 0x00    //asynchronous index
#define SYNCH_INDX 0x40     //synchronous index
//Clock filter modes
#define FILTER_1 0x00   //filter clock frequency division factor 1
#define FILTER_2 0x80   //filter clock frequency division factor 2

/* MDR1 configuration data */
//Flag modes
#define NO_FLAGS 0x00   //all flags disabled
#define IDX_FLAG 0x10   //IDX flag
#define CMP_FLAG 0x20   //CMP flag
#define BW_FLAG 0x40    //BW flag
#define CY_FLAG 0x80    //CY flag
//1 to 4 bytes data-width
#define BYTE_4 0x00     //four byte mode
#define BYTE_3 0x01     //three byte mode
#define BYTE_2 0x02     //two byte mode
#define BYTE_1 0x03     //one byte mode
//Enable/disable counter
#define EN_CNTR 0x00    //counting enabled
#define DIS_CNTR 0x04   //counting disabled

/* LS7366R op-code list */
#define CLR_MDR0 0x08
#define CLR_MDR1 0x10
#define CLR_CNTR 0x20
#define CLR_STR 0x30
#define READ_MDR0 0x48
#define READ_MDR1 0x50
#define READ_CNTR 0x60
#define READ_OTR 0x68
#define READ_STR 0x70
#define WRITE_MDR1 0x90
#define WRITE_MDR0 0x88
#define WRITE_DTR 0x98
#define LOAD_CNTR 0xE0
#define LOAD_OTR 0xE4

// SPISettings settingsA(4000000, MSBFIRST, SPI_MODE0);

/* Functions------------------------------------------------------------*/
/**
  * @brief  Class Constructor - sets up the lS7366R class.  Utilizes the stock
  *     SPI library.  Only chip select needs to be chosen.
  * @param  const uint8_t _cs - the pin to assign SS to
  * @param  const uint8_t _resolution - sets resolution of the counter in bytes
  * @param  const bool _debug - flag that sets debugging outputs
  * @return void
  */
LS7366R::LS7366R(const uint8_t _cs, const uint8_t _resolution, const bool _debug) : cs(_cs), resolution(_resolution), debug(_debug)
{
  pinMode(cs, OUTPUT);

  SPI_counter.begin();
  digitalWrite(cs, HIGH);

}

/**
  * @brief  Changes the Slave Select pin assignment (chip select)
  * @param  int pin - the pin to assign SS to
  * @return void
  */
void LS7366R::setCS(int pin)
{
    digitalWrite(cs, HIGH); // Make sure to set old cs high to disable the chip
    cs = pin;               //change to new cs pin
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
}

/**
  * @brief  Class Destructor - currently does nothing.
  * @param  void
  * @return void
  */
LS7366R::~LS7366R(){
    return;
}

/**
  * @brief  LS7366R setup function - clears the counter and sets
  *     the MDR0 and MDR1 registers.
  * @param  void
  * @return void
  */
void LS7366R::begin()
{
    load_rst_reg(CLR_CNTR);

    // Quadrature mode x4, free running count, index resets the counter, asynchronous, filter x1
    singleByteWrite(WRITE_MDR0, QUADRX4|FREE_RUN|INDX_RESETC|SYNCH_INDX|FILTER_1);
    // Index flag output, counter enabled, 3 byte operation
    singleByteWrite(WRITE_MDR1, IDX_FLAG|EN_CNTR|BYTE_3);

    if (debug){
        uint8_t MDR0 = singleByteRead(READ_MDR0);
        SerialUSB.print("MDR0: ");
        SerialUSB.println(MDR0, BIN);
        uint8_t MDR1 = singleByteRead(READ_MDR1);
        SerialUSB.print("MDR1: ");
        SerialUSB.println(MDR1, BIN);
    }
}

void LS7366R::load_rst_reg(unsigned char op_code) //dataless write command
{
    // SPI_counter.beginTransaction(settingsA);
    digitalWrite(cs, LOW);
    SPI_counter.transfer(op_code);
    digitalWrite(cs, HIGH);
    // SPI_counter.endTransaction();
}

void LS7366R::singleByteWrite(unsigned char op_code, unsigned char data) //single byte write command
{
    // SPI_counter.beginTransaction(settingsA);
    digitalWrite(cs, LOW);
    SPI_counter.transfer(op_code);
    SPI_counter.transfer(data);
    /*additional bytes can be sent here for multibyte write, e.g., write_DTR*/
    digitalWrite(cs, HIGH);
    // SPI_counter.endTransaction();
}

uint8_t LS7366R::singleByteRead(unsigned char op_code) //single byte read command
{
    uint8_t spi_data;
    // SPI_counter.beginTransaction(settingsA);
    digitalWrite(cs, LOW);
    SPI_counter.transfer(op_code);
    spi_data = SPI_counter.transfer(0);
    /*additional bytes can be received here for multibyte read, e.g., read_OTR*/
    digitalWrite(cs, HIGH);
    // SPI_counter.endTransaction();

    return spi_data;
}

void LS7366R::multiByteStoreRead(unsigned char op_code, int bytes)
{
    digitalWrite(cs, LOW);
    SPI_counter.transfer(op_code);
    for(int8_t i = 0; i < bytes; i++){
        data.bytes[i] = SPI_counter.transfer(0);
    }
    digitalWrite(cs, HIGH);
    return;
}

int32_t LS7366R::counterRead()
{
    const uint8_t sign_bit = 1;
    int32_t counter_value;

    multiByteStoreRead(READ_CNTR, counterBytes);
    uint8_t STR = singleByteRead(READ_STR);

    if ((STR & sign_bit) == sign_bit){
        counter_value = (-1) * (int32_t)data.uint32; // BYTE READ
    }
    else{
        counter_value = (int32_t)data.uint32;
    }

    if (debug){
        SerialUSB.print("STR: ");
        SerialUSB.println(STR, BIN);
        SerialUSB.print("Counter: ");
        SerialUSB.println(counter_value);
        SerialUSB.println();
    }

    return counter_value;
}

uint8_t LS7366R::stringRead()
{
    uint8_t STR = singleByteRead(READ_STR);

    if(debug){
        SerialUSB.print("STR: ");
        SerialUSB.println(STR, BIN);
        SerialUSB.println();
    }

    return STR;
}
