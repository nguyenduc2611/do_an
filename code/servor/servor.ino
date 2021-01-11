#include <Servo.h> 
 
#define SERVO_PIN 25
#define SERVO_PIN1 10
 
Servo gServo;
Servo gServo1;
 int i;
void setup() 
{ 
  Serial.begin(9600);
    gServo.attach(SERVO_PIN); 
    gServo1.attach(SERVO_PIN1); 
    gServo.write(120);
}
 
void loop() 
{ 

delay(500);

}
