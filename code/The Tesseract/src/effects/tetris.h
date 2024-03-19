/**
 * @file tetris.h
 * @brief Header file for the Tetris game.
 * 
 * This file contains the function prototypes for the Tetris game.
 */


#ifndef TETRIS_H
#define TETRIS_H

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include "tesseractFunctions.h"
#include "joystick.h"
#include "Entropy.h"


#define NUM_LEDS 1000
#define NUM_LAYERS 10

int block[4][4][4] = {0};
int blockX, blockY, blockZ;

int blockType = 0; // 0 - I Block, 1 - J Block, 2 - L Block, 3 - O Block, 4 - S Block, 5 - T Block, 6 - Z Block
CRGB colors[7] = {CRGB::Cyan, CRGB::Blue, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Purple, CRGB::Red};

int score = 0;
bool gameOver;

extern JoystickData joystickData[4];

void spawnNewBlock(CRGB pixels[NUM_LEDS]);
void playTetris(CRGB pixels[NUM_LEDS], TFT_eSPI tft, uint8_t speed, uint8_t fadeAmount);
void drawBlock(CRGB pixels[NUM_LEDS], int x, int y, int z, int blockType);
void handleInput(CRGB pixels[NUM_LEDS]);
int checkForCollision(CRGB pixels[NUM_LEDS], int x, int y, int z);
bool checkForCompleteLayer(CRGB pixels[NUM_LEDS]);
void dropBlockDown(CRGB pixels[NUM_LEDS]);
void moveBlock(CRGB pixels[NUM_LEDS], int directionX, int directionY, int directionZ);
void rotateBlock(CRGB pixels[NUM_LEDS], int direction, int axis);
void exitGame(CRGB pixels[NUM_LEDS]);




void playTetris(CRGB pixels[NUM_LEDS], TFT_eSPI tft, uint8_t speed, uint8_t fadeAmount)
{
    tft.fillScreen(TFT_BLACK);
    FastLED.setBrightness(255);
    FastLED.clear(true);

    score = 0;
    gameOver = false;

    tft.fillScreen(TFT_BLACK);
    //center the text "score: " and the score on the screen
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(4);
    tft.drawString("Score: ", tft.width() / 2, 80);
    tft.drawString(String(score), tft.width() / 2, 140);
    FastLED.show();

    spawnNewBlock(pixels);

    while (!gameOver) {

        readOmnitouchData();
        if (omnitouchData.button == 1) {
            return;
        }


        handleUsb();
        handleInput(pixels);

        while (checkForCompleteLayer(pixels)) {
            score += 100;
            tft.fillScreen(TFT_BLACK);
            //center the text "score: " and the score on the screen
            tft.setCursor(0, 0);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextDatum(MC_DATUM);
            tft.setTextSize(4);
            tft.drawString("Score: ", tft.width() / 2, 80);
            tft.drawString(String(score), tft.width() / 2, 140);
            FastLED.show();
            
        }

    }
    

}




void spawnNewBlock(CRGB pixels[NUM_LEDS])
{

    blockType = Entropy.random(0, 7);
    blockX = 4;
    blockY = 4;
    blockZ = 5;

    drawBlock(pixels, blockX, blockY, blockZ, blockType);
    FastLED.show();

}


void handleInput(CRGB pixels[NUM_LEDS])
{
    if (joystickData[0].axisDataX == 0) {   // Move block left
        moveBlock(pixels, -1, 0, 0);
        while (joystickData[0].axisDataX == 0) { handleUsb(); }
    }

    if (joystickData[0].axisDataX == 255) {   // Move block right
        moveBlock(pixels, 1, 0, 0);
        while (joystickData[0].axisDataX == 255) { handleUsb(); }
    }

    if (joystickData[0].axisDataY == 0) {      // Move block back
        moveBlock(pixels, 0, 1, 0);
        while (joystickData[0].axisDataY == 0) { handleUsb(); }
    }

    if (joystickData[0].axisDataY == 255) {     // Move block forward
        moveBlock(pixels, 0, -1, 0);
        while (joystickData[0].axisDataY == 255) { handleUsb(); }
    }

    if (joystickData[0].buttonData[0] == 1) {   // Drop block down
        dropBlockDown(pixels);
        while (joystickData[0].buttonData[0] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[2] == 1) {   // Drop block down
        moveBlock(pixels, 0, 0, -1);
        while (joystickData[0].buttonData[2] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[1] == 1) {   // Rotate block clockwise
        rotateBlock(pixels, 1, 0);
        while (joystickData[0].buttonData[1] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[3] == 1) {   // Rotate block counterclockwise
        rotateBlock(pixels, -1, 0);
        while (joystickData[0].buttonData[3] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[4] == 1) {   // Rotate block up
        rotateBlock(pixels, -1, 1);
        while (joystickData[0].buttonData[4] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[5] == 1) {   // Rotate block down
        rotateBlock(pixels, -1, 1);
        while (joystickData[0].buttonData[5] == 1) { handleUsb(); }
    }

    if (joystickData[0].buttonData[9] == 1) {   // Restart game
        exitGame(pixels);
        while (joystickData[0].buttonData[9] == 1) { handleUsb(); }
    }
}



void moveBlock(CRGB pixels[NUM_LEDS], int directionX, int directionY, int directionZ)
{
    //check if every pixel o the block array is within the bounds of the cube and if the space is empty
    Serial.println("Moving block");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                //only check for collision if the block is present in the block array and the space it is moving into is not part of the block
                if (block[i][j][k] == 1) {
                    if (i + directionX >= 0 && i + directionX < 4 &&
                        j + directionY >= 0 && j + directionY < 4 &&
                        k + directionZ >= 0 && k + directionZ < 4 &&
                        block[i + directionX][j + directionY][k + directionZ] == 1) {
                    } else {
                    
                        int collision = checkForCollision(pixels, blockX + i + directionX, blockY + j + directionY, blockZ + k + directionZ);
                        Serial.println(blockX + i + directionX);
                        Serial.println(blockY + j + directionY);
                        Serial.println(blockZ + k + directionZ);
                        if (collision == 2) {
                            spawnNewBlock(pixels);
                            return;
                        }

                        if (directionZ == -1) {
                            collision = checkForCollision(pixels, blockX + i, blockY + j, blockZ + k + directionZ);
                            if (collision == 1 || collision == 2) { // Check for both types of collision when moving down
                                spawnNewBlock(pixels);
                                return;
                            }
                        }

                        if (collision == 1) {
                            return;
                        }
                    }
                }
            }
        }
    }

    //clear the block
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (block[i][j][k] == 1) {
                    setLed(blockX + i, blockY + j, blockZ + k, CRGB(0, 0, 0));
                }
            }
        }
    }

    //move the block
    blockX += directionX;
    blockY += directionY;
    blockZ += directionZ;

    //draw the block
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (block[i][j][k] == 1) {
                    setLed(blockX + i, blockY + j, blockZ + k, colors[blockType]);
                }
            }
        }
    }

    FastLED.show();

}


void rotateBlock(CRGB pixels[NUM_LEDS], int direction, int axis)
{
    // axis = 0 - rotate around the x axis, axis = 1 - rotate around the y axis, axis = 2 - rotate around the z axis

    //clear the block
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (block[i][j][k] == 1) {
                    setLed(blockX + i, blockY + j, blockZ + k, CRGB(0, 0, 0));
                }
            }
        }
    }

    int tempBlock[4][4][4] = {0};

    //rotate the block around its virtual center depending on the axis
    if (axis == 0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (direction == 1) {
                        tempBlock[i][j][k] = block[i][3 - k][j];
                    } else {
                        tempBlock[i][j][k] = block[i][k][3 - j];
                    }
                }
            }
        }

    } else if (axis == 1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (direction == 1) {
                        tempBlock[i][j][k] = block[3 - k][j][i];
                    } else {
                        tempBlock[i][j][k] = block[k][j][3 - i];
                    }
                }
            }
        }

    } else if (axis == 2) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (direction == 1) {
                        tempBlock[i][j][k] = block[j][3 - i][k];
                    } else {
                        tempBlock[i][j][k] = block[3 - j][i][k];
                    }
                }
            }
        }
    }

    //check if every pixel o the block array is within the bounds of the cube and if the space is empty
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (tempBlock[i][j][k] == 1) {
                    int collision = checkForCollision(pixels, blockX + i, blockY + j, blockZ + k);
                    if (collision != 0) {
                        return;
                    }
                }
            }
        }
    }

    //draw the block
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                block[i][j][k] = tempBlock[i][j][k];
            }
        }
    }

    //draw the block with its center at x, y, z and its color from the blockType and colors array using the setLed function
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (block[i][j][k] == 1) {
                    setLed(blockX + i, blockY + j, blockZ + k, colors[blockType]);
                }
            }
        }
    }

    FastLED.show();

}



void dropBlockDown(CRGB pixels[NUM_LEDS])
{
    while (blockZ > 0)
    {
        moveBlock(pixels, 0, 0, -1);
    }
    
}




int checkForCollision(CRGB pixels[NUM_LEDS], int x, int y, int z)
{
    // 0 - no collision, 1 - other collision, 2 - collision at the bottom

    if (z < 0) {
        return 2;
    }
    if (x < 0 || x > 9 || y < 0 || y > 9 || z < 0 || z > 9) {
        return 1;
    }
    if (getLedColour(x, y, z) != CRGB(0, 0, 0)) {
        return 1;
    }
    
    return 0;
}



bool checkForCompleteLayer(CRGB pixels[NUM_LEDS])
{
    //check if every pixel of the bottom layer is not empty
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (getLedColour(i, j, 0) == CRGB(0, 0, 0)) {
                return false;
            }
       }
    }

    //store the colours of the entire cube in a 3D array
    CRGB grid[10][10][10] = {0};

    for (int k = 0; k < 10; k++) {
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                grid[i][j][k] = getLedColour(i, j, k);
            }
        }
    }

    //move every layer down by one
    for (int k = 1; k < 10; k++) {
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                setLed(i, j, k - 1, grid[i][j][k]);
            }
        }
    }

    FastLED.show();
    return true;

}




void drawBlock(CRGB pixels[NUM_LEDS], int x, int y, int z, int blockType)
{
    //clear the block
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                block[i][j][k] = 0;
            }
        }
    }
    

    if (blockType == 0) {   // I Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }

    } else if (blockType == 1) {     // J Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {0}, {0}, {0} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {0}, {0}, {0} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }

    } else if (blockType == 2) {     // L Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {1} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {1} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }
    } else if (blockType == 3) {     // O Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {0} },
                { {0}, {1}, {1}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {0} },
                { {0}, {1}, {1}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }

    } else if (blockType == 4) {     // S Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {1} },
                { {1}, {1}, {1}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {1} },
                { {1}, {1}, {1}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }

    } else if (blockType == 5) {     // T Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {0} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {1}, {1}, {0} },
                { {1}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }
    } else if (blockType == 6) {     // Z Block
        int tempBlock[4][4][4] = {
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {1}, {1}, {0} },
                { {0}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {1}, {1}, {1}, {0} },
                { {0}, {1}, {1}, {1} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            },
            {
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} },
                { {0}, {0}, {0}, {0} }
            }
        };
    
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    block[i][j][k] = tempBlock[i][j][k];
                }
            }
        }
    }

    //draw the block with its center at x, y, z and its color from the blockType and colors array using the setLed function
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (block[i][j][k] == 1) {
                    setLed(x + i, y + j, z + k, colors[blockType]);
                }
            }
        }
    }
    FastLED.show();
}




void exitGame(CRGB pixels[NUM_LEDS])
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                setLed(i, j, k, CRGB(0, 0, 0));
            }
        }
    }
    FastLED.show();
    delay(1000);
    playTetris(pixels, tft, 100, 100);
}


#endif // TETRIS_H
