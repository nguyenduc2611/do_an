#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"           
#include <SimpleTimer.h>
SimpleTimer timer;
char auth[] = "46yxK2hp1zGFYC0ITMGkIVOW6ORyyO5V";    //AuthToken copy ở Blynk Project
char ssid[] = "Duc Long Electronics";  //Tên wifi
char pass[] = "linhkienhadong.vn";     //Mật khẩu wifi
float t;
float h;
int recieve = 0;
uint32_t buff[16];
int i = 0;
uint16_t request = 0;
char button_light =0;
char gas = 0;
void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(2000, sendUptime);
}

void sendUptime()
{
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V5, h);
  
}
BLYNK_WRITE(V0)
{
  button_light= param.asInt();
}

void loop()
{
  if(Serial.available() > 0) //Nếu có tín hiệu gửi đến Serial
  {
    char ledState = Serial.read(); //Đọc giá trị gửi
    if((ledState == 'B')&&(recieve == 0))
    {
      recieve = 1;
    }
    else if(recieve == 1)
    {
      buff[i] = ledState;
      i++;
    }
    else
    {
      
    }
    if((recieve == 1)&&(ledState == 'E'))
    {
      t = buff[2];
      h = buff[4];
      recieve = 0;
      i = 0;
    }
    else
    {
      
    }
  }

  if(request == 900)
  {
    Serial.write('S');
    request = 0;
  }
  if(button_light != 0)
  {
    Serial.write(button_light);
  }
  request++;
  Blynk.run();
  timer.run();
}
