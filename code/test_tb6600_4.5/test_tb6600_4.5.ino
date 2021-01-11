const int stepPin = 5;
const int dirPin = 4;
const int enPin = 13;
void setup() {
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
 
}
void loop() {
  delay(2000);
  digitalWrite(dirPin,LOW);    // clockwise driection
  for(int x = 0; x < 16000; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(2000);
 
}
