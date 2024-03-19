/**
 * @file magnet.h
 * @brief Header file for the magnet visalization effect.
 * 
 * Lights up the tesseract depending on the magnetic field strength from the ominitouch.
 */


#ifndef MAGNET_H
#define MAGNET_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "fillcube.h"
#include "omnitouch.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

extern OmnitouchData omnitouchData;

void magnet(CRGB pixels[NUM_LEDS])
{
    readOmnitouchData();
    if (omnitouchData.button == 1)
    {
        return;
    }
    
    FastLED.setBrightness(100);

    // Clear the cube
    fillCube(pixels, CHSV(0, 0, 0));

    CHSV color = CHSV(0, 255, 255);

    if (omnitouchData.magnet > 0) {
        color = CHSV(0, 255, 255);
    } else {
        color = CHSV(160, 255, 255);
    }

    // Clamp the magnet value to the range 0-1000 and run it through the absolute function
    int magnet = abs(omnitouchData.magnet);
    magnet = constrain(magnet, 0, 1000);

    // Map the magnet value to the range 0-9 inclusive
    int layer = map(magnet, 0, 1000, 0, 9);

    // Light up the layers of the tesseract depending on the magnet value using thes
    for (int i = 0; i < 10; i++)
    {
        if (i <= layer)
        {
            for (int j = 0; j < 100; j++)
            {
                pixels[i * 100 + j] = color;
            }
        }
    }

    FastLED.show();

    
}

#endif // MAGNET_H
