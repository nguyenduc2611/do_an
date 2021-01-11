#define BLYNK_PRINT Serial
#include <NTPtimeESP.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>


RtcDS3231<TwoWire> rtcObject(Wire);
NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
char *ssid      = "Duc Long Electronics";               // Ten WiFi SSID
char *password  = "linhkienhadong.vn";               // Mat khau wifi
char auth[] = "fpFEAnETEyCzBftHFCXW1AJFTguJAh01";    //AuthToken copy ở Blynk Project
char status_update = 0;
int hour_time1 = 0;
int min_time1 = 0;
int hour_time2 = 0;
int min_time2 = 0;
int hour_time3 = 0;
int min_time3 = 0;
int hour_time4 = 0;
int min_time4 = 0;
char send_time1 = 0;
char send_time2 = 0;
int time1 = 0;
int time2 = 0;
int time3 = 0;
int time4 = 0;
int curent_time = 0;
char status_light = 0;
int time_run = 0;
char status_led = 0;
char mode_time_handle = 0;
char mode_time_auto = 0;
char amount_time = 0;
strDateTime dateTime;
void setup() {
  char t = 0;
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  pinMode(D5, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, password);
  //Wire.begin(D2, D1);
  rtcObject.Begin();
  EEPROM.begin(512);

  hour_time1 = char(EEPROM.read(0));
  min_time1 = char(EEPROM.read(1));
  hour_time2 = char(EEPROM.read(2));
  min_time2 = char(EEPROM.read(3));

  hour_time3 = char(EEPROM.read(4));
  min_time3 = char(EEPROM.read(5));
  hour_time4 = char(EEPROM.read(6));
  min_time4 = char(EEPROM.read(7));

  amount_time = char(EEPROM.read(8));

  mode_time_handle = char(EEPROM.read(9));
  mode_time_auto = char(EEPROM.read(10));


while(t == 10)
{
  dateTime = NTPch.getNTPtime(7.0, 0);
  delay(100);
  RtcDateTime currentTime = RtcDateTime(20, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
  rtcObject.SetDateTime(currentTime);
  t++;
}
  
}

BLYNK_WRITE(V3)
{
  status_led= param.asInt();
}

BLYNK_WRITE(V1)
{
  mode_time_auto= param.asInt();
  EEPROM.write(10, mode_time_auto);
    EEPROM.commit();
  if(mode_time_auto == 1)
  {
    mode_time_handle = 0;
    Blynk.virtualWrite(V5, mode_time_handle);
  }
}

BLYNK_WRITE(V4)
{
  send_time1= param.asInt();
}

BLYNK_WRITE(V2)
{
  send_time2= param.asInt();
}

BLYNK_WRITE(V5)
{
  mode_time_handle= param.asInt();
  EEPROM.write(9, mode_time_handle);
  EEPROM.commit();
  if(mode_time_handle == 1)
  {
    mode_time_auto = 0;
    Blynk.virtualWrite(V1, mode_time_auto);
  }
}

BLYNK_WRITE(V6)
{
  TimeInputParam t(param);
  if(t.hasStartTime())
  {
    hour_time1 = t.getStartHour();
    min_time1 = t.getStartMinute();
  }
  if(t.hasStopTime())
  {
    hour_time2 = t.getStopHour();
    min_time2 = t.getStopMinute();
  }
}

BLYNK_WRITE(V7)
{
  TimeInputParam t(param);
  if(t.hasStartTime())
  {
    hour_time3 = t.getStartHour();
    min_time3 = t.getStartMinute();
  }
  if(t.hasStopTime())
  {
    hour_time4 = t.getStopHour();
    min_time4 = t.getStopMinute();
  }
}

BLYNK_WRITE(V8)
{
  amount_time = param.asInt();
  EEPROM.write(8, amount_time);
  EEPROM.commit();
}

void loop() {

  // Tham so dau tien la Time zone duoi dang floating point (7.0 la VN); 
  // Tham so thu hai la DayLightSaving: 1 voi thoi gian; 2 la thoi gian US (o VN khong su dung)
  dateTime = NTPch.getNTPtime(7.0, 0);
  
  // Kiem tra dateTime.valid truoc khi dua ra gia tri thoi gian
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Gio
    byte actualMinute = dateTime.minute;  // Phut
    byte actualsecond = dateTime.second;  // Giay
    int actualyear = dateTime.year;       // Nam
    byte actualMonth = dateTime.month;    // Thang
    byte actualday =dateTime.day;         // Ngay
    byte actualdayofWeek = dateTime.dayofWeek;
  }


  if(mode_time_handle == 1)
  {
    if(status_led == 0)
    {
      status_light = 0;
    }
    else
    {
      status_light = 1;
    }
  }
  if(mode_time_auto == 1)
  {
    if(send_time1 == 1)
  {
    EEPROM.write(0, hour_time1);
    EEPROM.write(1, min_time1);
    EEPROM.write(2, hour_time2);
    EEPROM.write(3, min_time2);
    EEPROM.commit();
    send_time1 = 0;
    Blynk.virtualWrite(V4, send_time1);
  }

  if(send_time2 == 1)
  {
    EEPROM.write(4, hour_time3);
    EEPROM.write(5, min_time3);
    EEPROM.write(6, hour_time4);
    EEPROM.write(7, min_time4);
    EEPROM.commit();
    send_time2 = 0;
    Blynk.virtualWrite(V2, send_time2);
  }
  
  RtcDateTime real_time = rtcObject.GetDateTime();
  curent_time = 60 * real_time.Hour() + real_time.Minute();

time1 = 60 * hour_time1 + min_time1;
time2 = 60 * hour_time2 + min_time2;
time3 = 60 * hour_time3 + min_time3;
time4 = 60 * hour_time4 + min_time4;
if(((time3 != 0)||(time4!=0))&&((time1 != 0)||(time2!=0)))
{
  if(amount_time == 1)
  {
    if((curent_time > time1)&&(curent_time <= time2))
    {
      status_light = 1;
    }
    else
    {
      status_light = 0;
    }
  }
  else if(amount_time == 2)
  {
    if(((curent_time > time3)&&(curent_time <= time4))||((curent_time > time1)&&(curent_time <= time2)))
    {
      status_light = 1;
    }
    else
    {
      status_light = 0;
    }
  }
  else
  {
    
  }
}
  
  }

  


  if((status_update == 0)&&(17 == dateTime.hour))
  {
    RtcDateTime currentTime = RtcDateTime(20, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
    rtcObject.SetDateTime(currentTime);
    status_update = 1;
  }
  else if ((status_update == 1)&&(17 != dateTime.hour))
  {
    status_update = 0;
  }
  else
  {
    
  }
  

  if(status_light == 1)
  {
    Blynk.virtualWrite(V0, 1);
    digitalWrite(D5, HIGH);
    Serial.println("ON");
  }
  else
  {
    Blynk.virtualWrite(V0, 0);
    digitalWrite(D5, LOW);
    Serial.println("OFF");
  }
  delay(500);
  Blynk.run();
}
