# CS684 :: Lab 3 : Lustre/Heptagon implementation of Line Follower
```
Submitted by:
- Santanu Sahoo (23m0777)
- Sm Arif Ali (23m0822)
- Soumik Dutta (23m0826)
- Arnab Bhakta (23m0835)
```

### YouTube link
[**Link**](https://www.youtube.com/watch?v=ca1qenzCxME)

## Libraries Needed

### Arduino Libraries Required for OLED
The following libraries are needed to support the OLED display:
1. "Adafruit SSD1306"
2. "Arduino_GFX"
The library folder is included in the submission.
To install them manually, use the following commands:
```bash
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Arduino_GFX"
```
**or comment the `#define DEBUG` line**

## States
The robot operates in several states to follow the line effectively. The key states are:

### 1. Calibrate
- The robot moves in circular motion to determine the minimum and maximum sensor values.
- Runs in circle for some time.
- Switches to the `Idle` state once calibration is complete.

### 2. Idle
- The robot remains stationary.
- It transitions to `Start` once the all-high condition is detected.

### 3. Start
- The robot starts moving forward with a base speed.
- When it detects a black line, it transitions to `WonB`.

### 4. WonB
- This is the primary line-following state.
- The PID controller is used to maintain the robot on the black line.
- If all sensors detect white (line lost), it transitions to `Recovery`.

### 5. Recovery
- The robot has lost the white line.
- The robot moves backward to find the line back.
- Returns to `WonB` once the line is detected again.

## Charge Requirement
- All these motions are tuned on full charge. At lower charge lower speed has less power and the turn etc would not be accurate.