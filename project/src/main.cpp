/*
 * SimpleReceiver.cpp
 *
 * Demonstrates receiving ONLY NEC protocol IR codes with IRremote
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 *
 *  This file is part of Arduino-IRremote
 *https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 ************************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2025 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 *
 ************************************************************************************
 */

#include <Arduino.h>

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
// #define DECODE_DENON        // Includes Sharp
// #define DECODE_JVC
// #define DECODE_KASEIKYO
// #define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
// #define DECODE_LG
#define DECODE_NEC // Includes Apple and Onkyo. To enable all protocols ,
// just comment/disable this line.
// #define DECODE_SAMSUNG
// #define DECODE_SONY
// #define DECODE_RC5
// #define DECODE_RC6

// #define DECODE_BOSEWAVE
// #define DECODE_LEGO_PF
// #define DECODE_MAGIQUEST
// #define DECODE_WHYNTER
// #define DECODE_FAST

// #define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width
// protocols #define DECODE_HASH         // special decoder for all protocols

// #define DECODE_BEO          // This protocol must always be enabled manually,
// i.e. it is NOT enabled if no protocol is defined. It prevents decoding of
// SONY!

// #define DEBUG               // Activate this for lots of lovely debug output
// from the decoders.

// #define RAW_BUFFER_LENGTH  750 // For air condition remotes it may require up
// to 750. Default is 200.

/*
 * This include defines the actual pin number for pins like IR_RECEIVE_PIN,
 * IR_SEND_PIN for many different boards and architectures
 */
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

#define PWMA 6  // Left Motor Speed pin (ENA)
#define AIN2 A0 // Motor-L forward (IN2).
#define AIN1 A1 // Motor-L backward (IN1)
#define PWMB 5  // Right Motor Speed pin (ENB)
#define BIN1 A2 // Motor-R forward (IN3)
#define BIN2 A3 // Motor-R backward (IN4)

#define KEY2 0x18       // Key:2
#define KEY8 0x52       // Key:8
#define KEY4 0x08       // Key:4
#define KEY6 0x5A       // Key:6
#define KEY1 0x0C       // Key:1
#define KEY3 0x5E       // Key:3
#define KEY5 0x1C       // Key:5
#define SpeedDown 0x07  // Key:VOL-
#define SpeedUp 0x15    // Key:VOL+
#define ResetSpeed 0x09 // Key:EQ
#define Repeat 0xFF     // press and hold the key

unsigned long lasttime = 0;
unsigned char results;
byte flag = 0;
int Speed = 150;

char IR_decode(unsigned char *code);
void translateIR();
void forward();
void backward();
void right();
void left();
void stop();

void setup() {
    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__
                     "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN
    // pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    // Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
    pinMode(PWMA, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    Serial.println("IR control example start");
}

void loop() {
    if (IR_decode(&results)) {
        flag = 1;
        lasttime = millis();
        translateIR();
    } else {
        if (flag == 1) {
            if (millis() - lasttime > 150) {
                flag = 0;
                stop();
                Serial.println("stop");
            }
        }
    }
}

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received
// describing KEYES Remote IR codes
{
    switch (results) {
    case KEY2:
        forward();
        break;
    case KEY4:
        left();
        break;
    case KEY5:
        stop();
        break;
    case KEY6:
        right();
        break;
    case KEY8:
        backward();
        break;
    case SpeedUp:
        Speed += 10;
        if (Speed > 255)
            Speed = 250;
        break;
    case SpeedDown:
        Speed -= 10;
        if (Speed < 0)
            Speed = 0;
        break;
    case ResetSpeed:
        Speed = 150;
        break;
    case Repeat:
        break;
    default:
        stop();
    } // End Case
} // END translateIR

char IR_decode(unsigned char *code) {
    if (IrReceiver.decode()) {
        IrReceiver.resume(); // Ready for next IR signal

        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Unknown or unsupported protocol received."));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            return 0; // No valid command received
        }

        *code = IrReceiver.decodedIRData.command; // Store received command
        Serial.print(F("Received command: 0x"));
        Serial.println(*code, HEX);

        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
            Serial.println(F("Repeat signal received."));
            return 2; // Indicate repeat signal
        }

        return 1; // Valid command received
    }
    return 0; // No data received
}

void forward() {
    analogWrite(PWMA, Speed);
    analogWrite(PWMB, Speed);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
}

void backward() {
    analogWrite(PWMA, Speed);
    analogWrite(PWMB, Speed);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
}

void right() {
    analogWrite(PWMA, 50);
    analogWrite(PWMB, 50);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
}

void left() {
    analogWrite(PWMA, 50);
    analogWrite(PWMB, 50);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
}

void stop() {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
}
