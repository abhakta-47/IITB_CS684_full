# Line-Following Robot Statechart Report

## Strategy Used
The line-following robot follows a black line on a white surface using five white line sensors. The strategy involves continuously reading sensor values and determining the appropriate movement based on predefined patterns. The robot executes movements such as going straight, turning left, turning right, and handling intersections based on the sensor readings. If the robot moves out of the designated area, it performs a 180-degree turn to reorient itself. The robot stops when it detects the end of the line.

## Assumptions Made
- The track consists of a single continuous black line on a white background.
- The five sensors are evenly spaced along the front of the robot.
- Sensor readings are binary, where `1` indicates black (line detected) and `0` indicates white (no line detected).
- The robot can execute movement commands such as moving straight, turning left, and turning right with minimal delay.
- The robot detects intersections and handles them based on predefined transition rules.
- The robot stops when all sensors detect black (`11111`), indicating the end of the track.
- If all sensors detect white (`00000`), the robot has moved out of bounds and performs a 180-degree turn.

## Description of the Statechart
The statechart defines the various states of the robot and their transitions based on sensor inputs. It consists of the following main states:

1. **Idle**: The initial state where the robot waits for the start command.
2. **Sensor Detection**: The robot reads sensor values and determines the next movement.
   - The robot moves straight if the center sensor is active (`00100`).
   - It turns left if the leftmost sensors detect the line (`11000`).
   - It turns right if the rightmost sensors detect the line (`00011`).
   - If both left and right sensors detect the line (`11011`), it prioritizes turning left.
   - The robot stops when all sensors detect black (`11111`).
   - If all sensors detect white (`00000`), it rotates 180 degrees to reorient itself.
3. **Movement States**: The robot executes movement commands based on sensor readings.
   - **Move Straight**: Both motors move forward.
   - **Move Left**: The left motor stops while the right motor moves forward.
   - **Move Right**: The right motor stops while the left motor moves forward.
   - **Rotate 180**: The robot performs a 180-degree turn to reposition itself.
4. **Stopped**: The robot stops after completing a movement cycle and transitions back to sensor detection.
5. **Finished**: The final state where the robot stops completely after reaching the end of the track.

## Input-Output Table
| Sensor Input | Meaning                | Robot Action       |
|-------------|-----------------------|--------------------|
| 00100       | Line detected at center | Move Straight     |
| 11000       | Line detected on left  | Move Left         |
| 00011       | Line detected on right | Move Right        |
| 11011       | Line detected on both sides | Move Left |
| 00111       | Straight and right turn | Move Straight |
| 11100       | Straight and left turn | Move Left |
| 11111       | End of track detected | Stop             |
| 00000       | Out of bounds         | Rotate 180        |

This report summarizes the implemented statechart and describes the logic used for the line-following robot. The approach ensures efficient navigation along the track while handling intersections and edge cases effectively.

