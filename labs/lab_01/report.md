**Line-Following Robot Statechart Report**

### 1. Strategy Used
The robot follows a black line on a white surface using five white line sensors. The statechart is designed to interpret sensor readings and determine the appropriate movement (straight, left, or right) to keep the robot on track. The strategy follows these key principles:
- **Sensor Detection**: The robot continuously reads the sensor values to determine its position relative to the black line.
- **Decision Making**: Based on sensor readings, the robot decides whether to move straight, turn left, turn right, or handle intersections.
- **Movement Execution**: The robot adjusts motor velocities and direction based on the detected state.
- **Handling Intersections**: When multiple sensors detect black, the robot determines the correct turn to continue toward the end location.
- **End Detection**: The robot identifies the end of the path when all sensors detect black.
- **Recovery Mechanism**: If the robot moves out of bounds, it performs a 180-degree rotation to reposition itself.

### 2. Assumptions Made
- **Sensor Values**: The white line sensors output values between 0 and 1023, where low values indicate a white surface and high values indicate a black surface.
- **Movement and Distance**: The robot moves a fixed distance per state transition.
- **Intersection Handling**: Specific sensor combinations represent intersections, and predefined turns are used to navigate them.
- **Out-of-Bounds Handling**: If the robot detects an out-of-area condition, it rotates 180 degrees to realign with the track.
- **Motor Control**: The robot uses three outputs to control motion: one for direction (0 = backward, 1 = forward) and two for motor velocities (range: 0 to 100).

### 3. Description of the Statechart
The statechart consists of the following states:
- **Idle**: The initial state, waiting for the first sensor input.
- **Sensor Detection**: Reads sensor values and determines the next movement.
- **Movement States**:
  - **MoveStraight**: Moves forward when the center sensor detects the line.
  - **MoveLeft**: Turns left if the left sensors detect the line.
  - **MoveRight**: Turns right if the right sensors detect the line.
- **Intersection Handling**: Different sensor combinations trigger specific turns.
- **EndLine**: The final state when all sensors detect black.
- **OutOfArea**: If the robot moves out of bounds, it rotates 180 degrees.
- **Stopped**: The robot momentarily stops before resuming sensor detection.
- **Finished**: The final state when the robot successfully reaches the end.

### 4. Inputs and Outputs
The following table summarizes key input conditions and corresponding outputs:

| Sensor Input  | Detected Condition  | Movement Output (Direction, Left Motor, Right Motor) |
|--------------|---------------------|--------------------------------|
| 11111       | End of Path         | (0, 0, 0) - Stop              |
| 00100       | Straight            | (1, 100, 100) - Move Straight |
| 11000       | Left                | (1, 50, 100) - Correct Left   |
| 11100       | Left                | (1, 100, 50) - Turn Right     |
| 00011       | Right Intersection   | (1, 50, 100) - Turn Left      |
| 00111       | Right Intersection  | (1, 50, 100) - Turn Left      |
| 11011       | Left Right | (1, -100, 100) - Rotate 180   |

### 5. Conclusion
The implemented statechart ensures smooth navigation of the robot from start to end while effectively handling intersections and out-of-area scenarios. The strategy focuses on real-time sensor detection, adaptive movement control, and efficient state transitions to maintain the robot’s path on the black line.

