#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "alphabot_drivers.h"
#include "line_follower.h"
#include "line_follower_types.h"

#define NUM_SENSORS 5
int sensorValues[NUM_SENSORS];

#define OLED_SA0 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 9 // As per schematic (D9)
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

Line_follower__main_out _res;
Line_follower__main_mem _mem;

void setup() {
    init_devices();

    Wire.begin();                              // Initialize I2C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Use the correct I2C address
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Hello, AlphaBot2!");
    display.display();

    Line_follower__main_reset(&_mem);
    // TODO undo ck =
    // _mem.ck = Line_follower__St_3_Idle;

    Serial.begin(115200);
}

void safePrint(long value) {
    if (value >= 0 && value <= 100) { // Adjust based on known valid enum range
        display.print(value);
    } else {
        display.print("ERR"); // Print error instead of crashing
    }
}

void motor_control();
void debug_display();

void loop() {
    AnalogRead(sensorValues);
    Serial.print(sensorValues[0]);
    Serial.print(" ");
    Serial.print(sensorValues[1]);
    Serial.print(" ");
    Serial.print(sensorValues[2]);
    Serial.print(" ");
    Serial.print(sensorValues[3]);
    Serial.print(" ");
    Serial.print(sensorValues[4]);
    Serial.println();
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);

    motor_control();
    debug_display();
}

void debug_display() {
    Line_follower__st_3 root_state = _mem.ck;
    Line_follower__st_2 BW_state = _mem.v_103;
    Line_follower__st_1 intersection_state = _mem.v_123;
    Line_follower__st WB_state = _mem.v_132;
    long pid_error = _mem.pid_error_2;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);

    // Print sensor values compactly
    for (int i = 0; i < NUM_SENSORS; i++) {
        display.print(_mem.sen_2[i]);
        if (i != NUM_SENSORS - 1)
            display.print(".");
    }
    display.println("");

    display.setTextColor(WHITE);
    display.setTextSize(2);

    // Print state information
    switch (root_state) {
    case Line_follower__St_3_Calibrate:
        display.print(F("Calibration"));
        break;
    case Line_follower__St_3_Idle:
        display.print(F("Idle"));
        break;
    case Line_follower__St_3_Start:
        display.print(F("StartLine"));
        break;
    case Line_follower__St_3_WonB:
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
    case Line_follower__St_3_Transition:
        display.print(F("Transition"));
        break;
    case Line_follower__St_3_BoW:
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

    display.println();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.print(pid_error);
    display.print(F(" "));
    display.print(_res.dir);
    display.print(F(" "));
    display.print(_res.v_l);
    display.print(F(" "));
    display.print(_res.v_r);

    display.display();
}

void motor_control() {
    SetSpeed(_res.v_r, _res.v_l);
    switch (_res.dir) {
    case 0:
        stop();
        break;
    case 1:
        forward();
        break;
    case 2:
        left();
        break;
    case 20:
        forward_left();
        break;
    case 3:
        right();
        break;
    case 30:
        forward_right();
        break;
    case 4:
        backward();
        break;
    case 5:
        brake();
        break;
    default:
        stop();
        break;
    }
}
