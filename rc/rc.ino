/*
 * rc: Remotely control an Arduino-based boat.
 *
 * See README.md for instructions and more information.
 *
 * Copyright (C) 2021 Emil Overbeck <https://github.com/Swarthe>
 * Copyright (C) 2021 Claudiu Cherciu
 *
 * Licensed under the MIT License. See LICENSE.txt for more information.
 *
 */

#include <SoftwareSerial.h>

// Motor pin configuration
#define PIN_RIGHT        10
#define PIN_LEFT         6
#define PIN_DIR_RIGHT    12
#define PIN_DIR_LEFT     4

// RX, TX pins of the WiFi/Bluetooth chip
SoftwareSerial mySerial(2, 3);

// Buffer which stores the command chars received from RoboRemo
char cmd[100];
int cmdIndex;

// This is the last "clock time" when a command was received
unsigned long lastCmdTime = 0;

// Power supplied to the motors
int power = 0;

// Execute the command from cmd
void exeCmd()
{
    if (cmdStartsWith("2")) {
        stop_all();
    } else if (cmdStartsWith("3")) {
        Serial.println("beat");
    } else if (cmdStartsWith("0")) {
        if (cmd[1] == ' ') {
            calc_power();
            ctrl_motor(PIN_LEFT, PIN_DIR_LEFT);
            // debug
            Serial.print("left: ");
            Serial.println(power);
        }
    } else if (cmdStartsWith("1")) {
        if (cmd[1] == ' ') {
            calc_power();
            ctrl_motor(PIN_RIGHT, PIN_DIR_RIGHT);
            // debug
            Serial.print("right: ");
            Serial.println(power);
        } else {
            Serial.println("No space");
        }
    }  else {
        Serial.print("unknown command: ");
        Serial.println(cmd);
    }

    lastCmdTime = millis();
}

// Open serial communications
void setup()
{
    Serial.begin(57600);
    mySerial.begin(9600);
    Serial.println("ready!");
    pinMode(PIN_DIR_LEFT, OUTPUT);
    pinMode(PIN_DIR_RIGHT, OUTPUT);
}

void loop()
{
    // If contact lost for more than 300 milliseconds, stop motors
    if (millis() - lastCmdTime > 300) {
        Serial.println("connection lost!");
        stop_all();
        delay(1000);
    }

    if (mySerial.available()) {
        char c = (char)mySerial.read();     // read char from client (RoboRemo app)

        if (c == '\n') {                    // if it is command ending
            cmd[cmdIndex] = 0;
            exeCmd();                       // execute the command
            cmdIndex = 0;                   // reset the cmdIndex
        } else {
            cmd[cmdIndex] = c;              // add to the cmd buffer

            if (cmdIndex < 99)
                ++cmdIndex;
        }
    }
}
