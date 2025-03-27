#include <Wire.h>

// Options
// #define DEBUG
// #define DEBUG_SERIAL
// #define DEBUG_DETAILED
// #define CALIBRATE
#define RUNMOTOR

#ifndef RUNMOTOR
#define DEBUG_SERIAL
#endif

#define OBSTACLE_WAIT 3000 // ms

#ifdef DEBUG
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

#include "alphabot_drivers.h"
#include "line_follower.h"
#include "line_follower_types.h"

#define NUM_SENSORS 5
int sensorValues[NUM_SENSORS];

int ir_val;
int IR_LEFT = 2, IR_RIGHT = 4;
bool OBS_LEFT = false, OBS_RIGHT = false;

#ifdef DEBUG
#define OLED_SA0 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 9 // As per schematic (D9)
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);
#endif

Line_follower__main_out _res;
Line_follower__main_mem _mem;

void motor_control();
void debug_display();

void setup() {
    init_devices();
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    Serial.begin(115200);
    Line_follower__main_reset(&_mem);

#ifdef DEBUG
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Use the correct I2C address
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("calibrating...."));
    display.display();
#endif

#ifdef CALIBRATE
    Serial.println(F("Calibration started..."));
    unsigned long startTime = millis();
    while (millis() - startTime < 10 * 1000) {
        AnalogRead(sensorValues);
        Line_follower__main_step(sensorValues[0], sensorValues[1],
                                 sensorValues[2], sensorValues[3],
                                 sensorValues[4], 0, &_res, &_mem);

        motor_control();
        delay(500);
        stop();
    }
    Serial.println(F("Calibration finished!"));
#endif

#ifdef DEBUG
    display.println(F("Calibration finished!"));
    display.display();
#endif

    _mem.ck = Line_follower__St_4_Idle;
    Serial.flush();
}

void loop() {
    // stop();
    AnalogRead(sensorValues);
    ir_val = read_ir();
    OBS_LEFT = !digitalRead(IR_LEFT);
    OBS_RIGHT = !digitalRead(IR_RIGHT);
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], ir_val, OBS_LEFT,
                             OBS_RIGHT, &_res, &_mem);
#ifdef RUNMOTOR
    motor_control();
#endif

#ifdef DEBUG_SERIAL
    debug_serial();
    delay(1000);
#endif

#ifdef DEBUG
    debug_display();
#endif
}

#ifdef DEBUG_SERIAL
void debug_serial() {
    long pid_error = _mem.pid_error_3;

    // Print sensor values compactly
    Serial.println();
    Serial.println();
    Serial.print(F("Max: "));
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_mem.max_vals_1[i] <= 9 && _mem.max_vals_1[i] >= -9)
            Serial.print(F(" "));
        if (_mem.max_vals_1[i] <= 99 && _mem.max_vals_1[i] >= -99)
            Serial.print(F(" "));
        Serial.print(_mem.max_vals_1[i]);
        Serial.print(F(" "));
    }

    Serial.println();
    Serial.print(F("Min: "));
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_mem.min_vals_1[i] <= 9 && _mem.min_vals_1[i] >= -9)
            Serial.print(F(" "));
        if (_mem.min_vals_1[i] <= 99 && _mem.min_vals_1[i] >= -99)
            Serial.print(F(" "));
        Serial.print(_mem.min_vals_1[i]);
        Serial.print(F(" "));
    }

    Serial.println();
    Serial.print(F("Raw: "));
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (sensorValues[i] <= 9 && sensorValues[i] >= -9)
            Serial.print(F(" "));
        if (sensorValues[i] <= 99 && sensorValues[i] >= -99)
            Serial.print(F(" "));
        Serial.print(sensorValues[i]);
        Serial.print(F(" "));
    }

    Serial.println();
    Serial.print(F("Cal: "));
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_mem.sen_2[i] <= 9 && _mem.sen_2[i] >= -9)
            Serial.print(F(" "));
        if (_mem.sen_2[i] <= 99 && _mem.sen_2[i] >= -99)
            Serial.print(F(" "));
        Serial.print(_mem.sen_2[i]);
        Serial.print(F(" "));
    }

    Line_follower__st_4 root_state = _mem.ck;
    Line_follower__st_3 obs_state = _mem.v_240;
    Line_follower__st_2 BW_state = _mem.v_267;
    Line_follower__st_1 inx_state = _mem.v_275;
    Line_follower__st WB_state = _mem.v_331;
    char buff[200];
    Serial.println();
    Serial.print(F("Root: "));
    Serial.print(string_of_Line_follower__st_4(root_state, buff));
    Serial.print(F("\tWonB: "));
    Serial.print(string_of_Line_follower__st(WB_state, buff));
    Serial.print(F("\tBonW: "));
    Serial.print(string_of_Line_follower__st_2(BW_state, buff));
    Serial.print("\tInx: ");
    Serial.print(string_of_Line_follower__st_1(inx_state, buff));
    Serial.print(F("\tObsA: "));
    Serial.print(string_of_Line_follower__st_3(obs_state, buff));

    Serial.println();
    Serial.print(F("er:"));
    Serial.print(pid_error);
    Serial.print(F("\tir:"));
    Serial.print(ir_val);
    Serial.print(F("\tolr:"));
    Serial.print(OBS_LEFT);
    Serial.print(OBS_RIGHT);
    Serial.print(F("\tdir:"));
    Serial.print(_res.dir);
    Serial.print(F("\tvl:"));
    Serial.print(_res.v_l);
    Serial.print(F("\tvr:"));
    Serial.print(_res.v_r);
    Serial.println();

    // Serial.flush();
}
#endif

#ifdef DEBUG
void debug_display() {
#ifdef DEBUG_DETAILED
    Line_follower__st_4 root_state = _mem.ck;
    Line_follower__st_3 obs_state = _mem.v_240;
    Line_follower__st_2 BW_state = _mem.v_267;
    Line_follower__st_1 intersection_state = _mem.v_275;
    Line_follower__st WB_state = _mem.v_331;
#endif
    long pid_error = _mem.pid_error_3;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
#ifdef DEBUG_DETAILED
    display.setTextColor(BLACK, WHITE);
#endif
    // for (int i = 0; i < NUM_SENSORS; i++) {
    //     if (sensorValues[i] >= 1000)
    //         display.print(F("*"));
    //     else
    //         display.print(sensorValues[i]);
    //     if (i != NUM_SENSORS - 1)
    //         display.print(F("."));
    // }
    // display.println();

    // Print sensor values compactly
    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_mem.sen_2[i] >= 1000)
            display.print(F("*"));
        else
            display.print(_mem.sen_2[i]);
        if (i != NUM_SENSORS - 1)
            display.print(F("."));
    }
    display.println();

#ifdef DEBUG_DETAILED
    display.setTextColor(WHITE);
    display.setTextSize(2);

    switch (root_state) {
    case Line_follower__St_4_Calibrate:
        display.print(F("Calibration"));
        break;
    case Line_follower__St_4_Idle:
        display.print(F("Idle"));
        break;
    case Line_follower__St_4_Start:
        display.print(F("StartLine"));
        break;
    case Line_follower__St_4_WonB:
        display.print(F("WB "));

        switch (WB_state) {
        case Line_follower__St_PID:
            display.print(F("PID"));
            break;
        case Line_follower__St_Recovery:
            display.print(F("Recovery"));
        default:
            break;
        }
        break;
    case Line_follower__St_4_Transition:
        display.print(F("Transition"));
        break;
    case Line_follower__St_4_BoW:
        display.print(F("BW "));
        switch (BW_state) {
        case Line_follower__St_2_PID:
            display.print(F("PID"));
            break;
        case Line_follower__St_2_Recovery:
            display.print(F("Rcvr"));
            break;
        case Line_follower__St_2_Intersection:
            display.print(F("Inx "));
            switch (intersection_state) {
            case Line_follower__St_1_GoStraight:
                display.print(F("Str"));
                break;
            case Line_follower__St_1_GoRight:
                display.print(F("Rgt"));
                break;
            // case Line_follower__St_1_GoLeftPrep:
            //     display.print(F("LftPrep"));
            //     break;
            case Line_follower__St_1_GoLeft:
                display.print(F("Lft"));
                break;
            default:
                display.print(intersection_state);
            }
            break;
        default:
            display.print(BW_state);
        }
        break;
    default:
        display.print(root_state);
    }
#endif

    display.println();
#ifdef DEBUG_DETAILED
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
#else
    display.setTextSize(2);
#endif
    display.print(pid_error);
    display.print(F(" "));
    display.print(_res.dir);
    display.print(F(" "));
    display.print(_res.v_l);
    display.print(F(" "));
    display.print(_res.v_r);

    display.display();
}
#endif

int read_ir() {
    char ir_val = ir_read();
    switch (ir_val) {
    case 'l':
        return 2;
        break;
    case 'r':
        return 1;
    case 'b':
        return 3;
    case 'n':
        return 0;
    default:
        return -1;
    }
}

void motor_control() {
    switch (_res.dir) {
    case 0:
        stop();
        break;
    case 1:
        forward();
        break;
    case 2:
        left();
        // delay(3);
        break;
    case 20:
        forward_left();
        break;
    case 25:
        sharp_left();
        break;
    case 29:
        left();
        delay(400);
        stop();
        break;
    case 3:
        right();
        // delay(3);
        break;
    case 30:
        forward_right();
        break;
    case 35:
        sharp_right();
        break;
    case 39:
        right();
        delay(400);
        stop();
        break;
    case 4:
        backward();
        break;
    case 5:
        brake();
        break;
    case 9:
        brake();
        break;
    case 99:
        brake();
        delay(OBSTACLE_WAIT);
        break;
    default:
        stop();
        break;
    }
    SetSpeed(_res.v_r, _res.v_l);

    // delay(20);
}
