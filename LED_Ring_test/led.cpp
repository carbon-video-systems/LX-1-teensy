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
const int ledsStrip = 15;
const int config = WS2811_GRB | WS2811_800kHz;

#define RAINBOW_LIGHTNESS   20;
#define RAINBOW_SATURATION  100;

/* Variables  ----------------------------------------------------------*/
DMAMEM int displayMemory[ledsStrip*6];
int drawingMemory[ledsStrip*6];

OctoWS2811 leds(ledsStrip, displayMemory, drawingMemory, config);

int rainbowColors[ledsStrip];
int rainbowColors2[ledsStrip * 2];
int rainbowColors6[ledsStrip * 6];

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
  for (int i= (4 * ledsStrip - 1); i < (5 * ledsStrip - 1); i++){
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
    for (uint32_t i = (4 * ledsStrip - 1); i < (5 * ledsStrip - 1); i++){
        leds.setPixel(i, red, green, blue);
    }
    leds.show();
}

void generateRainbow(void){
    //  hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
    //                            120=yellow, 180=green, 240=blue, 300=violet
    //  saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
    //  lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
    for (int i=0; i < ledsStrip; i++) {
        int hue = i * 360 / ledsStrip;
        int saturation = RAINBOW_SATURATION;
        int lightness = RAINBOW_LIGHTNESS;
        // pre-compute the 180 rainbow colors
        rainbowColors[i] = makeColor(hue, saturation, lightness);
  }
}

void generateRainbow2(void){
    //  hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
    //                            120=yellow, 180=green, 240=blue, 300=violet
    //  saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
    //  lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
    for (int i=0; i < (2 * ledsStrip); i++) {
        int hue = i * 180 / ledsStrip;
        int saturation = RAINBOW_SATURATION;
        int lightness = RAINBOW_LIGHTNESS;
        // pre-compute the 180 rainbow colors
        rainbowColors2[i] = makeColor(hue, saturation, lightness);
  }
}

void generateRainbow6(void){
    //  hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
    //                            120=yellow, 180=green, 240=blue, 300=violet
    //  saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
    //  lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
    for (int i=0; i < (6 * ledsStrip); i++) {
        int hue = i * 60 / ledsStrip;
        int saturation = RAINBOW_SATURATION;
        int lightness = RAINBOW_LIGHTNESS;
        // pre-compute the 180 rainbow colors
        rainbowColors6[i] = makeColor(hue, saturation, lightness);
  }
}

void rainbow(int delay_ms)
{
    static int shift = 0;
    int displacement = ledsStrip - shift;

    for (int i = 0; i < ledsStrip; i++){
        if (i < displacement)
            leds.setPixel(4 * ledsStrip + i, rainbowColors[i + shift]);
        else
            leds.setPixel(4 * ledsStrip + i, rainbowColors[i - displacement]);
    }
    leds.show();

    if (shift >= ledsStrip - 1)
        shift = 0;
    else
        shift++;

    delay(delay_ms);
}

void rainbow2(int delay_ms)
{
    static int shift = 0;
    int displacement = (2 * ledsStrip) - shift;

    for (int i = 0; i < ledsStrip; i++){
        if (i <= (displacement / 2))
            leds.setPixel(4 * ledsStrip + i, rainbowColors2[2 * i + shift]);
        else
            leds.setPixel(4 * ledsStrip + i, rainbowColors2[2 * i - displacement]);
    }
    leds.show();

    if (shift >= (2 * ledsStrip - 1))
        shift = 0;
    else
        shift++;

    delay(delay_ms);
}

void rainbow6(int delay_ms)
{
    static int shift = 0;
    int displacement = (6 * ledsStrip) - shift;

    for (int i = 0; i < ledsStrip; i++){
        if (i <= (displacement / 6))
            leds.setPixel(4 * ledsStrip + i, rainbowColors6[6 * i + shift]);
        else
            leds.setPixel(4 * ledsStrip + i, rainbowColors6[6 * i - displacement]);
    }
    leds.show();

    if (shift >= (6 * ledsStrip - 1))
        shift = 0;
    else
        shift++;

    delay(delay_ms);
}

// Converts HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
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


unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
	if (hue < 180) return v2 * 60;
	if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);

	return v1 * 60;
}
