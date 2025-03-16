#include "TRSensors.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "alphabot_drivers.h"
#include "line_follower.h"
#include "line_follower_types.h"

#define NUM_SENSORS 5
TRSensors trs = TRSensors();
int sensorValues[NUM_SENSORS];

#define OLED_RESET 9
#define OLED_SA0 8
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

Line_follower__main_out _res;
Line_follower__main_mem _mem;
Line_follower__st_1 *normal_states;
Line_follower__st *intersection_states;

void setup() {
    Serial.begin(115200);
    init_devices();
    display.begin(SSD1306_SWITCHCAPVCC,
                  0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    Line_follower__main_reset(&_mem);
    normal_states = &(_mem.v_85);
    intersection_states = &(_mem.v_101);
}

void loop() {
    AnalogRead(sensorValues);
    Serial.print(sensorValues[0]);
    Serial.print(sensorValues[1]);
    Serial.print(sensorValues[2]);
    Serial.print(sensorValues[3]);
    Serial.print(sensorValues[4]);
    Serial.println();
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    // // Calbration debug
    // display.print(_res.min_cals[0]);
    // display.print(" ");
    // display.print(_res.min_cals[1]);
    // display.print(" ");
    // display.print(_res.min_cals[2]);
    // display.print(" ");
    // display.print(_res.min_cals[3]);
    // display.print(" ");
    // display.print(_res.min_cals[4]);
    // display.print("|");
    // display.print(_res.max_cals[0]);
    // display.print(" ");
    // display.print(_res.max_cals[1]);
    // display.print(" ");
    // display.print(_res.max_cals[2]);
    // display.print(" ");
    // display.print(_res.max_cals[3]);
    // display.print(" ");
    // display.print(_res.max_cals[4]);
    // display.print(" ");
    if (_mem.line_switched_1)
        display.print("BW");
    else
        display.print("WB");
    display.print(".");
    // char s_state[30] = {0};
    // string_of_Line_follower__st(*intersection_states, s_state);
    // display.println(s_state);
    if (_mem.ck != Line_follower__St_2_Normal) {
        char s2_state[30] = {0};
        string_of_Line_follower__st_2(_mem.ck, s2_state);
        display.print(s2_state);
    }
    //  else {
    //     char s1_state[30] = {0};
    //     string_of_Line_follower__st_1(_mem.v_85, s1_state);
    //     display.println(s1_state);
    // }
    // display.clearDisplay();
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
