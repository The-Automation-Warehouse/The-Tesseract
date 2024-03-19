/**
 * @file sparkles.h
 * @brief Header file for the sparkles effect
 *
 * Creates a simple random sparkling effect.
 */

#ifndef SPARKLES_H
#define SPARKLES_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <tesseractFunctions.h>
#include "Entropy.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

void sparkles(CRGB layers[NUM_LEDS], uint8_t fade, bool color, uint8_t deltaHue)
{
    FastLED.setBrightness(255);

    int posX = Entropy.random(0, 10);
    int posY = Entropy.random(0, 10);
    int posZ = Entropy.random(0, 10);
    int hue = Entropy.random(0, 255);

    fadeToBlackBy(pixels, NUM_LEDS, fade);

    //covert the hue into a CRGB color for the setLed function
    CRGB RGBcolor = CHSV(hue, 255, 255);

    if (color)
    {
        setLed(posX, posY, posZ, RGBcolor);
        FastLED.show();
    } else {
        setLed(posX, posY, posZ, CRGB::White);
        FastLED.show();
    }
    
}


#endif // SPARKLES_H
