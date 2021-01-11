#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"           
#include <SimpleTimer.h>

#define LIGHT_ON 10
#define LIGHT_OFF 11
#define AUTO 8
#define MANUAL 9

int status_light = 0;

SimpleTimer timer;
char auth[] = "cudKhdUaaUuvJNnbhN8HfgD98Qw6MQVB";    //AuthToken copy ở Blynk Project
char ssid[] = "Duc Long Electronics";  //Tên wifi
char pass[] = "Duc Long Electronics";     //Mật khẩu wifi
float t;
float h;
int recieve = 0;
uint32_t buff[16];
int i = 0;
uint32_t request = 0;
char button_auto =0;
char button_manual =0;
char gas = 0;
char light_on = 0;
uint32_t Wh = 0;
uint32_t lux1 = 0;
uint32_t lux2 = 0;
WidgetLCD lcd(V7);
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
  if(gas == 1)
  {
    lcd.print(3, 0, "Nong Do Khi");
    lcd.print(3, 1, "Binh Thuong");
  }
  else if(gas == 2)
  {
    lcd.print(3, 0, "Nong Do Khi");
    lcd.print(3, 1, "Bao Dong   ");
  }
  else if(gas == 3)
  {
    lcd.print(3, 0, "Nong Do Khi");
    lcd.print(3, 1, "Co Chay    ");
  }
  else
  {
    
  }
  Blynk.virtualWrite(V8, Wh);
  Blynk.virtualWrite(V9, lux1);
  Blynk.virtualWrite(V10, lux2);
}
BLYNK_WRITE(V0)
{
  button_auto= param.asInt();
}

BLYNK_WRITE(V1)
{
  button_manual= param.asInt();
}

BLYNK_WRITE(V2)
{
  //light1_on= param.asInt();
}

BLYNK_WRITE(V3)
{
  //light2_on= param.asInt();
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
      t = buff[1];
      h = buff[3];
      gas = buff[5];
      Wh = buff[7];
      lux1 = buff[9];
      lux2 = buff[11];
      recieve = 0;
      i = 0;
    }
    else
    {
      
    }
  }

  if(request == 90000)
  {
    Serial.write('S');
    request = 0;
  }
  if(button_auto == 1)
  {
    Serial.write(AUTO);
  }
  else if(button_manual == 1)
  {
    Serial.write(MANUAL);
  }
  else if((status_light == MANUAL)&&(light_on == 1))
  {
    Serial.write(LIGHT_ON);
  }
  else if((status_light == MANUAL)&&(light_on == 0))
  {
    Serial.write(LIGHT_OFF);
  }
  else
  {
    
  }
  request++;
  Blynk.run();
  timer.run();
}
