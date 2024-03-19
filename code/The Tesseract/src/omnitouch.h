/**
 * @file omnitouch.h
 * @brief Header file for the omnitouch.
 * 
 * Contains the variables and functions for the omnitouch.
 */


#ifndef OMNITOUCH_H
#define OMNITOUCH_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct OmnitouchData {
    int button;
    int mode;
    int rotation;
    float magnet;
    float temperature;
    bool assist;
};

extern OmnitouchData omnitouchData;



void printOmnitouchData();
void readOmnitouchData();






// print the joystick data to the serial monitor for debugging formated into a table
void printOmnitouchData() {
    Serial.print("B:");
    Serial.print(omnitouchData.button);
    Serial.print(" M:");
    Serial.print(omnitouchData.mode);
    Serial.print(" R:");
    Serial.print(omnitouchData.rotation);
    Serial.print(" M:");
    Serial.print(omnitouchData.magnet);
    Serial.print(" T:");
    Serial.print(omnitouchData.temperature);
    Serial.print(" A:");
    Serial.print(omnitouchData.assist);
    Serial.println();

  
}


void readOmnitouchData() {
    StaticJsonDocument<400> doc; // Create a static JSON document
    String input = "";

    // Read from Serial1 until EOT character is encountered
    while (Serial1.available()) {
        delayMicroseconds(150);
        char c = (char)Serial1.read();
        //Serial.print(c);
        if (c == '\x04') { // EOT character
            break;
        }
        if (c >= 0) { // Only add valid characters to the string
            input += c;
        }
    }
    
    DeserializationError error = deserializeJson(doc, input);

    if (error) {
        return;
    }

    omnitouchData.button = doc["b"];
    omnitouchData.mode = doc["m"];
    omnitouchData.rotation = doc["c"];
    omnitouchData.magnet = doc["h"];
    omnitouchData.temperature = doc["t"];
    omnitouchData.assist = doc["a"];

    //printOmnitouchData();


}











#endif // JOYSTICK_H
