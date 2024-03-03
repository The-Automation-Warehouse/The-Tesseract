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


#define NUM_LEDS 1000
#define NUM_LAYERS 10


void fillCube(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], CHSV color)
{
    fill_solid(layers[0], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[1], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[2], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[3], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[4], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[5], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[6], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[7], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[8], NUM_LEDS / NUM_LAYERS, color);
    fill_solid(layers[9], NUM_LEDS / NUM_LAYERS, color);
}


#endif // FILL_CUBE_H