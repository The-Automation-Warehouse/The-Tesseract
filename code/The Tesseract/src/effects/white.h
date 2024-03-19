/**
 * @file white.h
 * @brief Header file for the white effect.
 * 
 * Turns the entire cube white at full brightness.
 * 
 * Not included in the effects.h file.
 * 
 * ONLY USE THIS EFFECT FOR TESTING PURPOSES.
 */


#ifndef WHITE_H
#define WHITE_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <TimeLib.h>
#include "tesseractFunctions.h"
#include "effects.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

void white(CRGB pixels[NUM_LEDS])
{
    readOmnitouchData();
    if (omnitouchData.button == 1)
    {
        return;
    }

    FastLED.setBrightness(255);
    fillCube(pixels, CHSV(0, 0, 255));
    FastLED.show();
}


#endif // VORTEX_H
