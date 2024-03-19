/**
 * @file dataMatrix.c
 * @brief Header file for the dataMatrix effect
 * 
 * Creates a matrix effect that moves green pixels from back to front
 */


#ifndef DATA_MATRIX_H
#define DATA_MATRIX_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "Entropy.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

void dataMatrix(CRGB pixels[NUM_LEDS])
{
    
    
    
}

#endif // DATA_MATRIX_H
