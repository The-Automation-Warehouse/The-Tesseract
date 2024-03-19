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
#include "omnitouch.h"

#include "effects/effects.h"
#include "effects/white.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10
extern OmnitouchData omnitouchData;

void presentation(CRGB pixels[NUM_LEDS], TFT_eSPI tft);
void waitForOmniTouch();

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
    FastLED.setBrightness(120);
    fillCube(pixels, CHSV(0, 0, 0));


    // Wait for the press of the omnitouch before starting the presentation and showing the tessaract effect
    while (omnitouchData.button != 1) {
        readOmnitouchData();
    }
    waitForOmniTouch();


    // First slide of the presentation - the tesseract effect running
    while (omnitouchData.button != 1) {
        tesseract(pixels, 120);
    }
    waitForOmniTouch();



    setFanSpeed(0);
    // Second slide of the presentation - the spectrum analyser effect running
    while (omnitouchData.button != 1) {
        spectrumAnalyser(pixels, 40);
    }
    setFanSpeed(InternalTemperature.readTemperatureC());
    waitForOmniTouch();



    // Third slide of the presentation - the flying text "The Tesseract" effect running
    while (omnitouchData.button != 1) {
        drawText(pixels, "TESSERACT", 0, 40, 80);
    }
    waitForOmniTouch();


    // Fifth slide of the presentation - the clock effect running
    while (omnitouchData.button != 1) {
        clock(pixels, 30);
    }
    waitForOmniTouch();



    // Fifth slide of the presentation - a game of tetris running
    while (omnitouchData.button != 1) {
        playTetris(pixels, tft, 6, 10);
    }
    waitForOmniTouch();

    // Reprint the text "The Tesseract" on the TFT screen and under it the text "by Majrich Denis"
    tft.setCursor(0, 0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(5);
    tft.drawString("The Tesseract", tft.width() / 2, 120);
    
    tft.setTextSize(3.5);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString("by Majrich Denis", tft.width() / 2, 200);




    // Sixth slide of the presentation - the simpleRainbow effect running
    while (omnitouchData.button != 1) {
        simpleRainbow(pixels, 1, 20);
    }
    waitForOmniTouch();
    


    // Seventh slide of the presentation - the planets effect running
    while (omnitouchData.button != 1) {
        planets(pixels, 12);
    }
    waitForOmniTouch();



    // Eighth slide of the presentation - the white effect running
    while (omnitouchData.button != 1) {
        white(pixels);
    }
    waitForOmniTouch();



    // Eighth slide of the presentation - the lines effect running
    while (omnitouchData.button != 1) {
        lines(pixels);
    }
    waitForOmniTouch();


    // Eleventh slide of the presentation - the magnet effect running
    while (omnitouchData.button != 1) {
        magnet(pixels);
    }
    waitForOmniTouch();


    // Twelfth slide of the presentation - the vortex effect running
    while (omnitouchData.button != 1) {
        vortex(pixels, 6);
    }
    waitForOmniTouch();
    
    

    // Wait for the press of the omnitouch
    // end of presentation
    fillCube(pixels, CHSV(0, 0, 0));


}


void waitForOmniTouch()
{
    while (omnitouchData.button != 0) {
        readOmnitouchData();
    }
}

#endif // PRESENTATION_H