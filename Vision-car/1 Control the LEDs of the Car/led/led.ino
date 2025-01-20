#define LED 12  //define pin of LED
void setup() {
  pinMode(LED,OUTPUT);  //set pin of led as output mode
}

void loop() {
  digitalWrite(LED,HIGH);   // pin of LED output high level
  delay(1000);              //delay 1s
  digitalWrite(LED,LOW);    //pin of LED output low level
  delay(1000);              //delay 1s
}