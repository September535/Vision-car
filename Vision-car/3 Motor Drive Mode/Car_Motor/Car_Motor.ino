#include "SetMotor.h"  

void setup() {

  i2c_init();  // IIC init
}

void loop() {

  Car_forward(100);  // forward
  delay(2000);       

  Car_backwards(100);  // backward
  delay(2000);        

  Car_left(100);       // left
  delay(2000);       

  Car_right(100);      // right
  delay(2000);       

  Car_stop();         // stop
  delay(1000);       
}