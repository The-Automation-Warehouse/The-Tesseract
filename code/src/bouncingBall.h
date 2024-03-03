/**
 * @file bouncingBall.h
 * @brief Header file for the bouncingBall effect.
 * 
 * Creates a bouncing ball effect that move like the DVD screensaver.
 */


#ifndef BOUNCINGBALL_H
#define BOUNCINGBALL_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <tesseractFunctions.h>


#define NUM_LEDS 1000
#define NUM_LAYERS 10

float posX;
float posY;
float posZ;

struct Vector3D {
    float x;
    float y;
    float z;
};

Vector3D ballDirection;

// Function to normalize a vector
void normalize(Vector3D& v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= length;
    v.y /= length;
    v.z /= length;
}

void bouncingBall(CRGB layers[NUM_LAYERS][NUM_LEDS / NUM_LAYERS], uint8_t initialHue, uint8_t deltaHue, uint8_t speed, uint8_t fadeAmount)
{

    FastLED.setBrightness(255);
    posX = Entropy.random(0, 10);
    posY = Entropy.random(0, 10);
    posZ = Entropy.random(0, 10);

    ballDirection.x = Entropy.random(0, 2) * 2 - 1;
    ballDirection.y = Entropy.random(0, 2) * 2 - 1;
    ballDirection.z = Entropy.random(0, 2) * 2 - 1;


    // Normalize the direction vector
    normalize(ballDirection);

moveBall:

    // Fade the entire cube
    for (int z = 0; z < NUM_LAYERS; z++) {
        for (int i = 0; i < NUM_LEDS / NUM_LAYERS; i++) {
            layers[z][i].fadeToBlackBy(fadeAmount);
        }
    }

    // Move the ball
    posX += ballDirection.x;
    posY += ballDirection.y;
    posZ += ballDirection.z;

    // Bounce the ball off the walls
    if (posX <= 0.0f || posX >= 9.0f) {
        ballDirection.x *= -1;
        posX += ballDirection.x; // Add offset
    }
    if (posY <= 0.0f || posY >= 9.0f) {
        ballDirection.y *= -1;
        posY += ballDirection.y; // Add offset
    }
    if (posZ <= 0.0f || posZ >= 9.0f) {
        ballDirection.z *= -1;
        posZ += ballDirection.z; // Add offset
    }

    // Draw the ball, it is at a virtual floating point position, so draw the surrounding LEDs accordingly taking into account the decimal part of the position, use the setLed function to draw the ball
    // Calculate the fractional part of the ball's position
    float fracX = posX - (int)posX;
    float fracY = posY - (int)posY;
    float fracZ = posZ - (int)posZ;

    // Calculate the brightness for the LEDs based on the ball's position
    uint8_t brightness = 255 * (1.0 - sqrt(fracX * fracX + fracY * fracY + fracZ * fracZ));
    uint8_t brightnessNext = 255 * sqrt(fracX * fracX + fracY * fracY + fracZ * fracZ);

    // Draw the ball and the surrounding LEDs with the calculated brightness
    setLed((int)posX, (int)posY, (int)posZ, CHSV(initialHue, 255, brightness));
    setLed((int)posX + 1, (int)posY, (int)posZ, CHSV(initialHue, 255, brightnessNext));
    setLed((int)posX - 1, (int)posY, (int)posZ, CHSV(initialHue, 255, brightnessNext));
    setLed((int)posX, (int)posY + 1, (int)posZ, CHSV(initialHue, 255, brightnessNext));
    setLed((int)posX, (int)posY - 1, (int)posZ, CHSV(initialHue, 255, brightnessNext));
    setLed((int)posX, (int)posY, (int)posZ + 1, CHSV(initialHue, 255, brightnessNext));
    setLed((int)posX, (int)posY, (int)posZ - 1, CHSV(initialHue, 255, brightnessNext));



    
    FastLED.show();
    delay(speed);

    initialHue += deltaHue;

    goto moveBall;  

}



#endif // BOUNCINGBALL_H
