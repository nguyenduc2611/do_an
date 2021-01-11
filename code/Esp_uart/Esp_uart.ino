#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
void setup() 
{
  Serial.begin(9600); //Set Baudrate của Serial
  mySerial.begin(9600);
}

void loop()
{
  if(Serial.available() > 0) //Nếu có tín hiệu gửi đến Serial
  {
    char ledState = Serial.read(); //Đọc giá trị gửi
    if(ledState == 'a') //Nếu gửi '1'
    {
      Serial.println(ledState);
    }
  }
  delay(10); //Ngưng 10ms
}
