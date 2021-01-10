#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <BlynkSimpleSerialBLE.h>

#define BLYNK_USE_DIRECT_CONNECT
#define TIN_HIEU1 9
#define TIN_HIEU2 12
#define RELAY 8
#define BUTTON 7
#define LED 5
char blynk = 0;
SoftwareSerial mp3Serial(10, 11); // RX, TX
SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int temp_sum = 0;
int temp = 0;
int count = 0;
char flag_check = 0;
char flag_connect = 0;
char auth[] = "VCcsBHp523sCrMD--YHXj_MnqueTjrk9";
void setup() {
  Serial.begin(9600);
  //mySerial.begin (9600);
  //Blynk.begin(mySerial, auth);
  mp3_set_serial (mp3Serial);  //set softwareSerial for DFPlayer-mini mp3 module 
  mp3_set_volume (15);
  mlx.begin();  
  pinMode(TIN_HIEU1, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(TIN_HIEU2, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {

  int temp_human = 0;
  int chuc = 0;
  int dvi = 0;
  char tin_hieu2 = digitalRead(TIN_HIEU2);
  
  if(tin_hieu2 == LOW)
  {
    mp3Serial.begin (9600);
    if(!flag_check)
    {
      mp3_play (17);
      delay(2000);
      flag_check = 1;
    }
    temp_human = mlx.readObjectTempC();
    temp_sum = temp_sum + temp_human;
    count++;
    if(count == 100)
    {
      //Blynk.begin(mySerial, auth);
      temp = temp_sum/100;
      temp = temp + 4;
      if(blynk)
      {
        mySerial.begin (9600);
        Blynk.begin(mySerial, auth);
        Blynk.virtualWrite(V0, temp);
      }
      if(temp < 35)
      {
        mp3_play (14);
        delay(2500);
      }
      else if(temp > 37)
      {
        mp3_play (18);
        delay(3000);
      }
      else
      {
      chuc =  temp/10;
      dvi = temp - chuc*10;
      mp3_play (12);
      delay(2500);
      speak(chuc,1);
      Serial.println(chuc);
       delay(2000);
      speak(dvi,0);
      Serial.println(dvi);
       delay(1800);
      mp3_play (13);
       delay(2000);
       flag_check = 0;
      }
       count  = 0;
       temp_sum = 0;
    }
    delay(50);
  }
  
  char a = digitalRead(TIN_HIEU1);
  if(a == LOW)
  {
    digitalWrite(RELAY, HIGH);
    delay(50);
    digitalWrite(RELAY, LOW);
    delay(10000);
  }

if(Blynk.connected())
  {
    blynk = 1;
    digitalWrite(LED, LOW);
    if(!flag_connect)
    {
      mp3Serial.begin (9600);
      mp3_play (16);
      delay(2500);
      flag_connect = 1;
    }
  }
  else
  {
    flag_connect = 0;
    digitalWrite(LED, HIGH);
    blynk = 0;
  }
  char btn = digitalRead(BUTTON);
  if(HIGH == btn)
  {
    mp3Serial.begin (9600);
    mp3_play (15);
    delay(2500);
    Serial.println("hello");
    mySerial.begin (9600);
    Blynk.begin(mySerial, auth);
  }
  
  if(blynk)
  {
    Blynk.run();
  }
}

void speak(int so, int flag)
{
  if(!flag)
  {
  switch (so)
  {
    case 1:
      mp3_play (1);
    break;
    case 2:
      mp3_play (2);
    break;
    case 3:
      mp3_play (3);
    break;
    case 4:
      mp3_play (4);
    break;
    case 5:
      mp3_play (5);
    break;
    case 6:
      mp3_play (6);
    break;
    case 7:
      mp3_play (7);
    break;
    case 8:
      mp3_play (8);
    break;
    case 9:
      mp3_play (9);
    break;
    default:
    break;
  }
  }
  if(flag)
  {
    Serial.println("hello");
  switch (so)
  {
    case 1:
      mp3_play (10);
    break;
    case 2:
      mp3_play (20);
    break;
    case 3:
      mp3_play (30);
    break;
    case 4:
      mp3_play (40);
    break;
    case 5:
      mp3_play (50);
    break;
    case 6:
      mp3_play (60);
    break;
    case 7:
      mp3_play (70);
    break;
    case 8:
      mp3_play (80);
    break;
    case 9:
      mp3_play (90);
    break;
    default:
    break;
  }
  }
}
