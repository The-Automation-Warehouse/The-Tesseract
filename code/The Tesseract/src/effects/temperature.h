/**
 * @file temperature.h
 * @brief Header file for the temperature visualization effect.
 * 
 * Lights up the tesseract in a color depending on the temperature from the ominitouch.
 */


#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "fillcube.h"
#include "omnitouch.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

extern OmnitouchData omnitouchData;

static float color = 0;

void temperature(CRGB pixels[NUM_LEDS])
{

    // Clear the cube
    fillCube(pixels, CHSV(0, 0, 0));

    // Clamp the temperature value to the range 40-50
    float temperature = constrain(omnitouchData.temperature, 35, 50);

    // Map the temperature value to the range 0-100 inclusive but make sure the color changes from red to green even though the temperature value is increasing
    float newColor = map(temperature, 40, 50, 0, 100);
    newColor = 100 - newColor;

    // Smoothly change the color by taking only 1% of the new color and 99% of the old color
    color = (color * 99 + newColor) / 100;

    // Fill the teseract with a color depending on the temperature value
    fillCube(pixels, CHSV(color, 255, 255));

    FastLED.show();

    readOmnitouchData();

    
}

#endif // MAGNET_H
