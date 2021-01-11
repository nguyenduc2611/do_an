#include <SoftwareSerial.h>
SoftwareSerial my(3, 2);
void setup() {
  // put your setup code here, to run once:
my.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
my.println("AS");
delay(1000);
}
