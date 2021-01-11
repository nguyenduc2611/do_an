#include <Servo.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Adafruit_Fingerprint.h>
int led = 13; //Chân led
Servo myservo;
SoftwareSerial mySerial(2, 3);
void setup() 
{
  pinMode(led, OUTPUT); //Định nghĩa chân led là OUTPUT
  Serial.begin(9600); //Set Baudrate của Serial
  mySerial.begin(9600);
  myservo.attach(5);
  myservo.write(90);
}

void loop()
{
  if(Serial.available() > 0) //Nếu có tín hiệu gửi đến Serial
  {
    char ledState = Serial.read(); //Đọc giá trị gửi
    if(ledState == '1') //Nếu gửi '1'
    {
      Serial.println(ledState);
      myservo.write(0);
      delay(3000);
      myservo.write(90);
      ledState = 0;
    }
  }
  delay(10); //Ngưng 10ms
}
