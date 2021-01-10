#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

#define SERVO 10
#define BUTTON_1 7
#define BUTTON_2 6
#define BUTTON_3 5
#define BUTTON_4 4
#define BUTTON_5 A0
#define BUTTON_6 A1

#define status_b5 analogRead(A0)
Servo myservo;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(SERVO);
  pinMode(BUTTON_1, OUTPUT);
  pinMode(BUTTON_2, OUTPUT);
  pinMode(BUTTON_3, OUTPUT);
  pinMode(BUTTON_4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = status_b5;
  Serial.println(a);
  delay(1000);
}
