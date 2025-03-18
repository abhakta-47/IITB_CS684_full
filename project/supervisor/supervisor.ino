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

// for IR
#define Addr 0x20

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
    _mem.ck = Line_follower__St_5_Idle;

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

int read_ir();

void loop() {
    int ir_val = read_ir();
    Serial.print("Obstacle State: ");
    Serial.println(ir_val, BIN); // Print as binary (00, 01, 10, 11)

    // if (obstacleState == 0b00) {
    //     Serial.println("No Obstacle (00) → Move Forward");
    //     forward();
    // } else if (obstacleState == 0b10) {
    //     Serial.println("Left Obstacle (10) → Turn Right");
    //     right();
    // } else if (obstacleState == 0b01) {
    //     Serial.println("Right Obstacle (01) → Turn Left");
    //     left();
    // } else if (obstacleState == 0b11) {
    //     Serial.println("Front Obstacle (11) → Stop and Turn Right");
    //     stop();
    //     delay(500);
    //     right();
    // }

    delay(200); // Short delay before next check
}

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

void debug_display() {
    Line_follower__st_5 root_state = _mem.ck;
    Line_follower__st_4 BW_state = _mem.v_103;
    Line_follower__st_3 intersection_state = _mem.v_123;
    Line_follower__st_2 BW_pid_state = _mem.v_149;
    Line_follower__st_1 WB_state = _mem.v_161;
    Line_follower__st WB__pid_state = _mem.v_186;
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
    case Line_follower__St_5_Calibrate:
        display.print(F("Calibration"));
        break;
    case Line_follower__St_5_Idle:
        display.print(F("Idle"));
        break;
    case Line_follower__St_5_Start:
        display.print(F("StartLine"));
        break;
    case Line_follower__St_5_WonB:
        display.print(F("WB "));
        switch (WB_state) {
        case Line_follower__St_1_PID:
            display.print(F("PID "));
            // switch (WB_pid_state) {
            // case Line_follower__St_Straight:
            //     display.print(F("Str"));
            //     break;
            // case Line_follower__St_SoftTurn:
            //     display.print(F("Sft"));
            //     break;
            // case Line_follower__St_SharpTurn:
            //     display.print(F("shr"));
            //     break;
            // default:
            //     break;
            // }
            break;
        case Line_follower__St_1_Recovery:
            display.print(F("Rcvr"));
        default:
            break;
        }
        break;
    case Line_follower__St_5_Transition:
        display.print(F("Transition"));
        break;
    case Line_follower__St_5_BoW:
        display.print(F("BW "));
        switch (BW_state) {
        case Line_follower__St_4_PID:
            display.print(F("PID "));
            switch (BW_pid_state) {
            case Line_follower__St_2_Straight:
                display.print(F("Str"));
                break;
            case Line_follower__St_2_SoftTurn:
                display.print(F("Sft"));
                break;
            case Line_follower__St_2_SharpTurn:
                display.print(F("shr"));
                break;
            default:
                break;
            }
            break;
        case Line_follower__St_4_Recovery:
            display.print(F("Rcvr"));
            break;
        case Line_follower__St_4_Intersection:
            display.print(F("Inx "));
            switch (intersection_state) {
            case Line_follower__St_3_GoStraight:
                display.print(F("Str"));
                break;
            case Line_follower__St_3_GoRight:
                display.print(F("Rgt"));
                break;
            // case Line_follower__St_3_GoLeftPrep:
            //     display.print(F("LftPrep"));
            //     break;
            case Line_follower__St_3_GoLeft:
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
