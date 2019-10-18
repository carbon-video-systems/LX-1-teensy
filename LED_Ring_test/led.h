/*
 * LED Header
 *
 * @file    led.h
 * @author  Carbon Video Systems 2019
 * @description   LED Ring Driver.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

/* Constants -----------------------------------------------------------*/
// #define RED    0xFF0000
// #define GREEN  0x00FF00
// #define BLUE   0x0000FF
// #define YELLOW 0xFFFF00
// #define PINK   0xFF1088
// #define ORANGE 0xE05800
// #define WHITE  0xFFFFFF

// Less intense...

#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

/* Includes-------------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
void led_begin(bool);
void led_begin(void);

void setAllColour(int);
void setAllColour(int, int);

void ArtNetUpdate(uint8_t red, uint8_t green, uint8_t blue);
void colorWipe(int);

void generateRainbow(void);
void generateRainbow2(void);
void generateRainbow6(void);
void rainbow(int delay_ms);
void rainbow2(int delay_ms);
void rainbow6(int delay_ms);
int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);
unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);

#endif //LED_H
