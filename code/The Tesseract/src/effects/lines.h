/**
 * @file lines.h
 * @brief Header file for the lines effect.
 * 
 * Creates random lines that move through the cube in random directions.
 */


#ifndef LINES_H
#define LINES_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include "tesseractFunctions.h"
#include "Entropy.h"

#define NUM_LEDS 1000
#define NUM_LAYERS 10

void lines(CRGB pixels[NUM_LEDS])
{

    // Create a static array of lines with each line having a random position and direction
    static struct Line
    {
        bool initialized = false;
        float x;
        float y;
        float z;
        float xDir;
        float yDir;
        float zDir;
        float hue = 0;

    } lines[10];


    // Fade the cube to black
    fadeToBlackBy(pixels, NUM_LEDS, 20);
        

    for (int i = 0; i < 10; i++)
    {
        readOmnitouchData();
        if (omnitouchData.button == 1)
        {
            return;
        }
        
        if (!lines[i].initialized)
        {
            // pick a random starting side of the cube for each line
            int side = Entropy.random(6);
            //  0 - top
            //  1 - bottom
            //  2 - left
            //  3 - right
            //  4 - front
            //  5 - back

            // pick a random starting position for each line depending on the side and set its direction
            switch (side)
            {
            case 0:
                lines[i].x = Entropy.random(10);
                lines[i].y = Entropy.random(10);
                lines[i].z = 9;
                lines[i].xDir = 0;
                lines[i].yDir = 0;
                lines[i].zDir = -0.25;
                break;
            case 1:
                lines[i].x = Entropy.random(10);
                lines[i].y = Entropy.random(10);
                lines[i].z = 0;
                lines[i].xDir = 0;
                lines[i].yDir = 0;
                lines[i].zDir = 0.25;
                break;
            case 2:
                lines[i].x = 0;
                lines[i].y = Entropy.random(10);
                lines[i].z = Entropy.random(10);
                lines[i].xDir = 0.25;
                lines[i].yDir = 0;
                lines[i].zDir = 0;
                break;
            case 3:
                lines[i].x = 9;
                lines[i].y = Entropy.random(10);
                lines[i].z = Entropy.random(10);
                lines[i].xDir = -0.25;
                lines[i].yDir = 0;
                lines[i].zDir = 0;
                break;
            case 4:
                lines[i].x = Entropy.random(10);
                lines[i].y = 0;
                lines[i].z = Entropy.random(10);
                lines[i].xDir = 0;
                lines[i].yDir = 0.25;
                lines[i].zDir = 0;
                break;
            case 5:
                lines[i].x = Entropy.random(10);
                lines[i].y = 9;
                lines[i].z = Entropy.random(10);
                lines[i].xDir = 0;
                lines[i].yDir = -0.25;
                lines[i].zDir = 0;
                break;
            }

            lines[i].hue += 15;
            lines[i].initialized = true;
        }

        // draw the line at its current position
        drawVirtualLed(lines[i].x, lines[i].y, lines[i].z, CHSV(lines[i].hue, 255, 255));

        // move the line in its direction
        lines[i].x += lines[i].xDir;
        lines[i].y += lines[i].yDir;
        lines[i].z += lines[i].zDir;


        // if the line is outside the cube, reset its position and direction
        if (lines[i].x < 0 || lines[i].x > 9 || lines[i].y < 0 || lines[i].y > 9 || lines[i].z < 0 || lines[i].z > 9)
        {
            lines[i].initialized = false;
        }

        // increment the hue for the next line
        

        FastLED.show();
        delay(2);

    }

    
}

#endif // VORTEX_H
