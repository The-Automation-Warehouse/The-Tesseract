/**
 * @file fireworks.h
 * @brief Header file for the fireworks effect.
 *
 * Creates a fireworks effect that creates a random explosion of colors.
 */

#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <tesseractFunctions.h>
#include "Entropy.h"


#define NUM_LEDS 1000
#define NUM_LAYERS 10

void fireworks(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t deltaHue)
{
    FastLED.setBrightness(255);

    for (int i = 0; i < 100; i++)
    {
        for (int z = 0; z < NUM_LAYERS; z++) {
            for (int j = 0; j < NUM_LEDS / NUM_LAYERS; j++) {
                layers[z][j].fadeToBlackBy(30);
            }
        }
        drawVirtualLed(4.5, 4.5, 4.5, CHSV(Entropy.random(0, 255), 255, 255));
        FastLED.show();
    }

    FastLED.setBrightness(brightness);
    
    
    
}


#endif // FIREWORKS_H
