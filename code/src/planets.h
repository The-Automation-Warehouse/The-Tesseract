/**
 * @file planets.h
 * @brief Header file for the planets effect.
 * 
 * Creates an sun in the center of the cube and planets orbiting around it.
 */


#ifndef PLANETS_H
#define PLANETS_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "Entropy.h"


#define NUM_LEDS 1000
#define NUM_LAYERS 10

extern int brightness;
void planets(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS]);
void sun(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t hue);

#include "Entropy.h"

void planets(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS])
{
    
    FastLED.setBrightness(255);
    

    for (int i = 0; i < 45; i++)
    {
      for (int z = 0; z < NUM_LAYERS; z++) {
        for (int j = 0; j < NUM_LEDS / NUM_LAYERS; j++) {
            layers[z][j].fadeToBlackBy(30);
        }
      }
      sun(layers, (Entropy.random(20, 50)));
      float radians = i * 8 * 0.0174533;
      drawVirtualLed(4.5 + 4 * cos(radians), 4.5 + 4 * sin(radians), 4.5 + 4 * cos(radians), CHSV(i, 255, 255));
      drawVirtualLed(4.5, 4.5 + 4 * cos(radians), 4.5 + 4 * sin(radians), CHSV(i + 80, 255, 255));
      drawVirtualLed(4.5 + 4 * sin(radians), 4.5 + 4 * cos(radians), 4.5 + 4 * cos(radians), CHSV(i + 80 * 2, 255, 255)); 
      FastLED.show();   
    }

    FastLED.setBrightness(brightness);
    
} 


void sun(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t hue)
{

    //draw the sun at 2x2x2 pixels in the center
    setLed(4, 4, 4, CHSV(hue, 255, 255));
    setLed(4, 4, 5, CHSV(hue, 255, 255));
    setLed(4, 5, 4, CHSV(hue, 255, 255));
    setLed(4, 5, 5, CHSV(hue, 255, 255));
    setLed(5, 4, 4, CHSV(hue, 255, 255));
    setLed(5, 4, 5, CHSV(hue, 255, 255));
    setLed(5, 5, 4, CHSV(hue, 255, 255));
    setLed(5, 5, 5, CHSV(hue, 255, 255));
}

#endif // PLANETS_H
