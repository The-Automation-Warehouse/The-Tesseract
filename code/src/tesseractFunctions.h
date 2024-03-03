/*
    * TesseractFunctions.h
    *
    *  Created on: 2013-10-25
*/
#include "Arduino.h"
#include <FastLED.h>
#include <SD.h>
#include <USBHost_t36.h>



#ifndef TESSERACTFUNCTIONS_H_
#define TESSERACTFUNCTIONS_H_

extern CRGB layers[10][100];
extern int brightness;
extern TFT_eSPI tft;
extern File root;
extern float temp;
extern const int LEFT_FAN;
extern const int RIGHT_FAN;

String uint32ToBinary(uint32_t value);
double calculateFPS(double seconds);
void setFanSpeed(int temp);
void printDirectory(File dir, int numTabs);
int getLedIndex(int x, int y, int z);
void setLed(int x, int y, int z, CRGB color);
CRGB getLedColour(int x, int y, int z);
void showAxis();
CRGB colorFraction(CRGB colorIn, float fraction);
void drawVirtualLed(float xPos, float yPos, float zPos, CRGB color);


/////////// General Functions ///////////

String uint32ToBinary(uint32_t value)
{
    String binary = "";
    for (int i = 31; i >= 0; i--)
    {
        binary += ((value >> i) & 1) ? "1" : "0";
    }
    return binary;
}


double calculateFPS(double seconds)
{
    static double framesPerSecond = 0.0;
    framesPerSecond = 0.9 * framesPerSecond + (1.0 / seconds * 0.1);
    return framesPerSecond;
}


void setFanSpeed(int temp) {
  if (temp > 70) {
    analogWrite(LEFT_FAN, 255);
    analogWrite(RIGHT_FAN, 255);
  } else if (temp > 60) {
    analogWrite(LEFT_FAN, 210);
    analogWrite(RIGHT_FAN, 210);
  } else if (temp > 50) {
    analogWrite(LEFT_FAN, 180);
    analogWrite(RIGHT_FAN, 180);
  } else if (temp > 40) {
    analogWrite(LEFT_FAN, 150);
    analogWrite(RIGHT_FAN, 150);
  } else {
    analogWrite(LEFT_FAN, 0);
    analogWrite(RIGHT_FAN, 0);
  }
}












/////////// SD Card Functions ///////////


void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}













/////////// Joystick Functions ///////////















/////////// LED Functions ///////////


int getLedIndex(int x, int y, int z) {
  int index = 0;
  if (y % 2 == 0) {
    index = (y * 10) + x;
  } else {
    index = (y * 10) + (9 - x);
  }
  return index;
}


void setLed(int x, int y, int z, CRGB color) {
  int index = getLedIndex(x, y, z);
  layers[z][index] = color;
}

CRGB getLedColour(int x, int y, int z) {
  int index = getLedIndex(x, y, z);
  return layers[z][index];
}


void showAxis() {
  setLed(0, 0, 0, CRGB(255, 255, 255));
  setLed(1, 0, 0, CRGB(0, 255, 0));
  setLed(0, 1, 0, CRGB(0, 0, 255));
  setLed(0, 0, 1, CRGB(255, 0, 0));
  FastLED.show();
}



CRGB colorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

void drawVirtualLed(float xPos, float yPos, float zPos, CRGB color) {

  // takes in a virtual floating point LED position and a color and draws the color on the surrounding LEDs dependiong on their distance from the virtual LED

  // Calculate the position of the surrounding 8 LEDs
  int xPosFloor = (int)xPos;
  int yPosFloor = (int)yPos;
  int zPosFloor = (int)zPos;

  int xPosCeil = xPosFloor + 1;
  int yPosCeil = yPosFloor + 1;
  int zPosCeil = zPosFloor + 1;
  
  // Calculate the distance of the virtual LED from the surrounding 8 LEDs
  float xDistFloor = 1.0f - (xPos - (float)xPosFloor);
  float yDistFloor = 1.0f - (yPos - (float)yPosFloor);
  float zDistFloor = 1.0f - (zPos - (float)zPosFloor);

  float xDistCeil = 1.0f - xDistFloor;
  float yDistCeil = 1.0f - yDistFloor;
  float zDistCeil = 1.0f - zDistFloor;

  // Calculate the amount of color to draw on each LED (some can be 0.0f)
  float xAmountFloor = min(xDistFloor, 1.0f);
  float yAmountFloor = min(yDistFloor, 1.0f);
  float zAmountFloor = min(zDistFloor, 1.0f);

  float xAmountCeil = min(xDistCeil, 1.0f);
  float yAmountCeil = min(yDistCeil, 1.0f);
  float zAmountCeil = min(zDistCeil, 1.0f);

  // Draw the color on the surrounding LEDs
  setLed(xPosFloor, yPosFloor, zPosFloor, getLedColour(xPosFloor, yPosFloor, zPosFloor) + colorFraction(color, xAmountFloor * yAmountFloor * zAmountFloor));
  setLed(xPosCeil, yPosFloor, zPosFloor, getLedColour(xPosCeil, yPosFloor, zPosFloor) + colorFraction(color, xAmountCeil * yAmountFloor * zAmountFloor));
  setLed(xPosFloor, yPosCeil, zPosFloor, getLedColour(xPosFloor, yPosCeil, zPosFloor) + colorFraction(color, xAmountFloor * yAmountCeil * zAmountFloor));
  setLed(xPosCeil, yPosCeil, zPosFloor, getLedColour(xPosCeil, yPosCeil, zPosFloor) + colorFraction(color, xAmountCeil * yAmountCeil * zAmountFloor));
  setLed(xPosFloor, yPosFloor, zPosCeil, getLedColour(xPosFloor, yPosFloor, zPosCeil) + colorFraction(color, xAmountFloor * yAmountFloor * zAmountCeil));
  setLed(xPosCeil, yPosFloor, zPosCeil, getLedColour(xPosCeil, yPosFloor, zPosCeil) + colorFraction(color, xAmountCeil * yAmountFloor * zAmountCeil));
  setLed(xPosFloor, yPosCeil, zPosCeil, getLedColour(xPosFloor, yPosCeil, zPosCeil) + colorFraction(color, xAmountFloor * yAmountCeil * zAmountCeil));
  setLed(xPosCeil, yPosCeil, zPosCeil, getLedColour(xPosCeil, yPosCeil, zPosCeil) + colorFraction(color, xAmountCeil * yAmountCeil * zAmountCeil));

}







#endif /* TESSERACTFUNCTIONS_H_ */