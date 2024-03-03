/**
 * @file simpleRainbow.h
 * @brief Header file for the simpleRainbow effect.
 * 
 * Creates a simple rainbow effect that travels from top to bottom.
 */


#ifndef SIMPLERAINBOW_H
#define SIMPLERAINBOW_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>


uint8_t hue;
#define NUM_LEDS 1000
#define NUM_LAYERS 10

void simpleRainbow(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t deltaHue, uint8_t density)
{

    fill_solid(layers[0], NUM_LEDS / NUM_LAYERS, CHSV(hue, 255, 255));
    fill_solid(layers[1], NUM_LEDS / NUM_LAYERS, CHSV(hue + density, 255, 255));
    fill_solid(layers[2], NUM_LEDS / NUM_LAYERS, CHSV(hue + 2 * density, 255, 255));
    fill_solid(layers[3], NUM_LEDS / NUM_LAYERS, CHSV(hue + 3 * density, 255, 255));
    fill_solid(layers[4], NUM_LEDS / NUM_LAYERS, CHSV(hue + 4 * density, 255, 255));
    fill_solid(layers[5], NUM_LEDS / NUM_LAYERS, CHSV(hue + 5 * density, 255, 255));
    fill_solid(layers[6], NUM_LEDS / NUM_LAYERS, CHSV(hue + 6 * density, 255, 255));
    fill_solid(layers[7], NUM_LEDS / NUM_LAYERS, CHSV(hue + 7 * density, 255, 255));
    fill_solid(layers[8], NUM_LEDS / NUM_LAYERS, CHSV(hue + 8 * density, 255, 255));
    fill_solid(layers[9], NUM_LEDS / NUM_LAYERS, CHSV(hue + 9 * density, 255, 255));
    FastLED.show();

    hue += deltaHue;
}

#endif // SIMPLERAINBOW_H
