/**
 * @file vortex.h
 * @brief Header file for the vortex effect.
 * 
 * Creates a vortex effect that creates spirals orbiting around the vertical center of the cube.
 */


#ifndef VORTEX_H
#define VORTEX_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "Entropy.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10
static float vortexHue = 0;

void vortex(CRGB pixels[NUM_LEDS], uint8_t delayTime)
{

    for (int i = 0; i < 360; i++)
    {
        
        fadeToBlackBy(pixels, NUM_LEDS, 5);

        //if a pixels brightness is below 20, set it to 0
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (pixels[i].getAverageLight() < 20)
            {
                pixels[i] = CRGB::Black;
            }
        }

        for (int j = 0; j < 10; j++)
        {
            // Calculate the x and y position of the pixel based on the angle and distance from the center
            float radians = i * 0.0174533;
            float xPos = 4.5 + 4 * cos(radians + 2*PI/9*j);
            float yPos = 4.5 + 4 * sin(radians + 2*PI/9*j);
            float zPos = j;

            // Set the pixel at the calculated position to the hue value
            drawVirtualLed(xPos, yPos, zPos, CHSV(vortexHue - j * 2, 255, 255));
        }

        for (int j = 0; j < 10; j++)
        {
            // Calculate the x and y position of the pixel based on the angle and distance from the center
            float radians = i * 0.0174533;
            float xPos = 4.5 - 4 * cos(radians + 2*PI/9*j);
            float yPos = 4.5 - 4 * sin(radians + 2*PI/9*j);
            float zPos = j;

            // Set the pixel at the calculated position to the hue value
            drawVirtualLed(xPos, yPos, zPos, CHSV(vortexHue + j, 255, 255));

        }
        vortexHue += 0.2;
        if (vortexHue > 255)
        {
            vortexHue = 0;
        }

        FastLED.show();
        delay(delayTime);
    }

    
}

#endif // VORTEX_H
