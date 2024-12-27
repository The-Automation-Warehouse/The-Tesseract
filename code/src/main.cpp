//include all the required libraries
#include <Arduino.h>
#include <SPI.h>
#include <InternalTemperature.h>
#include <TFT_eSPI.h>
#include <OctoWS2811.h>
#define USE_OCTOWS2811
#include <FastLED.h>
#define FastLED_INTERNAL
#include <SD.h>
#include <TimeLib.h>
#include <USBHost_t36.h>
#include <Wire.h>
#include "ArduinoJson.h"
#include "Entropy.h"

#include "tesseractFunctions.h"
#include "joystick.h"
#include "CTeensy4Controller.h"
#include "menu.h"

#include "effects/effects.h"

#include "presentation.h"




#define SD_CS 7
const int LEFT_FAN = 28;
const int RIGHT_FAN = 29;
#define BUTTON 31
#define BUTTON_LIGHT 33
int channel1 = 26;
int channel2 = 27;
int mic = 40;

#define initDelay 300
int i = 0;
String uint32ToBinary(uint32_t value);
float temp;
void setFanSpeed(int temp);
void startEffect(int effect, int effectType);

//add a function to reset the teensy on the press of a button using an interrupt
void resetTeensy() {  SCB_AIRCR = 0x05FA0004; digitalWrite(BUTTON_LIGHT, LOW); }


// define all objects and variables for the LCD
TFT_eSPI tft = TFT_eSPI();
int nextMenu = 0;


// define all objects and variables for the LEDs
int brightness = 20;

const int numPins = 10;
byte pinList[numPins] = {24, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const int ledsPerStrip = 100;
CRGB pixels[numPins * ledsPerStrip];
DMAMEM int displayMemory[ledsPerStrip * numPins * 3 / 4];
int drawingMemory[ledsPerStrip * numPins * 3 / 4];
OctoWS2811 octo(ledsPerStrip, displayMemory, drawingMemory, WS2811_GRB | WS2811_800kHz, numPins, pinList);
CTeensy4Controller<GRB, WS2811_800kHz> *pcontroller;



// define all objects and variables for the SD card

File root;





// define all objects and variables for the RTC
time_t getTeensy3Time() {
	return Teensy3Clock.get();
}




//define all objects and variables for the USB Host
JoystickData joystickData[4];






void setup(void) {

  // initialize the LCD
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);

  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("Initializing...", tft.width() / 2, 80);

  tft.setTextSize(2);
  int textWidth = tft.drawString("I/O...", tft.width() / 2, 140);
  delay(initDelay);

  analogWriteFrequency(LEFT_FAN, 20000);

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LEFT_FAN, OUTPUT);
  pinMode(RIGHT_FAN, OUTPUT);
  pinMode(BUTTON_LIGHT, OUTPUT);
  digitalWrite(BUTTON_LIGHT, HIGH);
  pinMode(channel1, INPUT);
  pinMode(channel2, INPUT);
  pinMode(mic, INPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  randomSeed(analogRead(4));
  attachInterrupt(digitalPinToInterrupt(BUTTON), resetTeensy, FALLING);
  Entropy.Initialize();

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 140);
  delay(initDelay);


  textWidth = tft.drawString("Serial...", tft.width() / 2, 170);
  delay(initDelay);
  
  Serial.begin(115200);
  Serial1.begin(115200);

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 170);
  delay(initDelay);


  textWidth = tft.drawString("SD Card...", tft.width() / 2, 200);
  delay(initDelay);

  if (!SD.begin(SPI_QUARTER_SPEED, SD_CS)) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Error", textWidth + tft.width() / 2, 200);
    //while (true) {}
    
  } else {
    tft.drawString("Ok", textWidth/2 + tft.width() / 2, 200);
    delay(initDelay);
  }


  textWidth = tft.drawString("USB Host...", tft.width() / 2, 230);
  delay(initDelay);

  myusb.begin();

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 230);
  delay(initDelay);




  textWidth = tft.drawString("LEDs...", tft.width() / 2, 260);
  delay(initDelay);

  octo.begin();
  pcontroller = new CTeensy4Controller<GRB, WS2811_800kHz>(&octo);

  FastLED.setBrightness(brightness);
  FastLED.addLeds(pcontroller, pixels, numPins * ledsPerStrip).setCorrection(TypicalLEDStrip);

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 260);
  delay(initDelay);


  textWidth = tft.drawString("RTC...", tft.width() / 2, 290);
  delay(initDelay);

  setSyncProvider(getTeensy3Time);
  timeStatus();

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 290);
  delay(initDelay);



  analogWrite(LEFT_FAN , 255);
  analogWrite(RIGHT_FAN, 255);


  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("Tesseract Ready!", tft.width() / 2, 100);
  tft.drawRect(19, 129, tft.width() - 28, 22, TFT_WHITE);
  tft.fillRect(20, 130, tft.width() - 38, 20, TFT_BLACK);

  for (int i = 0; i < tft.width() - 40; i++)
  {
    tft.fillRect(20, 130, i, 20, TFT_GREEN);
    delay(initDelay*3 / (tft.width() - 40));
  }
  delay(initDelay);

  tft.fillScreen(TFT_BLACK);
}








void loop() {

  handleUsb();

  temp = InternalTemperature.readTemperatureC();
  setFanSpeed(temp);
  FastLED.clear(true);
  
  


startMenu:

  touchX = 0;
  touchY = 0;
  int effectType = 0;
  int effect = 0;
    //  1 - Normal Effects
    //  2 - Sound Reactive Effects
    //  3 - Games

  nextMenu = mainMenu(tft);
  effectType = nextMenu;

  if (nextMenu == 1 || nextMenu == 2 || nextMenu == 3 || nextMenu == 4)
  {
    effect = effectsMenu(tft, nextMenu);

    if (effect == 0)
    {
      goto startMenu;
    
    } else 
    {
      startEffect(effectType, effect);
    }
    
  } else if (nextMenu == 5)
  {
    debugMenu(tft, pixels, temp, fps);
  } else if (nextMenu == 6)
  {
    presentation(pixels, tft);
  }





  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(BUTTON_LIGHT, HIGH);
  } else {
    digitalWrite(BUTTON_LIGHT, LOW);
  }


}


















extern int touchX, touchY;


void startEffect(int effectType, int effect)
{

  if (effectType == 1) // Normal Effects
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    int rectX = 40;
    int rectY = 40;
    int rectWidth = 80;
    int rectHeight = 80;
    

    tft.fillRect(rectX, rectY, rectWidth, rectHeight, TFT_RED);
    // Center a smaller rectangle inside the larger rectangle
    tft.fillRect(rectX + (rectWidth - rectWidth * 0.4) / 2, rectY + (rectHeight - rectHeight * 0.4) / 2, rectWidth * 0.4, rectHeight * 0.4, TFT_WHITE);

    int hue = 0;

    delay(200);

  playEffect:

    touchX = 0;
    touchY = 0;

    while (touchX == 0 || touchY == 0)
    {
        checkTouch(tft);

        if (effect == 1)
        {
            simpleRainbow(pixels, 1, 20);
        } else if (effect == 2)
        {
            fillCube(pixels, CHSV(hue, 255, 255));
            FastLED.show();
            hue++;

        } else if (effect == 3)
        {
            drawText(pixels, "TESSERACT", 0, 40, 80);
            hue += 15;
        } else if (effect == 4)
        {
            FastLED.setBrightness(160);
            lines(pixels);
        } else if (effect == 5)
        {
            sparkles(pixels, 10, false, 10);
        } else if (effect == 6)
        {
            sparkles(pixels, 10, true, 10);
        } else if (effect == 7)
        {
            planets(pixels, 12);
        } else if (effect == 8)
        {
            FastLED.setBrightness(120);
            vortex(pixels, 6);
        } else if (effect == 9)
        {
            tesseract(pixels, 100);
        } else if (effect == 10) {
            clock(pixels, 100);
        }


        if (touchX > rectX && touchX < rectX + rectWidth && touchY > rectY && touchY < rectY + rectHeight)
        { 
          break;
        }
        else
        { 
          goto playEffect;
        }
    }



  } else if (effectType == 2) // Sound Reactive Effects
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    int rectX = 40;
    int rectY = 40;
    int rectWidth = 80;
    int rectHeight = 80;

    
    int tempBrightness = FastLED.getBrightness();
    int tempTouchX = map(tempBrightness, 0, 255, 20, tft.width() - 20);

    tft.fillRect(rectX, rectY, rectWidth, rectHeight, TFT_RED);
    tft.fillRect(rectX + (rectWidth - rectWidth * 0.4) / 2, rectY + (rectHeight - rectHeight * 0.4) / 2, rectWidth * 0.4, rectHeight * 0.4, TFT_WHITE);

    tft.drawRect(19, 179, tft.width() - 28, 22, TFT_WHITE);
    tft.fillRect(20, 180, tft.width() - 20 - 2 - 20, 20, TFT_BLACK);
    tft.fillRect(20, 180, max(1, tempTouchX - 10), 20, 0x2786ba);
    tft.fillRoundRect(tempTouchX, 175, 12, 30, 3, TFT_ORANGE);


    delay(200);

    setupFFT();

playSoundEffect:

    touchX = 0;
    touchY = 0;


    while (touchX == 0 || touchY == 0)
    {
        checkTouch(tft);

        if (touchX > 20 && touchX < tft.width() - 20 && touchY > 160 && touchY < 220)
        {
          if (touchX != tempTouchX)
          {
            tempTouchX = touchX;
            tempBrightness = map(touchX, 20, tft.width() - 20, 0, 255);
            FastLED.setBrightness(tempBrightness);
            tft.fillRect(20, 175, tft.width() - 20 - 2, 4, TFT_BLACK);
            tft.fillRect(20, 201, tft.width() - 20 - 2, 4, TFT_BLACK);
            tft.drawRect(19, 179, tft.width() - 28, 22, TFT_WHITE);
            tft.fillRect(20, 180, tft.width() - 20 - 2, 20, TFT_BLACK);

            tft.fillRect(20, 180, max(1, touchX - 10), 20, 0x2786ba);
            tft.fillRoundRect(touchX, 175, 12, 30, 3, TFT_ORANGE);
          }
        }

        
        if (effect == 1)
        {
          setFanSpeed(0);
          spectrumAnalyser(pixels, mic);
        } else if (effect == 2)
        {
          //spectrumAnalyser(layers, channel1);
        } else if (effect == 3)
        {

        } else if (effect == 4)
        {

        }

        if (touchX > rectX && touchX < rectX + rectWidth && touchY > rectY && touchY < rectY + rectHeight)
        { 
          break;
        }
        else
        { 
          goto playSoundEffect;
        }
    }

  } else if (effectType == 3) // Games
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    int rectX = 40;
    int rectY = 40;
    int rectWidth = 80;
    int rectHeight = 80;

    tft.fillRect(rectX, rectY, rectWidth, rectHeight, TFT_RED);
    tft.fillRect(rectX + (rectWidth - rectWidth * 0.4) / 2, rectY + (rectHeight - rectHeight * 0.4) / 2, rectWidth * 0.4, rectHeight * 0.4, TFT_WHITE);

    int hue = 0;

    delay(200);

  playGame:

    touchX = 0;
    touchY = 0;

    while (touchX == 0 || touchY == 0)
    {
      checkTouch(tft);

      if (effect == 1)
      {
          playTetris(pixels, tft, 6, 10);

      } else if (effect == 2)
      {
          //fillCube(layers, CHSV(hue, 255, 255));
          FastLED.show();
          hue += 3;
      } else if (effect == 3)
      {
          //drawText(layers, "TESSERACT", 0, 40, 10);
          hue += 15;
      } else if (effect == 4)
      {
          //bouncingBall(layers, 0, 15, 100, 100);
      }
      if (touchX > rectX && touchX < rectX + rectWidth && touchY > rectY && touchY < rectY + rectHeight)
      { 
        break;
      }
      else
      { 
        goto playGame;
      }
    }

  }
    
    //clear the cube
    fillCube(pixels, CHSV(0, 0, 0));
    FastLED.setBrightness(brightness);
    FastLED.show();

}
