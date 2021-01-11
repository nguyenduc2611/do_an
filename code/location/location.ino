#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "string.h"
#include "stdlib.h"

SoftwareSerial mySerial(2, 3);

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
char status_sms = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
float lo = 0;
float la = 0;
void setup() {
  /*Sim module*/
 mySerial.begin(9600);
 /*GPS module*/
 ss.begin(GPSBaud);
 Serial.begin(9600);
}

void loop() {
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  Serial.println(" ");
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  Serial.println(" ");
  smartDelay(1000);
  if(status_sms == 1)
  {
    send_sms();
    status_sms = 2;
  }
}
void send_sms(void)
{
  int lng_pos = lo;
  long dot_lng_pos = (lo - (float)lng_pos)*10*10*10*10*10*10;
  int lat_pos = la;
  long dot_lat_pos = (la - (float)lat_pos)*10*10*10*10*10*10;
    mySerial.begin(9600);
    mySerial.write("AT");
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGF=1"); //cai dat module sim che do nhan tin
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGS="); //batdau cu phap text(AT+CMGS="sdt"
    mySerial.write(34);
    mySerial.write("0353954090");
    mySerial.write(34);
    mySerial.write(10); //dau enter
    delay(1000);
    mySerial.write("https://www.google.com/maps/place/"); //van ban text
    mySerial.print(lng_pos); //so nguyen toa do
    mySerial.write(".");
    mySerial.print(dot_lng_pos); //phan thuc
    mySerial.write(",");
    mySerial.print(lat_pos); //so nguyen
    mySerial.write(".");
    mySerial.print(dot_lat_pos);
    delay(1000);
    mySerial.write(26); //ket thuc text =send
    ss.begin(GPSBaud);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid) //neu 0 bt dc toa do
  {
    while (len-- > 1) //show len debug kí tu *
      Serial.print('*');
    Serial.print(' ');
  }
  else //bat dc toa do
  {
    if(status_sms == 0) //chua gui tin nhan
    {
      t1 = millis(); //thoi gian tai lan gui 1
      status_sms = 1; //send
    }
    else if((t2 - t1) > 60000) 
    {
      t1 = t2;
      status_sms = 1;
    }
    else
    {
      
    }
    t2 = millis();
    Serial.print(val, prec);
    lo = gps.location.lng();
    la = gps.location.lat();
  }
  smartDelay(0);
}



void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
