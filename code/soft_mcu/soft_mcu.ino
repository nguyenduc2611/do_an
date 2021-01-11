#include <SoftwareSerial.h>
SoftwareSerial mySerial(D6, D5);
void setup() {
  // put your setup code here, to run once:
mySerial.begin(9600);
Serial.begin(9600);
}

void loop() {
  char Esp_data = 0;
  // put your main code here, to run repeatedly:
mySerial.print("AAAD");
delay(1000);
}
