/*
 * rc: Remotely control an Arduino-based boat.
 *
 * Copyright (c) 2021 Emil Overbeck <https://github.com/Swarthe>, Claudiu Cherciu
 * Licensed under the MIT License. See LICENSE.txt for more information.
 *
 */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);    // RX, TX pins of the WiFi/Bluetooth chip


char cmd[100];    // Buffer which stores the command chars received from RoboRemo
int cmdIndex;
unsigned long lastCmdTime = 0;    // This is the last "clock time" when a command was received

// Pin configuration
const int PIN_RIGHT = 10;
const int PIN_LEFT = 6;
const int PIN_DIR_RIGHT =  12;
const int PIN_DIR_LEFT = 4;

int power = 0;


// Execute the command from cmd
void exeCmd() { 
    if (cmdStartsWith("2")) {
        stopall();
    } else if ( cmdStartsWith("3") ) {
        Serial.println("beat");
    } else if ( cmdStartsWith("0") ) {
        if (cmd[1] == ' ') {
            calcpower();
            ctrlmotor(PIN_LEFT, PIN_DIR_LEFT);

            // debug
            Serial.print("left: ");
            Serial.println(power);
        }
    } else if (cmdStartsWith("1")) {
        if (cmd[1] == ' ') {
            calcpower();
            ctrlmotor(PIN_RIGHT, PIN_DIR_RIGHT);

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
void setup() {
    Serial.begin(57600);
    mySerial.begin(9600);

    Serial.println("ready!");

    pinMode(PIN_DIR_LEFT, OUTPUT);
    pinMode(PIN_DIR_RIGHT, OUTPUT);
}



void loop() {

    // If contact lost for more than 300 milliseconds, stop motors
    if (millis() - lastCmdTime > 300) {
        Serial.println("connection lost!");

        stopall();
        delay(1000);
    }

    if (mySerial.available()) {
        char c = (char)mySerial.read(); // read char from client (RoboRemo app)

        if (c == '\n') { // if it is command ending
            cmd[cmdIndex] = 0;
            exeCmd();  // execute the command
            cmdIndex = 0; // reset the cmdIndex
        } else {
            cmd[cmdIndex] = c; // add to the cmd buffer

            if (cmdIndex < 99) cmdIndex++;
        }
    }
}
