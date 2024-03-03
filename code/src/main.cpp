//include all the required libraries
#include <Arduino.h>
#include <SPI.h>
#include <InternalTemperature.h>
#include <TFT_eSPI.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <SD.h>
#include <TimeLib.h>
#include <USBHost_t36.h>
#include <Wire.h>
#include "ArduinoJson.h"
#include "Entropy.h"

#include "tesseractFunctions.h"
#include "joystick.h"
#include "menu.h"
#include "simpleRainbow.h"
#include "drawText.h"
#include "fillCube.h"
#include "sparkles.h"
#include "vortex.h"
#include "fireworks.h"
#include "bouncingBall.h"
#include "planets.h"
#include "tetris.h"
#include "spectrumAnalyser.h"




#define SD_CS 7
const int LEFT_FAN = 28;
const int RIGHT_FAN = 29;
#define BUTTON 31
#define BUTTON_LIGHT 33
#define layer0Pin 24
#define layer1Pin 15
#define layer2Pin 16
#define layer3Pin 17
#define layer4Pin 18
#define layer5Pin 19
#define layer6Pin 20
#define layer7Pin 21
#define layer8Pin 22
#define layer9Pin 23
int channel1 = 26;
int channel2 = 27;
int mic = 40;

#define initDelay 300
double fps;
double timeStart;
double timeEnd;
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
#define NUM_LEDS 1000
#define NUM_LAYERS 10
int brightness = 20;
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
CRGB layers[NUM_LAYERS][NUM_LEDS/NUM_LAYERS]; // Initialize the layers array with the correct number of elements



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
  
  Serial.begin(9600);

  tft.drawString("Ok", textWidth/2 + tft.width() / 2, 170);
  delay(initDelay);


  textWidth = tft.drawString("SD Card...", tft.width() / 2, 200);
  delay(initDelay);

  if (!SD.begin(SPI_QUARTER_SPEED, SD_CS)) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Error", textWidth + tft.width() / 2, 200);
    while (true) {}
    
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

  FastLED.addLeds<LED_TYPE, layer0Pin, COLOR_ORDER>(layers[0], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer1Pin, COLOR_ORDER>(layers[1], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer2Pin, COLOR_ORDER>(layers[2], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer3Pin, COLOR_ORDER>(layers[3], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer4Pin, COLOR_ORDER>(layers[4], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer5Pin, COLOR_ORDER>(layers[5], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer6Pin, COLOR_ORDER>(layers[6], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer7Pin, COLOR_ORDER>(layers[7], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer8Pin, COLOR_ORDER>(layers[8], NUM_LEDS/NUM_LAYERS);
  FastLED.addLeds<LED_TYPE, layer9Pin, COLOR_ORDER>(layers[9], NUM_LEDS/NUM_LAYERS);

  FastLED.setBrightness(brightness);

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
  tft.fillRect(0, 130, tft.width() - 38, 20, TFT_BLACK);

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

  if (nextMenu == 1 || nextMenu == 2 || nextMenu == 3)
  {
    effect = effectsMenu(tft, nextMenu);

    if (effect == 0)
    {
      goto startMenu;
    
    } else 
    {
      startEffect(effectType, effect);
    }
    
  } else if (nextMenu == 4)
  {
    settingsMenu(tft);
  } else if (nextMenu == 5)
  {
    debugMenu(tft, layers, temp, fps);
  }






  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(BUTTON_LIGHT, HIGH);
  } else {
    digitalWrite(BUTTON_LIGHT, LOW);
  }


  timeStart = millis() / 1000.0;

  double timeEnd = millis() / 1000.0;
  fps = calculateFPS(timeEnd - timeStart);


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
            simpleRainbow(layers, 6, 10);
        } else if (effect == 2)
        {
            fillCube(layers, CHSV(hue, 255, 255));
            FastLED.show();
            hue += 3;
        } else if (effect == 3)
        {
            drawText(layers, "TESSERACT", 0, 40, 10);
            hue += 15;
        } else if (effect == 4)
        {
            fireworks(layers, 10);
        } else if (effect == 5)
        {
            sparkles(layers, 10, false, 10);
        } else if (effect == 6)
        {
            sparkles(layers, 10, true, 10);
        } else if (effect == 7)
        {
            planets(layers);
        } else if (effect == 8)
        {
            simpleRainbow(layers, 6, 10);
        } else if (effect == 9)
        {
            bouncingBall(layers, 0, 15, 100, 100);
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
          spectrumAnalyser(layers, mic);
        } else if (effect == 2)
        {
          spectrumAnalyser(layers, channel1);
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
          playTetris(layers, tft, 6, 10);

      } else if (effect == 2)
      {
          fillCube(layers, CHSV(hue, 255, 255));
          FastLED.show();
          hue += 3;
      } else if (effect == 3)
      {
          drawText(layers, "TESSERACT", 0, 40, 10);
          hue += 15;
      } else if (effect == 4)
      {
          bouncingBall(layers, 0, 15, 100, 100);
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
    fillCube(layers, CHSV(0, 0, 0));
    FastLED.setBrightness(brightness);
    FastLED.show();
 
}







