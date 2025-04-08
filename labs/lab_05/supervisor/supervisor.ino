#define CALIBRATE
#define DEBUG_SERIAL
#define RUNMOTOR

#ifdef RUNMOTOR
#undef DEBUG_SERIAL
#endif

#ifdef DEBUG_SERIAL
#define TRACK_TIME
#endif

#define OBSTACLE_WAIT 3000 // ms

#include "alphabot_drivers.h"
#include "line_follower.h"

#define NUM_SENSORS 5
int sensorValues[NUM_SENSORS];

int ir_val;
int IR_LEFT = 2, IR_RIGHT = 4;
bool OBS_LEFT = false, OBS_RIGHT = false;

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

#ifdef CALIBRATE
    Serial.println(F("Calibration started..."));
    for (int i = 0; i < 5; i++) {
        _mem.max_vals_1[i] = 1024;
        _mem.min_vals_1[i] = 0;
    }
    unsigned long startTime = millis();
    while (millis() - startTime < 10 * 1000) {
        AnalogRead(sensorValues);
        Line_follower__main_step(
            sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3],
            sensorValues[4], 0, false, false, &_res, &_mem);

#ifdef RUNMOTOR
        motor_control();
#endif
        delay(350);
        stop();
    }
    Serial.println(F("Calibration finished!"));
#endif

    Serial.flush();
    _mem.ck = Line_follower__St_5_Idle;
}

int st_time, end_time;
int read_time, step_time, motor_time;
#define TIMER_FN micros

void loop() {
// stop();
#ifdef TRACK_TIME
    st_time = micros();
#endif
    // 2200 micro s
    AnalogRead(sensorValues);
    ir_val = read_ir();
    OBS_LEFT = !digitalRead(IR_LEFT);
    OBS_RIGHT = !digitalRead(IR_RIGHT);
#ifdef TRACK_TIME
    end_time = micros();
    read_time = end_time - st_time;
#endif

#ifdef TRACK_TIME
    st_time = micros();
#endif
    // 470 micro s
    Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2],
                             sensorValues[3], sensorValues[4], ir_val, OBS_LEFT,
                             OBS_RIGHT, &_res, &_mem);
#ifdef TRACK_TIME
    end_time = micros();
    step_time = end_time - st_time;
#endif

#ifdef TRACK_TIME
    st_time = micros();
#endif
#ifdef RUNMOTOR
    motor_control();
#endif
#ifdef TRACK_TIME
    end_time = micros();
    motor_time = end_time - st_time;
#endif

#ifdef DEBUG_SERIAL
    debug_serial();
    delay(1000);
#endif
}

#ifdef DEBUG_SERIAL
void debug_serial() {
    long pid_error = _mem.pid_error_11;

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

    Line_follower__st_5 root_state = _mem.ck;
    Line_follower__st_4 park_state = _mem.v_171;
    Line_follower__st_3 obs_state = _mem.v_216;
    Line_follower__st_1 inx_state = _mem.v_351;
    Line_follower__st WB_state = _mem.v_414;
    long inx_counter = _mem.inx_counter_1;
    char buff[200];
    Serial.println();
    Serial.print(F("Root: "));
    Serial.print(string_of_Line_follower__st_5(root_state, buff));
    // Serial.print(F("\tWonB: "));
    // Serial.print(string_of_Line_follower__st(WB_state, buff));
    // Serial.print(F("\tInx: "));
    // Serial.print(string_of_Line_follower__st_1(inx_state, buff));
    // Serial.print(F("."));
    // Serial.print(inx_counter);
    // Serial.print(F("\tObsA: "));
    // Serial.print(string_of_Line_follower__st_3(obs_state, buff));
    // Serial.print(F("\tPark: "));
    // Serial.print(string_of_Line_follower__st_3(park_state, buff));

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

#ifdef TRACK_TIME
    Serial.println();
    Serial.print(F("times::"));
    Serial.print(F("\tread:"));
    Serial.print(read_time);
    Serial.print(F("\tstep:"));
    Serial.print(step_time);
    Serial.print(F("\tmotor:"));
    Serial.print(motor_time);
    Serial.print(F("\ttotal:"));
    Serial.print(read_time + step_time + motor_time);
    Serial.println();
#endif
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
        break;
    case 3:
        right();
        break;
    case 4:
        backward();
        break;
    case 9:
        brake();
        break;
    default:
        stop();
        break;
    }
    SetSpeed(_res.v_r, _res.v_l);
}
