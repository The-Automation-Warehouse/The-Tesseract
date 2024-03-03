/**
 * @file vortex.h
 * @brief Header file for the vortex effect.
 * 
 * Creates a vortex effect that creates a spinning rainbow.
 */


#ifndef VORTEX_H
#define VORTEX_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>


#define NUM_LEDS 1000
#define NUM_LAYERS 10

void vortex(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t deltaHue, uint8_t density)
{

    
}

#endif // VORTEX_H
