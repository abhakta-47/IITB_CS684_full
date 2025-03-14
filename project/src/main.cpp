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

void setup() {
    Serial.begin(115200);
    init_devices();
    display.begin(SSD1306_SWITCHCAPVCC,
                  0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    // init done
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.println("calibrate");
    // display.display();
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
    _mem.ck = Line_follower__St_Idle;
}

void loop() {
    AnalogRead(sensorValues);
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);

    Serial.println(" ");
    // Serial.print(_res.processed_sen[0]);
    // Serial.print(" ");
    // Serial.print(_res.processed_sen[1]);
    // Serial.print(" ");
    // Serial.print(_res.processed_sen[2]);
    // Serial.print(" ");
    // Serial.print(_res.processed_sen[3]);
    // Serial.print(" ");
    // Serial.print(_res.processed_sen[4]);
    switch (_mem.ck) {
    case Line_follower__St_Turn:
        Serial.print("Turn");
        break;
    case Line_follower__St_Straight:
        Serial.print("Straight");
        break;
    case Line_follower__St_Stop:
        Serial.print("Stop");
        break;
    case Line_follower__St_Start:
        Serial.print("Start");
        break;
    case Line_follower__St_Recovery:
        Serial.print("Recovery");
        break;
    case Line_follower__St_Intersection:
        Serial.print("Intersection");
        break;
    case Line_follower__St_Idle:
        Serial.print("Idle");
        break;
    // case Line_follower__St_Brake:
    //     Serial.print("Brake");
    //     break;
    default:
        Serial.print(_mem.ck);
        break;
    }
    Serial.print(" => ");
    Serial.print(_res.dir);
    Serial.print(" ");
    Serial.print(_res.v_l);
    Serial.print(" ");
    Serial.print(_res.v_r);

    display.clearDisplay();
    display.setTextSize(2);
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
    switch (_mem.ck) {
    case Line_follower__St_Turn:
        display.print("Turn");
        break;
    case Line_follower__St_Straight:
        display.print("Straight");
        break;
    case Line_follower__St_Stop:
        display.print("Stop");
        break;
    case Line_follower__St_Start:
        display.print("Start");
        break;
    case Line_follower__St_Recovery:
        display.print("Recovery");
        break;
    case Line_follower__St_Intersection:
        display.print("Intersection");
        break;
    case Line_follower__St_Idle:
        display.print("Idle");
        break;
    // case Line_follower__St_Brake:
    //     display.print("Brake");
    //     break;
    default:
        display.print(_mem.ck);
        break;
    }
    display.print(".");
    display.print(_mem.local_dir_1);
    display.print(" => ");
    display.print(_res.dir);
    display.print(" ");
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
    case 3:
        right();
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
