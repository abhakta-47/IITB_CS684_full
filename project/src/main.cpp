#include <Adafruit_SSD1306.h>
// #include <SPI.h>
// #include <Wire.h>

#include "alphabot_drivers.h"
#include "line_follower.h"
#include "line_follower_types.h"

#define NUM_SENSORS 5
int sensorValues[NUM_SENSORS];

#define OLED_RESET 9
#define OLED_SA0 8
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

Line_follower__main_out _res;
Line_follower__main_mem _mem;
// Line_follower__st_1 *normal_states;
// Line_follower__st *intersection_states;

void setup() {
    Serial.begin(115200);
    init_devices();
    display.begin(SSD1306_SWITCHCAPVCC,
                  0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    Line_follower__main_reset(&_mem);
    // TODO undo ck =
    // _mem.ck = Line_follower__St_2_Idle;
    // normal_states = &(_mem.v_85);
    // intersection_states = &(_mem.v_101);
}

void loop() {
    AnalogRead(sensorValues);
    Serial.print(sensorValues[0]);
    // Serial.print(sensorValues[1]);
    // Serial.print(sensorValues[2]);
    // Serial.print(sensorValues[3]);
    // Serial.print(sensorValues[4]);
    Serial.println();
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);
    for (int i = 0; i < 5; i++) {
        display.print(_mem.sen_2[i]);
        if (i != 4)
            display.print(".");
    }
    display.println("");
    display.setTextColor(WHITE);
    display.setTextSize(2);
    if (_mem.line_switched_1)
        display.print("BW");
    else
        display.print("WB");
    display.print(".");
    switch (_mem.ck) {
    case Line_follower__St_2_Transition:
        display.print("Transition");
        break;
    case Line_follower__St_2_Start:
        display.print("Start");
        break;
    case Line_follower__St_2_Normal:
        display.print("Norm.");
        display.println("");
        switch (_mem.v_98) {
        case Line_follower__St_1_Turn:
            display.print("Turn");
            break;
        case Line_follower__St_1_Straight:
            display.print("Straight");
            break;
        case Line_follower__St_1_Stop:
            display.print("Stop");
            break;
        case Line_follower__St_1_Recovery:
            display.print("Recovery");
            break;
        case Line_follower__St_1_Intersection:
            display.print("Inx.");
            switch (_mem.v_114) {
            case Line_follower__St_GoStraight:
                display.print("Str");
                break;
            case Line_follower__St_GoRight:
                display.print("Rgt");
                break;
            case Line_follower__St_GoLeft:
                display.print("Lft");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case Line_follower__St_2_Idle:
        display.print("Idle");
        break;
    case Line_follower__St_2_Calibrate:
        display.print("Calibrate");
        break;
    default:
        break;
    };

    // display.print(" => ");
    // display.print(_res.dir);
    // display.print(" ");
    // display.print(_res.v_l);
    // display.print(" ");
    // display.print(_res.v_r);
    display.display();

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
