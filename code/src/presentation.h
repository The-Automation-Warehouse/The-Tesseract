/**
 * @file presentation.h
 * @brief Header file for the presentation sequence.
 * 
 * Contains the presentation sequence that is shwon during the presentation of the tesseract.
 * controlled by the omnitouch.
 * 
 * 
 * 
 * 
 * 
 */


#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "Entropy.h"
#include "menu.h"

#include "effects/effects.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10
extern TFT_eSPI tft;
extern int touchX, touchY;
uint32_t lastMillis = 0;
int effectIndex = 0;
uint8_t fillHue = 0;

void presentation(CRGB pixels[NUM_LEDS], TFT_eSPI tft);

void presentation(CRGB pixels[NUM_LEDS], TFT_eSPI tft)
{
    // Print the text "The Tesseract" on the TFT screen and under it the text "by Majrich Denis"
    tft.setCursor(0, 0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(5);
    tft.drawString("The Tesseract", tft.width() / 2, 120);
    
    tft.setTextSize(3.5);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString("by Majrich Denis", tft.width() / 2, 200);

    // Set the brightness of the LEDs and clear the cube
    FastLED.setBrightness(255);
    fillCube(pixels, CHSV(0, 0, 0));

    lastMillis = millis();
    effectIndex = 0;
    touchX = 0;
    touchY = 0;

    delay(1000);


    // Alternate between the different effects
    while (touchX == 0 || touchY == 0)
    {
        // If the time is greater than 15 seconds, change the effect
        if (millis() - lastMillis > 15000)
        {
            lastMillis = millis();
            effectIndex++;
            setFanSpeed(InternalTemperature.readTemperatureC());
        }

        switch (effectIndex)
        {
        case 0:
            tesseract(pixels, 100);
            break;
        case 1:
            planets(pixels, 12);
            break;
        
        case 2:
            sparkles(pixels, 10, true, 10);
            break;
        
        case 3:
            simpleRainbow(pixels, 1, 20);
            break;

        case 4:
            drawText(pixels, "TESSERACT", 0, 40, 80);
            break;

        case 5:
            lines(pixels);
            break;

        case 6:
            FastLED.setBrightness(255);
            clock(pixels, 100);
            break;

        case 7:
            setFanSpeed(0);
            FastLED.setBrightness(255);
            spectrumAnalyser(pixels, mic);
            break;

        case 8:
            FastLED.setBrightness(255);
            fillCube(pixels, CHSV(fillHue, 255, 255));
            FastLED.show();
            fillHue++;
            break;
        
        case 9:
            vortex(pixels, 6);
            break;

        default:
            effectIndex = 0;
            break;
        }

        checkTouch(tft);
        Serial.println(touchX);
        Serial.println(touchY);

        
    }
    


}

#endif // PRESENTATION_H