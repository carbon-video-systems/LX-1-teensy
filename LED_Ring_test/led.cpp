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
#define RAINBOW_RESOLUTION  6

#define RAINBOW_LIGHTNESS   2
#define RAINBOW_SATURATION  100

const int ledsStrip = 60;
const int config = WS2811_GRB | WS2811_800kHz;
const int rainbow_bound = ledsStrip * RAINBOW_RESOLUTION;

/* Variables  ----------------------------------------------------------*/
DMAMEM int displayMemory[ledsStrip*6];
int drawingMemory[ledsStrip*6];

OctoWS2811 leds(ledsStrip, displayMemory, drawingMemory, config);

int rainbowColors[rainbow_bound];

/* Functions------------------------------------------------------------*/
/**
  * @brief  Initializes the led ring without turning on the LEDS
  * @param  void
  * @return void
  */
void led_begin(void)
{
    leds.begin();
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

/**
  * @brief  Sets all pixels (8 channels) to the received colour
  * @param  int color - RGB value to set the pixels to
  * @return void
  */
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
    // for (int i= (4 * ledsStrip - 1); i < (5 * ledsStrip - 1); i++){
    for (int i= 0; i < ledsStrip; i++){
        leds.setPixel(i, colour);
    }
    leds.show();
}

/**
  * @brief  Updates all LEDs with the same colour
  * @param  int colour - the RGB value to set the LED to
  * @param  int delay_millis - delay after setting the LEDS
  * @return void
  */
void setAllColour(int colour, int delay_millis)
{
  // for (int i= (4 * ledsStrip - 1); i < (5 * ledsStrip - 1); i++){
    for (int i= 0; i < ledsStrip; i++){
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
void ArtNetLEDUpdate(uint8_t red, uint8_t green, uint8_t blue){
    // for (uint32_t i = (4 * ledsStrip - 1); i < (5 * ledsStrip - 1); i++){
    for (int i = 0; i < ledsStrip; i++){
        leds.setPixel(i, red, green, blue);
    }
    leds.show();
}

/**
  * @brief  Generates the RGB values of a rainbow of resolution ledsStrip * RAINBOW_RESOLUTION
  * @param  void
  * @return void
  */
void generateRainbow(void){
    //  hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
    //                            120=yellow, 180=green, 240=blue, 300=violet
    //  saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
    //  lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
    for (int i=0; i < rainbow_bound; i++) {
        int hue = i * 360 / rainbow_bound;
        int saturation = RAINBOW_SATURATION;
        int lightness = RAINBOW_LIGHTNESS;
        // pre-compute the rainbow colors
        rainbowColors[i] = makeColor(hue, saturation, lightness);
  }
}

/**
  * @brief  Updates the LEDs to the next colours in the rainbow ring
  * @param  int delay_ms - delay in milliseconds after executing function
  * @return void
  */
void rainbow(int delay_ms)
{
    static int shift = 0;

    for (int i = 0; i < ledsStrip; i++){
        // leds.setPixel(4 * ledsStrip + i, rainbowColors[(RAINBOW_RESOLUTION * i + shift) % rainbow_bound]);
        leds.setPixel(i, rainbowColors[(RAINBOW_RESOLUTION * i + shift) % rainbow_bound]);
    }
    leds.show();

    if (shift >= rainbow_bound - 1)
        shift = 0;
    else
        shift++;

    delay(delay_ms);
}

/**
  * @brief  Converts HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
  * @param  unsigned int hue - HSL hue value
  * @param  unsigned int saturation - HSL saturation value
  * @param  unsigned int lightness - HSL lightness value
  * @return int - converted RGB value
  */
int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
	unsigned int red, green, blue;
	unsigned int var1, var2;

	if (hue > 359) hue = hue % 360;
    // if (hue > 359) hue = 359;
	if (saturation > 100) saturation = 100;
	if (lightness > 100) lightness = 100;

	// algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
	if (saturation == 0) {
		red = green = blue = lightness * 255 / 100;
	} else {
		if (lightness < 50) {
			var2 = lightness * (100 + saturation);
		} else {
			var2 = ((lightness + saturation) * 100) - (saturation * lightness);
		}
		var1 = lightness * 200 - var2;
		red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
		green = h2rgb(var1, var2, hue) * 255 / 600000;
		blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
	}
	return (red << 16) | (green << 8) | blue;
}

/**
  * @brief  Hue to RGB conversion table
  * @param  unsigned int v1 - HSL var 1 conversion
  * @param  unsigned int v2 - HSL var 2 conversion
  * @param  unsigned int hue - HSL hue conversion
  * @return unsigned int - converted R, G, or B value
  */
unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
	if (hue < 180) return v2 * 60;
	if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);

	return v1 * 60;
}
