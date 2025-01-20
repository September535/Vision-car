#define LED 12  //Define LED as 12 pin
void setup() {
  ledcSetup(0, 3000, 8);  //Set pwm channel, frequency and accuracy
  ledcAttachPin(LED, 0);  //Attach the LED port to the ledc channel
}

void loop() {
  for (int i = 0; i < 255; i++) {  //for loop, control i to increase from 0 to 255
    ledcWrite(0, i);               //output pwm to change the brightness of LED
    delay(10);
  }
  for (int i = 255; i > 1; i--) {  //for loop, control i to decrease from 255 to 0
    ledcWrite(0, i);               //output pwm to change the brightness of LED
    delay(10);
  }
}