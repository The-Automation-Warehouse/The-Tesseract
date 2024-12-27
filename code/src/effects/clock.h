/**
 * @file clock.h
 * @brief Header file for the clock effect.
 * 
 * Creates a clock effect that travels through the cube using the drawText function.
 */


#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <TimeLib.h>
#include "tesseractFunctions.h"
#include "drawText.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

void clock(CRGB pixels[NUM_LEDS], uint8_t delayTime)
{
    time_t currentTime = Teensy3Clock.get();
    char timeStr[3];

    sprintf(timeStr, "%02d", hour(currentTime));
    drawText(pixels, timeStr, 0, 40, 80);
    delay(delayTime);
    drawText(pixels, ":", 0, 40, 80);
    delay(delayTime);
    sprintf(timeStr, "%02d", minute(currentTime));
    drawText(pixels, timeStr, 0, 40, 80);
    delay(delayTime * 12); 
}

#endif // VORTEX_H
