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

void planets(CRGB pixels[NUM_LEDS]);
void sun(CRGB pixels[NUM_LEDS], CRGB color);

#include "Entropy.h"

void planets(CRGB pixels[NUM_LEDS], uint16_t delayTime)
{
    
    FastLED.setBrightness(255);
    

    for (int i = 0; i < 360; i++)
    {
        
      fadeToBlackBy(pixels, NUM_LEDS, 12);
      sun(pixels, CRGB(255, 255, 0));
      float radians = i * 0.0174533;
      //map the first planets hue to a range of 96-160 and make it change slowly even at the beginning of another orbit
      drawVirtualLed(4.5 + 4 * cos(radians), 4.5 + 4 * sin(radians), 4.5 + 4 * cos(radians), CHSV(map(sin(i * 0.0174533) * 100, -100, 100, 64, 96), 255, 255));
      drawVirtualLed(4.5, 4.5 + 4 * cos(radians), 4.5 + 4 * sin(radians), CHSV(map(sin(i * 0.0174533) * 100, -100, 100, 96, 128), 128, 192));
      drawVirtualLed(4.5 + 4 * sin(radians + 3), 4.5 + 4 * cos(radians + 3), 4.5 + 4 * cos(radians + 3), CHSV(map(sin(i * 0.0174533) * 100, -100, 100, 128, 160), 255, 255));
      drawVirtualLed(4.5 + (-4) * cos(radians), 4.5 + (-4) * sin(radians), 4.5 + 4 * cos(radians), CHSV(map(sin(i * 0.0174533) * 100, -100, 100, 160, 192), 255, 255));
      FastLED.show();   
      delay(delayTime);
    }

    
} 


void sun(CRGB pixels[NUM_LEDS], CRGB color)
{

    //draw the sun at 2x2x2 pixels in the center
    setLed(4, 4, 4, color);
    setLed(4, 4, 5, color);
    setLed(4, 5, 4, color);
    setLed(4, 5, 5, color);
    setLed(5, 4, 4, color);
    setLed(5, 4, 5, color);
    setLed(5, 5, 4, color);
    setLed(5, 5, 5, color);

}

#endif // PLANETS_H
