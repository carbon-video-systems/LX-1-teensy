/*
 * LED Source
 *
 * @file    led.cpp
 * @author  Carbon Video Systems 2019
 * @description   LED Ring Driver.
 *
 * @section LICENSE
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted in accordance with the BSD 3-Clause License.
 *
 * Distributed as-is; in accordance with the BSD 3-Clause License.
 */

/* Includes-------------------------------------------------------------*/
#include "led.h"
#include "WS2811.h"

/* Constants -----------------------------------------------------------*/
const int ledsPerStrip = 15;
const int config = WS2811_GRB | WS2811_800kHz;

/* Variables  ----------------------------------------------------------*/
DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

/* Functions------------------------------------------------------------*/
/**
  * @brief  Initializes the led ring without turning on the LEDS
  * @param  void
  * @return void
  */
void led_begin(void)
{
    led_begin(false);
}

/**
  * @brief  Initializes the led ring
  * @param  bool startup - flag that turns on the LEDS on startup
  * @return void
  */
void led_begin(bool startup)
{
    leds.begin();

    if(startup)
        leds.show();
}

void colorWipe(int color)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
  }
  leds.show();
}

/**
  * @brief  Updates all LEDs with the same colour and no delay
  * @param  int colour - the RGB value to set the LED to
  * @return void
  */
void setAllColour(int colour){
    setAllColour(colour, 0);
}

/**
  * @brief  Updates all LEDs with the same colour
  * @param  int colour - the RGB value to set the LED to
  * @param  int delay_millis - delay after setting the LEDS
  * @return void
  */
void setAllColour(int colour, int delay_millis)
{
  for (int i= (4 * ledsPerStrip - 1); i < (5 * ledsPerStrip - 1); i++){
    leds.setPixel(i, colour);
  }
  leds.show();
  delay(delay_millis);
}

/**
  * @brief  Updates all LEDs with discrete RGB values
  * @param  uint8_t red - setting for the red channel
  * @param  uint8_t green - setting for the green channel
  * @param  uint8_t blue - setting for the blue channel
  * @return void
  */
void ArtNetUpdate(uint8_t red, uint8_t green, uint8_t blue){
    for (uint32_t i = (4 * ledsPerStrip - 1); i < (5 * ledsPerStrip - 1); i++){
        leds.setPixel(i, red, green, blue);
    }
    leds.show();
}
