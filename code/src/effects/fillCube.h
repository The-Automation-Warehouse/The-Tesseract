/**
 * @file fillCube.h
 * @brief Header file for the fillCube effect.
 * 
 * Fill the entire cube with a single color.
 */


#ifndef FILL_CUBE_H
#define FILL_CUBE_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <OctoWS2811.h>


#define NUM_LEDS 1000
#define NUM_LAYERS 10


void fillCube(CRGB pixels[NUM_LEDS], CHSV color)
{
    fill_solid(pixels, NUM_LEDS, color);
}


#endif // FILL_CUBE_H