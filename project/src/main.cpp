#include "TRSensors.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "alphabot_drivers.h"
#include "line_follower.h"

#define NUM_SENSORS 5
TRSensors trs = TRSensors();
unsigned int sensorValues[NUM_SENSORS];
Line_follower__main_out _res;
Line_follower__main_mem _mem;

#define OLED_RESET 9
#define OLED_SA0 8
Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

void setup() {
    init_devices();
    display.begin(SSD1306_SWITCHCAPVCC,
                  0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    // init done

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("calibrate");
    display.display();
    delay(100);
    for (int i = 0; i < 400; i++) {
        trs.calibrate(); // reads all sensors 10 times
    }
    display.clearDisplay();
    display.println("calibrated!!");
    display.display();

    display.clearDisplay();
    delay(1000);
    Serial.begin(115200);
}

void loop() {
    // AnalogRead(sensorValues);
    trs.readCalibrated(sensorValues);

    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], &_res, &_mem);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(sensorValues[0]);
    display.print(" ");
    display.print(sensorValues[1]);
    display.println(" ");
    display.print(sensorValues[2]);
    display.println(" ");
    display.print(sensorValues[3]);
    display.print(" ");
    display.print(sensorValues[4]);
    display.println(" ");
    display.print(_res.dir);
    display.print(" ");
    display.print(_res.v_l);
    display.print(" ");
    display.print(_res.v_r);
    display.display();

    // delay(1000);

    // Serial.print(sensorValues[0]);
    // Serial.print(" ");
    // Serial.print(sensorValues[1]);
    // Serial.print(" ");
    // Serial.print(sensorValues[2]);
    // Serial.print(" ");
    // Serial.print(sensorValues[3]);
    // Serial.print(" ");
    // Serial.print(sensorValues[4]);
    // Serial.print(" => ");
    // Serial.print(_res.dir);
    // Serial.print(" ");
    // Serial.print(_res.v_l);
    // Serial.print(" ");
    // Serial.print(_res.v_r);
    // Serial.println(" ");

    SetSpeed(_res.v_l, _res.v_r);
    // switch (_res.dir) {
    // case 0:
    //     stop();
    //     break;
    // // case 1:
    // //     forward();
    // //     break;
    // // case 2:
    // //     left();
    // //     break;
    // default:
    //     forward();
    //     break;
    // }
}
