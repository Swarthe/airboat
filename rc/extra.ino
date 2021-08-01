/*
 * Copyright (c) 2021 Emil Overbeck <https://github.com/Swarthe>, Claudiu Cherciu
 * Licensed under the MIT License. See LICENSE.txt for more information.
 *
 */

// Checks if cmd starts with string
boolean cmdStartsWith(const char *st) {
    for (int i = 0; ; i++) {
        if (st[i] == 0) return true;
        if (cmd[i] == 0) return false;
        if (cmd[i] != st[i]) return false;;
    }

    return false;
}

// Stops all motors
void stopall() {
    Serial.println("stopping...");

    analogWrite(PIN_RIGHT, 0);
    analogWrite(PIN_LEFT, 0);
}

// Converts input for motor control
void calcpower() {
    power = atoi(cmd + 2);
    power = 2 * (power - 127);

    if (power < -255) {
        power = -255;
    }

    if (power > 255) {
        power = 255;
    }
}

// Applies input to motors
void ctrlmotor(int pin, int pin_dir) {
    if (power >= 0) {
        digitalWrite(pin_dir, HIGH);
    } else {
        digitalWrite(pin_dir, LOW);
        power = -power;
    }

    // set the power of pin 9:
    analogWrite(pin, power);
}
