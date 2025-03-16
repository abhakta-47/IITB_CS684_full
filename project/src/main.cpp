#include "TRSensors.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "alphabot_drivers.h"
#include "line_follower.h"

#define NUM_SENSORS 5
TRSensors trs = TRSensors();
int sensorValues[NUM_SENSORS];
Line_follower__main_out _res;
Line_follower__main_mem _mem;

#define OLED_RESET 9
#define OLED_SA0 8
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);
Line_follower__st_1 *nor_states;
Line_follower__st *int_states;

void setup() {
    Serial.begin(115200);
    init_devices();
    display.begin(SSD1306_SWITCHCAPVCC,
                  0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    // init done
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("hello");
    display.display();
    // delay(100);
    // for (int i = 0; i < 400; i++) {
    //     trs.calibrate(); // reads all sensors 10 times
    // }
    // display.clearDisplay();
    // display.println("calibrated!!");
    // display.display();
    // display.clearDisplay();
    // Serial.println();
    // Serial.print("mins: ");
    // for (int i = 0; i < NUM_SENSORS; i++) {
    //     Serial.print(trs.calibratedMin[i]);
    //     Serial.print(' ');
    // }
    // Serial.println();
    // Serial.print("maxs: ");
    // for (int i = 0; i < NUM_SENSORS; i++) {
    //     Serial.print(trs.calibratedMax[i]);
    //     Serial.print(' ');
    // }
    // Serial.println("calibrate done");
    // Serial.println();
    // delay(10000);
    _mem.ck = Line_follower__St_2_Idle;
    nor_states = &(_mem.v_97);
    int_states = &(_mem.v_113);
    *nor_states = Line_follower__St_1_Straight;
    *int_states = Line_follower__St_GoStraight;
}

void loop() {
    AnalogRead(sensorValues);
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // display.print(_res.processed_sen[0]);
    // display.print(" ");
    // display.print(_res.processed_sen[1]);
    // display.println(" ");
    // display.print(_res.processed_sen[2]);
    // display.println(" ");
    // display.print(_res.processed_sen[3]);
    // display.print(" ");
    // display.print(_res.processed_sen[4]);
    // display.println(" ");
    display.print(_mem.line_switched_1);
    display.print(".");
    switch (_mem.ck) {
    case Line_follower__St_2_Idle:
        display.print("Idle");
        break;
    case Line_follower__St_2_Start:
        display.print("Start");
        break;
    case Line_follower__St_2_Normal:
        display.print("Normal.");
        switch (*nor_states) {
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
            display.print("Int.");
            switch (*int_states) {
            case Line_follower__St_GoStraight:
                display.print("Str");
                break;
            case Line_follower__St_GoRight:
                display.print("Right");
                break;
            case Line_follower__St_GoLeft:
                display.print("Left");
                break;
            default:
                display.print(*int_states);
                break;
            }
            break;
        default:
            display.print(*nor_states);
            break;
        }

        break;
    case Line_follower__St_2_Transition:
        display.print("Transition");
        break;
    default:
        display.print(_mem.ck);
        break;
    }
    display.print(" => ");
    display.print(_res.v_l);
    display.print(" ");
    display.print(_res.v_r);
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
