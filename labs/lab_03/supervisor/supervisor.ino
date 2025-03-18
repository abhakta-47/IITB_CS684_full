#include "alphabot_drivers.h"
#include "line_follower.h"

#define NUM_SENSORS 5
int sensorValues[NUM_SENSORS];
Line_follower__main_out _res;

void setup()
{
  init_devices();
  
  Serial.begin(115200);
}

void loop()
{
  AnalogRead(sensorValues);
  Line_follower__main_step(sensorValues[0], sensorValues[1], sensorValues[2], sensorValues[3], sensorValues[4], &_res);
  Serial.print(_res.v_l);
  Serial.print(" ");
  Serial.println(_res.v_r);
  SetSpeed(_res.v_l, _res.v_r);
}
