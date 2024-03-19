/**
 * @file tesseract.h
 * @brief Header file for the tesseract effect.
 * 
 * Creates a tesseract that shriks and enventually disappears.
 */


#ifndef TESSERACT_H
#define TESSERACT_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "fillCube.h"


#define NUM_LEDS 1000
#define NUM_LAYERS 10
static int pos = 0;

void tesseract(CRGB pixels[NUM_LEDS], uint16_t speed)
{
    readOmnitouchData();
    if (omnitouchData.button == 1)
    {
        return;
    }

    EVERY_N_MILLISECONDS(speed) {
        //clear all layers
        fillCube(pixels, CHSV(0, 0, 0));

        //draw the outer edges of the tesseract
        for (int i = 0; i < NUM_LAYERS; i++)
        {
            setLed(i, 0, 0, CRGB::Blue);
            setLed(9, i, 0, CRGB::Blue);
            setLed(i, 9, 0, CRGB::Blue);
            setLed(0, i, 0, CRGB::Blue);

            setLed(0, 0, i, CRGB::Blue);
            setLed(9, 0, i, CRGB::Blue);
            setLed(0, 9, i, CRGB::Blue);
            setLed(9, 9, i, CRGB::Blue);

            setLed(i, 9, 9, CRGB::Blue);
            setLed(9, i, 9, CRGB::Blue);
            setLed(0, i, 9, CRGB::Blue);
            setLed(i, 0, 9, CRGB::Blue);
        }

        //draw the inner edges of the tesseract
        for (int i = pos; i < NUM_LAYERS - pos; i++)
        {
            setLed(i, pos, pos, CRGB::Blue);
            setLed(9 - pos, i, pos, CRGB::Blue);
            setLed(i, 9 - pos, pos, CRGB::Blue);
            setLed(pos, i, pos, CRGB::Blue);

            setLed(pos, pos, i, CRGB::Blue);
            setLed(9 - pos, pos, i, CRGB::Blue);
            setLed(pos, 9 - pos, i, CRGB::Blue);
            setLed(9 - pos, 9 - pos, i, CRGB::Blue);

            setLed(i, 9 - pos, 9 - pos, CRGB::Blue);
            setLed(9 - pos, i, 9 - pos, CRGB::Blue);
            setLed(pos, i, 9 - pos, CRGB::Blue);
            setLed(i, pos, 9 - pos, CRGB::Blue);
        }

        //draw the connecting lines
        for (int i = 0; i < pos; i++)
        {
            setLed(i, i, i, CRGB::Blue);
            setLed(9 - i, i, i, CRGB::Blue);
            setLed(i, 9 - i, i, CRGB::Blue);
            setLed(i, i, 9 - i, CRGB::Blue);

            setLed(i, 9 - i, 9 - i, CRGB::Blue);
            setLed(9 - i, i, 9 - i, CRGB::Blue);
            setLed(9 - i, 9 - i, i, CRGB::Blue);
        }

        FastLED.show();

        pos++;
        if (pos > 4)
        {
            pos = 0;
        }

    }


    
}

#endif // TESSERACT_H
