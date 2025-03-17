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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Line_follower__main_out _res;
Line_follower__main_mem _mem;
// Line_follower__st_1 *normal_states;
// Line_follower__st *intersection_states;

void setup() {
    Serial.begin(115200);
    init_devices();

    Wire.begin();                              // Initialize I2C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Use the correct I2C address
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.print("Hello, AlphaBot2!");
    display.display();

    Line_follower__main_reset(&_mem);
    // TODO undo ck =
    // _mem.ck = Line_follower__St_3_Idle;
    // normal_states = &(_mem.v_85);
    // intersection_states = &(_mem.v_101);
}

void safePrint(long value) {
    if (value >= 0 && value <= 100) { // Adjust based on known valid enum range
        display.print(value);
    } else {
        display.print("ERR"); // Print error instead of crashing
    }
}

void debug() {
    display.clearDisplay();
    // display.setTextSize(1);
    // display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);
    // for (int i = 0; i < 5; i++) {
    //     display.print(_mem.sen_2[i]);
    //     if (i != 4)
    //         display.print(".");
    // }
    // display.println("");
    display.setTextColor(WHITE);
    display.setTextSize(2);
    // switch (_mem.ck) {
    // case Line_follower__St_3_Calibrate:
    //     display.print("Calibrate");
    //     break;
    // case Line_follower__St_3_Idle:
    //     display.print("Idle");
    //     break;
    // case Line_follower__St_3_Start:
    //     display.print("Start");
    //     break;
    // case Line_follower__St_3_WonB:
    //     display.print("WB.");
    //     break;
    // case Line_follower__St_3_Transition:
    //     display.print("Transition");
    //     break;
    // case Line_follower__St_3_BoW:
    //     display.print("BW.");
    //     break;
    // default:
    //     break;
    // }
    // display.display();

    // if (_mem.ck == Line_follower__St_3_WonB) {
    //     switch (_mem.v_173) {
    //     case Line_follower__St_PID:
    //         display.print("PID");
    //     case Line_follower__St_Recovery:
    //         display.print("Recovery");
    //         break;
    //     default:
    //         break;
    //     }
    // }
    //  else if (_mem.ck == Line_follower__St_3_BoW) {
    //     switch (_mem.v_105) {
    //     case Line_follower__St_2_PID:
    //         display.print("PID");
    //     case Line_follower__St_2_Recovery:
    //         display.print("Recovery");
    //         break;
    //     case Line_follower__St_2_Intersection:
    //         display.print("Inx.");
    //         break;
    //     default:
    //         break;
    //     }
    // }
    // if (&_mem != NULL &&)
    display.print((int)_mem.v_105);
    // Serial.print(_mem.v_105);
    // safePrint(_mem.v_105);
    // if (_mem.v_105 ==
    // Line_follower__St_2_Intersection) {
    //     switch (_mem.v_136) {
    //     case Line_follower__St_1_GoStraight:
    //         // display.print("Str");
    //         break;
    //     case Line_follower__St_1_GoRight:
    //         // display.print("Rgt");
    //         break;
    //     case Line_follower__St_1_GoLeftPrep:
    //         // display.print("LftPrep");
    //         break;
    //     case Line_follower__St_1_GoLeft:
    //         // display.print("Lft");
    //         break;
    //     default:
    //         break;
    //     }
    // }
    display.display();
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
