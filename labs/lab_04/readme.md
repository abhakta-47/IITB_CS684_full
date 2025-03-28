# **CS684 :: Lab 4 : Heptagon implementation for Line Switching and Obstacle Navigation**  

**Submitted by:**  
- Santanu Sahoo (23M0777)  
- SM Arif Ali (23M0822)  
- Soumik Dutta (23M0826)  
- Arnab Bhakta (23M0835)  

## **YouTube [[Link]](https://youtu.be/SRNQqQcua1s?si=acKgFLWrAkdW8tvi)**  
<iframe width="560" height="315" src="https://www.youtube.com/embed/SRNQqQcua1s?si=geWlyrC5JoaawTBe" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## **Libraries Required**  
### **Arduino Libraries for OLED Display**  
The following libraries are required for OLED support:  
1. **Adafruit SSD1306**  
2. **Arduino_GFX**  

The library folder is included in the submission. To install them manually, use the following commands:  

```bash
arduino-cli lib install "Adafruit SSD1306"
arduino-cli lib install "Arduino_GFX"
```  

Alternatively, you can comment out the `#define DEBUG` line.  

## **Changes from Lab 3**  
- Improved white line following by tuning weights and parameters.  
- Modified color switch logic to avoid using complement-based calculations.  
- Tuned parameters for more stable black line following.  
- Implemented obstacle avoidance.  
- Implemented intersection navigation.  
- Tuned `kp` for better control.  

## **Relevant Automatons**  
This section explains the mechanisms for **[Color Switch](#color-switch)**, **[Black Line Follow](#black-line-follow)**, **[Obstacle Avoidance](#obstacle-avoidance)**, and **[Intersection Handling](#intersection-handling)**.  

### **Color Switch**  
The **Color Switch** mechanism detects and switches between white and black line-following modes.  

#### **Entry Condition**  
- **From:** WonB  
- **Condition:** Two end sensors detect high (white), while any other sensor detects low (black).  

#### **Internal States**  
1. **Transition**  
   - Triggered when a color change is detected.  
   - If a black line is found, transition to BonW.  
   - If not, attempt to locate it by rotating.  

#### **Exit Condition**  
- **To:** BonW  
- **Condition:** Any sensor detects black.  

#### **Internal State Transition Table**  
_No internal states._  


### **Black Line Follow**  
The **Black Line Follow** mechanism ensures the robot follows a black line on a white surface using PID control.  

#### **Entry Condition**  
- **From:** Transition (Color Switch) or ObstacleAvoid.  
- **Condition:** Any sensor detects black.  

#### **Internal States**  
1. **BonW (Black on White)**  
   - Uses PID error calculation to adjust motor speeds.  

2. **Intersection**  
   - Handles intersection. (explained [later](#intersection-handling)).

| **Current State** | **Condition** | **Next State** |
|------------------|--------------|--------------|
| **BonW** | Three neighboring sensors detect black | **Intersection** |
| **Transition** | Transition complete | **BonW** |

#### **Exit Condition**  
- **To:** ObstacleAvoid  
  - **Condition:** IR sensor detects an obstacle (`ir_value != 0`).  
- **To:** Stop  
  - **Condition:** Fourth intersection detected.  

### **Obstacle Avoidance**  
The **Obstacle Avoidance** mechanism ensures the robot navigates around obstacles and resumes line-following.  

#### **Entry Condition**  
- **From:** BonW (Black Line Follow).  
- **Condition:** IR sensor detects an obstacle (`ir_value != 0`).  

#### **Internal States**  
1. **Wait**  
   - Stops the robot momentarily.  
   - Counts cycles to confirm the obstacle's presence.  

2. **FullRight**  
   - Moves the robot right by ~90°.  
   - Prepares for obstacle avoidance maneuvers.  

3. **SlightRight**  
   - Moves right if the left side is blocked.  
   - Adjusts motor speed for stability.  

4. **SlightLeft**  
   - Moves left if the right side is blocked.  

5. **ExitStart**  
   - Stops movement temporarily before re-entering line-following mode.  

6. **ExitEnd1**  
   - Moves forward when sensors detect the black line.  

7. **ExitEnd2**  
   - Aligns with the black line using sensor feedback.

| **Current State** | **Condition** | **Next State** |
|------------------|--------------|--------------|
| **Wait** | Obstacle still present (`ncycles > 1000`) | **FullRight** |
| **Wait** | Obstacle moved (`ncycles < 1000 and ir_value = 0`) | **BonW** |
| **FullRight** | Obstacle on the left | **SlightRight** |
| **FullRight** | No obstacle, all sensors low | **SlightLeft** |
| **SlightRight** | Obstacle cleared | **SlightLeft** |
| **SlightRight** | Obstacle still present | **FullRight** |
| **SlightRight** / **SlightLeft** | Black line detected | **ExitStart** |
| **ExitStart** | Left sensors detect the line | **ExitEnd1** |
| **ExitStart** | Right sensors detect the line | **ExitEnd2** |
| **ExitEnd1** / **ExitEnd2** | Black line confirmed | **BonW** |

#### **Exit Condition**  
- **To:** BonW (Black Line Follow).  
- **Condition:** Sensors detect the black line.  

### **Intersection Handling**  
The **Intersection Handling** mechanism ensures the robot correctly navigates intersections.  

#### **Entry Condition**  
- **From:** BonW (Black Line Follow).  
- **Condition:** The intersection condition (`inx_cond`) is met.  

#### **Internal States**  
1. **Counter**  
   - Stops the robot momentarily.  
   - Increments the intersection counter.  

2. **Entry**  
   - Moves forward while checking intersection patterns.  
   - Determines if a turn is required.  

3. **ExitRight**  
   - Turns right if all sensors detect white momentarily.  

4. **ExitLeft**  
   - Turns left if only one sensor detects low (black).  

| **Current State** | **Condition** | **Next State** |
|------------------|--------------|--------------|
| **Counter** | Always | **Entry** |
| **Entry** | `all_high = true` | **ExitRight** |
| **Entry** | `only_one_low = true` | **ExitLeft** |

#### **Exit Condition**  
- **To:** BonW (Black Line Follow).  
- **Condition:** Intersection maneuver is completed (`complete = true`).  


## **Charge Requirement**  
- All these motions are tuned for a full battery charge. At lower charge levels, reduced motor power affects speed and accuracy, impacting turns and navigation.  