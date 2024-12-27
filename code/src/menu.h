/**
 * @file menu.h
 * @brief Header file for the menu.
 * 
 * Contains the menu functions and variables.
 */


#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "effects/effects.h"
#include "images.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

extern float temp;
double fps;
double timeStart;
double timeEnd;

//define a array for the names of the effects
const char *normalEffects[10] = {"Rainbow", "Cube Fill", "Flying Text", "Lines", "Sparkles", "Sparkles - RGB", "Planets", "Vortex", "Tesseract", "Clock"};
const char *soundReactiveEffects[4] = {"VU Meter", "Coming soon", "Coming soon", "Coming soon"};
const char *games[4] = {"Tetris", "Coming soon", "Coming soon", "Coming soon"};
int touchX, touchY;

void drawMenu(TFT_eSPI tft, int menuIndex);
int mainMenu(TFT_eSPI tft);
int effectsMenu(TFT_eSPI tft);
void settingsMenu(TFT_eSPI tft);
void debugMenu(TFT_eSPI tft, CRGB pixels[NUM_LEDS], float temp, double fps);
void checkTouch(TFT_eSPI tft);


void drawMenu(TFT_eSPI tft, int menuIndex)
{
    if (menuIndex == 0)
    {
        mainMenu(tft);
    }
    else if (menuIndex == 1)
    {
        effectsMenu(tft);
    }
    else if (menuIndex == 2)
    {
        settingsMenu(tft);
    }
    else if (menuIndex == 3)
    {
        debugMenu(tft, pixels, temp, fps);
    }
}



int mainMenu(TFT_eSPI tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    int buttonX = 40;
    int buttonY = 30;
    int buttonWidth = 80;
    int buttonHeight = 80;

    int button2X = buttonX + buttonWidth + 30;
    int button3X = button2X + buttonWidth + 30;

    int debuxX = 400;
    int debugY = 240;
    int debugWidth = 60;
    int debugHeight = 60;

    int presentationX = 20;
    int presentationY = 240;
    int presentationWidth = 60;
    int presentationHeight = 60;


    tft.fillRect(buttonX, buttonY, buttonWidth, buttonHeight, TFT_GREEN);
    tft.pushImage(buttonX + 10, buttonY + 10, 60, 60, stars_icon);

    tft.fillRect(button2X, buttonY, buttonWidth, buttonHeight, TFT_PINK);
    tft.pushImage(button2X + 10, buttonY + 10, 60, 60, note_icon);

    tft.fillRect(button3X, buttonY, buttonWidth, buttonHeight, TFT_YELLOW);
    tft.pushImage(button3X + 10, buttonY + 10, 60, 60, gamepad_icon);

    tft.fillRect(debuxX, debugY, debugWidth, debugHeight, TFT_LIGHTGREY);
    tft.pushImage(debuxX + 5, debugY + 5, 50, 50, debug_icon);

    tft.fillRect(presentationX, presentationY, presentationWidth, presentationHeight, TFT_CYAN);
    tft.pushImage(presentationX + 5, presentationY + 5, 50, 50, presentation_icon);

    delay(200);

    touchX = 0;
    touchY = 0;

waitForTouch:

    while (touchX == 0 || touchY == 0)
    {
        checkTouch(tft);
    }

    //check if the user has touched the screen and if the user has touched the button if not, then do nothing
    if (touchX > buttonX && touchX < buttonX + buttonWidth && touchY > buttonY && touchY < buttonY + buttonHeight)
    {
        touchX = 0;
        touchY = 0;
        return 1;
    }
    else if (touchX > button2X && touchX < button2X + buttonWidth && touchY > buttonY && touchY < buttonY + buttonHeight)
    {
        touchX = 0;
        touchY = 0;
        return 2;
    }
    else if (touchX > button3X && touchX < button3X + buttonWidth && touchY > buttonY && touchY < buttonY + buttonHeight)
    {
        touchX = 0;
        touchY = 0;
        return 3;
    }
    else if (touchX > debuxX && touchX < debuxX + debugWidth && touchY > debugY && touchY < debugY + debugHeight)
    {
        touchX = 0;
        touchY = 0;
        return 5;
    }
    else if (touchX > presentationX && touchX < presentationX + presentationWidth && touchY > presentationY && touchY < presentationY + presentationHeight)
    {
        touchX = 0;
        touchY = 0;
        return 6;
    }
    else
    {
        touchX = 0;
        touchY = 0;
        goto waitForTouch;
    }
}



int effectsMenu(TFT_eSPI tft, int effectType)
{
    const char **effects = nullptr;
    if (effectType == 1)
    {
        effects = normalEffects;
    }
    else if (effectType == 2)
    {
        effects = soundReactiveEffects;
    }
    else if (effectType == 3)
    {
        effects = games;
    }
    else
    {
        return 0;
    }

    int effectIndex = 0;

    //draw the effects menu on the tft screen with each effect in a button in a wide rectangle with the effect name in the middle of the rectangle 
    //and two buttons on the top and bottom of the screen for scrolling through the effects menu
    //and a button on the top right corner of the screen for going back to the main menu

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    int effectButtonX = 40;
    int effectButtonY = 30;
    int effectButtonW = 200;
    int effectButtonH = 50;
    int upButtonX = 280;
    int upButtonY = 30;
    int upButtonW = 60;
    int upButtonH = 60;
    int downButtonX = 280;
    int downButtonY = 230;
    int downButtonW = 60;
    int downButtonH = 60;
    int backButtonX = 380;
    int backButtonY = 30;
    int backButtonW = 60;
    int backButtonH = 60;

    //draw the buttons for scrolling through the effects menu as squares with arrows in them on the top and bottom of the screen
    tft.fillRect(upButtonX, upButtonY, upButtonW, upButtonH, TFT_RED);
    tft.fillTriangle(upButtonX + upButtonW / 4, upButtonY + upButtonH * 3 / 4, upButtonX + upButtonW * 3 / 4, upButtonY + upButtonH * 3 / 4, upButtonX + upButtonW / 2, upButtonY + upButtonH / 4, TFT_WHITE);

    tft.fillRect(downButtonX, downButtonY, downButtonW, downButtonH, TFT_RED);
    tft.fillTriangle(downButtonX + downButtonW / 4, downButtonY + downButtonH / 4, downButtonX + downButtonW * 3 / 4, downButtonY + downButtonH / 4, downButtonX + downButtonW / 2, downButtonY + downButtonH * 3 / 4, TFT_WHITE);


    //draw the button for going back to the main menu as a square with an arrow in it on the top right corner of the screen

    tft.fillRect(backButtonX, backButtonY, backButtonW, backButtonH, TFT_RED);
    tft.fillTriangle(backButtonX + backButtonW / 3, backButtonY + backButtonH / 4, backButtonX + backButtonW / 3, backButtonY + backButtonH * 3 / 4, backButtonX + backButtonW * 3 / 4, backButtonY + backButtonH / 2, TFT_WHITE);

drawEffectButtons:
    //draw the buttons for the effects
    for (int i = 0; i < 4; i++)
    {
        tft.fillRect(effectButtonX, effectButtonY + i * 70, effectButtonW, effectButtonH, TFT_BLUE);
        tft.setCursor(effectButtonX + effectButtonW / 2 - tft.textWidth(effects[i + effectIndex]) / 2, effectButtonY + effectButtonH / 2 + i * 70 - tft.fontHeight() / 2);
        tft.print(effects[i + effectIndex]);
    }
    
    //check if the user has touched the screen and if the user has touched the screen, then check if the user has touched any of the buttons
    //if the user has touched any of the buttons, then do the action that the button is supposed to do
waitForTouch:

    while (touchX == 0 || touchY == 0)
    {
        checkTouch(tft);
    }

    for (int i = 0; i < 4; i++)
    {
        if (touchX > effectButtonX && touchX < effectButtonX + effectButtonW && touchY > effectButtonY + i * 70 && touchY < effectButtonY + effectButtonH + i * 70)
        {
            touchX = 0;
            touchY = 0;
            effectIndex += i;
            return effectIndex + 1;
            
        }
    }

    
    if (touchX > upButtonX && touchX < upButtonX + upButtonW && touchY > upButtonY && touchY < upButtonY + upButtonH)
    {
        touchX = 0;
        touchY = 0;
        effectIndex--;
        if (effectIndex < 0)
        {
            effectIndex = 0;
        }
        goto drawEffectButtons;
        
    }
    else if (touchX > downButtonX && touchX < downButtonX + downButtonW && touchY > downButtonY && touchY < downButtonY + downButtonH)
    {
        touchX = 0;
        touchY = 0;
        effectIndex++;
        if (effectIndex > static_cast<int>(sizeof(effects)) + 3)
        {
            effectIndex = static_cast<int>(sizeof(effects)) + 3;
        }
        goto drawEffectButtons;
        
    }
    else if (touchX > backButtonX && touchX < backButtonX + backButtonW && touchY > backButtonY && touchY < backButtonY + backButtonH)
    {
        touchX = 0;
        touchY = 0;
        effectIndex = 0;
        return effectIndex;
    }
    else
    {
        touchX = 0;
        touchY = 0;
        goto waitForTouch;
        
    }



}




void settingsMenu(TFT_eSPI tft)
{
    
}


void debugMenu(TFT_eSPI tft, CRGB pixels[NUM_LEDS], float temp, double fps)
{

    tft.fillScreen(TFT_BLACK);
    fillCube(pixels, CHSV(0, 0, 0));

    int backButtonX = 400;
    int backButtonY = 20;
    int backButtonW = 50;
    int backButtonH = 50;

    tft.fillRect(backButtonX, backButtonY, backButtonW, backButtonH, TFT_RED);
    tft.fillTriangle(backButtonX + backButtonW / 3, backButtonY + backButtonH / 4, backButtonX + backButtonW / 3, backButtonY + backButtonH * 3 / 4, backButtonX + backButtonW * 3 / 4, backButtonY + backButtonH / 2, TFT_WHITE);

    touchX = 0;
    touchY = 0;

waitForTouch:


    checkTouch(tft);

    //print the CPU temperature to the tft screen to the top left corner  
    tft.setCursor(3, 3);
    tft.drawRect(100, 0, 100, 20, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TR_DATUM);
    tft.setTextSize(2);
    tft.print("CPU: ");
    tft.println(temp);
    setFanSpeed(temp);

    //print the FPS to the tft under the CPU temperature
    tft.setCursor(3, tft.fontHeight(2));
    tft.drawRect(0, 0, 100, 20, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(2);
    tft.print("FPS: ");
    tft.println(fps);


    //print the time of the RTC to the tft screen to the top right corner
    time_t currentTime = Teensy3Clock.get();

    tft.setCursor(3, tft.fontHeight(2) * 2);
    tft.drawRect(0, 0, 100, 20, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(2);
    tft.print("Time: ");
    tft.print(hour(currentTime));
    tft.print(":");
    tft.print(minute(currentTime));
    tft.print(":");
    tft.println(second(currentTime));

    timeStart = millis() / 1000.0;

    //play a effect used for debugging
    //make 4 pixels in the corners of the cube float up and down using the drawVirtualLed function
    for (float i = 0; i <= 9; i+=0.25) {

        fadeToBlackBy(pixels, NUM_LEDS, 64);

        drawVirtualLed(0, 0, i, CHSV(1, 255, 255));
        drawVirtualLed(9, 0, i, CHSV(64, 255, 255));
        drawVirtualLed(0, 9, i, CHSV(128, 255, 255));
        drawVirtualLed(9, 9, i, CHSV(192, 255, 255));
        FastLED.show();
    }

    for (float i = 9; i >= 0; i-=0.25) {

        fadeToBlackBy(pixels, NUM_LEDS, 64);
        
        drawVirtualLed(0, 0, i, CHSV(1, 255, 255));
        drawVirtualLed(9, 0, i, CHSV(64, 255, 255));
        drawVirtualLed(0, 9, i, CHSV(128, 255, 255));
        drawVirtualLed(9, 9, i, CHSV(192, 255, 255));
        FastLED.show();
    }

    fps = calculateFPS((millis() / 1000.0) - timeStart);


    if (touchX > backButtonX && touchX < backButtonX + backButtonW && touchY > backButtonY && touchY < backButtonY + backButtonH)
    {
        touchX = 0;
        touchY = 0;
    }
    else
    {
        touchX = 0;
        touchY = 0;
        goto waitForTouch;
    }


    
}






void checkTouch(TFT_eSPI tft)
{
    //the top right corner of the touch screen is the 0,0 coordinate, but for the canvas, it is the 480,0 coordinate
    //the bottom left corner of the touch screen is the 480,320 coordinate, but for the canvas, it is the 0,320 coordinate

    uint16_t x[3] = {0};
    uint16_t y[3] = {0};
    uint16_t xFinal = 0;
    uint16_t yFinal = 0;

    //take 3 readings every time check if the reading is valid and if it is, then store it in the array
    for (int i = 0; i < 3;)
    {
        uint16_t xi;
        uint16_t yi;

        if (tft.getTouch(&xi, &yi)) {
            if (xi != 452 && yi != 18 && xi != 0 && yi != 0)
            {
                x[i] = xi;
                y[i] = yi;
                i++;

                if (i == 3)
                {
                    //avarage the 3 readings
                    xFinal = (x[0] + x[1] + x[2]) / 3;
                    yFinal = (y[0] + y[1] + y[2]) / 3;
                }
                
            }
        } 
        else {
            break;
        }

    }
 

    /*

    for (int i = 0; i < 3; i++)
    {
        Serial.print("x: ");
        Serial.print(x[i]);
        Serial.print(" y: ");
        Serial.println(y[i]);
        Serial.println();
        Serial.println("XFinal: ");
        Serial.println(xFinal);
        Serial.println("YFinal: ");
        Serial.println(yFinal);
        Serial.println();
        Serial.println();
    }

    */

    
   //convert the touch screen coordinates to the canvas coordinates
    xFinal = map(xFinal, 0, 480, 480, 0);

    touchX = xFinal;
    touchY = yFinal;
}


#endif // MENU_H