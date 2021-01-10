#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "string.h"
#include "stdlib.h"
#include <LiquidCrystal_I2C.h>

#define MOTOR 8
#define BUTTON 9
SoftwareSerial mySerial(10, 11);

LiquidCrystal_I2C lcd(0x27,16,2);
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
char status_sms = 0;
float lo = 0;
float la = 0;
char buff_sms[255] = {0};
char content_sms[100] = {0};
char sim_AT[100] = {0};
char STD[12] = {0};
int count = 0;
int c = 0;
char flag_content = 0;
char flag_location = 0;
char flag2 = 0;
char count_gps = 0;
char status_button = 0;
char warning = 0;
void setup() {
  delay(15000);
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
    /*Sim module*/
    Serial.println("ret");
  mySerial.begin(9600);
  delay(500);
  mySerial.write("AT+CMGF=1");
  mySerial.write(10);
  delay(500);
  mySerial.write("AT+CNMI=2,2");
  mySerial.write(10);
  delay(3000);
  while(mySerial.available())
  {
    sim_AT[c] = mySerial.read();
    Serial.println(sim_AT[c]);
    c++;
  }
  pinMode(MOTOR, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(MOTOR, HIGH);
  mySerial.begin(9600);
}

void loop() {

  int ancol = analogRead(A1);
  //Serial.println(ancol);
  if(ancol < 900)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Canh Bao");
    status_button = 0;
    send_sms(0);
    warning = 1;
  }

  if((HIGH == digitalRead(BUTTON))&&(!status_button))
  {
    status_button = 1;
    delay(500);
  }
  else if((HIGH == digitalRead(BUTTON))&&(status_button))
  {
    status_button = 0;
    delay(500);
  }
  else
  {
    
  }

  if(status_button)
  {
    digitalWrite(MOTOR, LOW);
  }
  else
  {
    digitalWrite(MOTOR, HIGH);
  }
  if(warning)
  {
    sms_rev();
    while((flag_content)&&(count_gps < 100))
    {
      Serial.println("doc");
      ss.begin(GPSBaud);
      printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
      printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
      Serial.println("lo");
      Serial.println(lo);
      delay(100);
      count_gps++;

      count = 0;
      if(lo!= 0)
      {
        Serial.println("gui");
        send_sms(1);
        for(int l = 0; l < 12;l++)
        {
          STD[l] = 0;
        }
        flag_content = 0;
        count = 0;
        warning = 0;
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Welcome!");
      }
    }
    count_gps = 0;
  }


}
void sms_rev(void)
{
  char j, p, k = 0;
  char flag = 0;
  char flag_std = 0;
  count = 0;
  while(mySerial.available()>0)
  {
    buff_sms[count] = mySerial.read();
    Serial.print(buff_sms[count]);
    Serial.println(buff_sms[count]);
    count++;
    delay(10);
    
  }
  if(count > 0)
  {
    Serial.println("count");
    Serial.println(count);
    for(int g = 0; g < count;g++)
    {
      if((buff_sms[g] == '8')&&(buff_sms[g+1] == '4'))
      {
        flag_std = 1;
      }
      if((flag_std)&&(buff_sms[g] == 34))
      {
        flag_std = 0;
      }
      if(flag_std)
      {
        if(buff_sms[g] > 47)
        {
        STD[k] = buff_sms[g];
        k++;
        }
      }

      if((buff_sms[g] == 'L')&&((buff_sms[g+7] == 'n')))
      {
         flag_content = 1;
         break;
      }

      if(flag)
      {
        content_sms[j] = buff_sms[g];
        Serial.print(content_sms[j]);
        j++;
      }
    }
    Serial.println(content_sms);
    Serial.println(STD);
    /*while(flag)
    {
      Serial.println("get content");
      flag_content = 1;
      flag = 0;
    }*/
  }

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
    while (len-- > 1); //show len debug kí tu *
  }
  else //bat dc toa do
  {
    lo = gps.location.lng();
    la = gps.location.lat();
  }
  smartDelay(0);
}

void send_sms(char TH)
{
    mySerial.begin(9600);
    Serial.println("SEND ok");
    mySerial.write("AT");
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGF=1"); //cai dat module sim che do nhan tin
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGS="); //batdau cu phap text(AT+CMGS="sdt"
    mySerial.write(34);
    if(TH)
    {
    mySerial.write('+');
    mySerial.write(STD);
    }
    else
    {
      mySerial.write("0963011752");
    }
    mySerial.write(34);
    mySerial.write(10); //dau enter
    delay(1000);
    if(TH)
    {
      mySerial.print(gps.location.lng(),6); //so nguyen toa do
      mySerial.write(",");
      mySerial.print(gps.location.lat(),6);
    }
    else
    {
      mySerial.print("Canh Bao Nong Do Con Cao");
    }
    delay(1000);
    mySerial.write(26); //ket thuc text =send
}
