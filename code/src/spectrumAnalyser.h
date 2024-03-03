/**
 * @file spectrumAnalyser.h
 * @brief Header file for the spectrumAnalyser effect.
 * 
 * Creates a spectrum analyser effect that displays the frequency spectrum of an audio signal.
 */


#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include <arduinoFFT.h>


#define NUM_LEDS 1000
#define NUM_LAYERS 10
const uint16_t samples = 256;
const double samplingFrequency = 30000;
unsigned int sampling_period_us;
unsigned long microseconds;
unsigned long newTime;

double vReal[samples];
double vImag[samples];
int bandValues[] = {0,0,0,0,0,0};
byte peak[] = {0,0,0,0,0,0};
int oldBarHeight[] = {0,0,0,0,0,0};

#define NUM_BANDS 6
#define AMPLITUDE 1024

extern int brightness;

extern int channel1;
extern int channel2;
extern int mic;

arduinoFFT FFT(vReal, vImag, samples, samplingFrequency);

void soundTest(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS]);
void setupFFT();
void spectrumAnalyser(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], int inputPin);
void sampleTest();



void soundTest(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS])
{
    
    // light up the layers of the cube depending on the analogRead value of channel1
    fillCube(layers, CHSV(0, 0, 0));
    int value = analogRead(mic);
    int layer = map(value, 0, 1023, 0, 10);
    for (int i = 0; i < layer; i++) {
        for (int j = 0; j < NUM_LEDS / NUM_LAYERS; j++) {
            layers[i][j] = CHSV(255, 255, 255);
        }
    }
    FastLED.show();

    Serial.println(value);
    
} 


void setupFFT()
{
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
}


void spectrumAnalyser(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], int inputPin)
{
  // Clear the cube
  fillCube(layers, CHSV(0, 0, 0));

  // Reset the band values
  for (int i = 0; i < NUM_BANDS; i++)
  {
    bandValues[i] = 0;
  }


  // Sample the audio signal
  for (int i = 0; i < samples; i++)
  {
    newTime = micros();
    vReal[i] = analogRead(inputPin);
    vImag[i] = 0;
    while (micros() - newTime < sampling_period_us)
    {
      // Wait for sampling period
    }
  }

  // Compute the FFT
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // Analyse the FFT output
  for (int i = 2; i < (samples / 2); i++)
  {
    if (vReal[i] > 1000) {
     //6 bands, 8kHz top band
      if (i<=1 )           bandValues[0]  += (int)vReal[i];
      if (i>1   && i<=4  ) bandValues[1]  += (int)vReal[i];
      if (i>4   && i<=10  ) bandValues[2]  += (int)vReal[i];
      if (i>10   && i<=20  ) bandValues[3]  += (int)vReal[i];
      if (i>20   && i<=36  ) bandValues[4]  += (int)vReal[i];
      if (i>36           ) bandValues[5]  += (int)vReal[i];

    }
  }



  // Process the band values into a range of 0-9
  for (byte band = 0; band < NUM_BANDS; band++)
  {
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > 10) barHeight = 10;

    barHeight = ((oldBarHeight[band] * 1) + barHeight) / 2;

    if (barHeight > peak[band]) {
      peak[band] = min(10, barHeight);
    }

    // Store the bar height for the next iteration
    oldBarHeight[band] = barHeight;

    // Set the LEDs to the new bar height on the cube assigning a ring of leds to each band (band 0 = outer ring, band 4 = inner 4 leds) using the setLed function

    if (band == 5) {    // band 5 = outer ring red color
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 5), 10) + 1; j++)
        {
          setLed(4 - i, 0, j, CHSV(255, 255, 255));
          setLed(5 + i, 0, j, CHSV(255, 255, 255));
        }
      }
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 5), 10) + 1; j++)
        {
          setLed(0, 4 - i, j, CHSV(255, 255, 255));
          setLed(0, 5 + i, j, CHSV(255, 255, 255));
        }
      }
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 5), 10) + 1; j++)
        {
          setLed(9, 4 - i, j, CHSV(255, 255, 255));
          setLed(9, 5 + i, j, CHSV(255, 255, 255));
        }
      }
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 5), 10) + 1; j++)
        {
          setLed(4 - i, 9, j, CHSV(255, 255, 255));
          setLed(5 + i, 9, j, CHSV(255, 255, 255));
        }
      }
      
    } else if (band == 4) {     // band 4 = second ring yellow color
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 4), 10) + 1; j++)
        {
          setLed(4 - i, 1, j, CHSV(50, 255, 255));
          setLed(5 + i, 1, j, CHSV(50, 255, 255));
        }
      }
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 4), 10) + 1; j++)
        {
          setLed(1, 4 - i, j, CHSV(50, 255, 255));
          setLed(1, 5 + i, j, CHSV(50, 255, 255));
        }
      }
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 4), 10) + 1; j++)
        {
          setLed(8, 4 - i, j, CHSV(50, 255, 255));
          setLed(8, 5 + i, j, CHSV(50, 255, 255));
        }
      }
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 4), 10) + 1; j++)
        {
          setLed(4 - i, 8, j, CHSV(50, 255, 255));
          setLed(5 + i, 8, j, CHSV(50, 255, 255));
        }
      }
    } else if (band == 3) {     // band 3 = third ring green color
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 3), 10) + 1; j++)
        {
          setLed(4 - i, 2, j, CHSV(100, 255, 255));
          setLed(5 + i, 2, j, CHSV(100, 255, 255));
        }
      }
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 3), 10) + 1; j++)
        {
          setLed(2, 4 - i, j, CHSV(100, 255, 255));
          setLed(2, 5 + i, j, CHSV(100, 255, 255));
        }
      }
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 3), 10) + 1; j++)
        {
          setLed(7, 4 - i, j, CHSV(100, 255, 255));
          setLed(7, 5 + i, j, CHSV(100, 255, 255));
        }
      }
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 3), 10) + 1; j++)
        {
          setLed(4 - i, 7, j, CHSV(100, 255, 255));
          setLed(5 + i, 7, j, CHSV(100, 255, 255));
        }
      }
    } else if (band == 2) {     // band 2 = fourth ring blue color
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 2), 10) + 1; j++)
        {
          setLed(4 - i, 3, j, CHSV(150, 255, 255));
          setLed(5 + i, 3, j, CHSV(150, 255, 255));
        }
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 2), 10) + 1; j++)
        {
          setLed(3, 4 - i, j, CHSV(150, 255, 255));
          setLed(3, 5 + i, j, CHSV(150, 255, 255));
        }
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 2), 10) + 1; j++)
        {
          setLed(6, 4 - i, j, CHSV(150, 255, 255));
          setLed(6, 5 + i, j, CHSV(150, 255, 255));
        }
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 2), 10) + 1; j++)
        {
          setLed(4 - i, 6, j, CHSV(150, 255, 255));
          setLed(5 + i, 6, j, CHSV(150, 255, 255));
        }
      }
    } else if (band == 1) {     // band 1 = fifth ring purple color
      for (int i = 0; i < 1; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 1), 10) + 1; j++)
        {
          setLed(4 - i, 4, j, CHSV(200, 255, 255));
          setLed(5 + i, 4, j, CHSV(200, 255, 255));
        }
      }
      for (int i = 0; i < 1; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 1), 10) + 1; j++)
        {
          setLed(4, 4 - i, j, CHSV(200, 255, 255));
          setLed(4, 5 + i, j, CHSV(200, 255, 255));
        }
      }
      for (int i = 0; i < 1; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 1), 10) + 1; j++)
        {
          setLed(5, 4 - i, j, CHSV(200, 255, 255));
          setLed(5, 5 + i, j, CHSV(200, 255, 255));
        }
      }
      for (int i = 0; i < 1; i++) {
        for (int j = 0; j < min(((barHeight * (i + 1)) / 1), 10) + 1; j++)
        {
          setLed(4 - i, 5, j, CHSV(200, 255, 255));
          setLed(5 + i, 5, j, CHSV(200, 255, 255));
        }
      }
    }
  }
  FastLED.show();
 
  
}








int analogValue;
unsigned long testTime;


void sampleTest() {
  testTime = micros();
  
  // Do 1 million reads and record time taken
  for (int i = 0; i < 1000000; i++) {
    analogValue = analogRead(mic);
  }

  float conversionTime = (micros() - testTime) / 1000000.0;

  Serial.print("Conversion time: ");
  Serial.print(conversionTime);
  Serial.println(" uS");

  Serial.print("Max sampling frequency: ");
  Serial.print((1.0 / conversionTime) * 1000000);
  Serial.println(" Hz");
}





#endif // SPECTRUMANALYSER_H
