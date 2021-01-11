#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.

char auth[] = "KEqF-bpkeu09pARTIRPtDMw6g48DfYue";//Enter your Auth token here

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Duc Long Electronics";           //SSID
char pass[] = "linhkienhadong.vn";      //Password
char i = 0;
int buff[6] = {0};
char d,c = 0;
SimpleTimer timer;
WidgetLCD lcd(V7);
void setup()
{
  // Debug console
  Serial.begin(9600); //ESP8266 boards

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  timer.setInterval(2000, sendUptime);
  //Blynk.begin(auth, ssid, pass, IPAddress(XXX,XXX,X,XX), 8080);
}

void sendUptime()
{
  Blynk.virtualWrite(V6, d);
  Blynk.virtualWrite(V5, c);

  if(c == 2)
  {
    lcd.print(3, 0, "Khong chay");
  }
  else
  {
    lcd.clear();
    lcd.print(3, 0, "co chay");
  }

  if(d < 10)
  {
    lcd.print(3, 1, "co trom");
  }
  else
  {
    lcd.print(3, 1, "an toan");
  }
}

void loop()
{
  
  if(Serial.available() > 0)
  {
    buff[i] = Serial.read();
    i++;
  }
  if(buff[i-1] == 'Z')
  {
    if(i == 4)
    {
      d = 0;
      c = buff[2];
    }
    else
    {
      d = buff[1];
      c = buff[3];
    }
    i = 0;
  }
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
