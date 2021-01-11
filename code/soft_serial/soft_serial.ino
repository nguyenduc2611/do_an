#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 9);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
char Esp_data = 0;
  if(mySerial.available() > 0)
  {
    Esp_data = mySerial.read();
    Serial.println(Esp_data);
  }
}
