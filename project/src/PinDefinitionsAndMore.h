#ifndef PIN_DEFINITIONS_AND_MORE_H
#define PIN_DEFINITIONS_AND_MORE_H

#include <Arduino.h>

// Define default pins for Arduino Uno
#define IR_RECEIVE_PIN 4 // Recommended pin for IR receiver
// #define IR_SEND_PIN 3          // Recommended pin for IR LED
#define IR_SEND_PWM_USE_TIMER1 // Use Timer1 for PWM (standard for Uno)

// Debugging Macros
#ifdef DEBUG
#define DBG_PRINT(...) Serial.print(__VA_ARGS__)
#define DBG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DBG_PRINT(...)   // No operation
#define DBG_PRINTLN(...) // No operation
#endif

#endif // PIN_DEFINITIONS_AND_MORE_H
