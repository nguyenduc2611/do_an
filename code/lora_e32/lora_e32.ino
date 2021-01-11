 
void setup() {
  Serial.begin(9600);
  delay(500);
 
  Serial.println("Hi, I'm going to send message!");
 
  Serial2.begin(9600);
}
 
void loop() {
  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  /*Serial2.println("aa");
  delay(500);*/
}
