#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define GAS_PIN A0
#define RF_PIN 6
#define TRIG_PIN 12
#define ECHO_PIN 11
#define DS18B20_PIN 5
#define HN_PIN 3
#define SPEAKER_PIN 13
#define DOOR_PIN1 4
#define DOOR_PIN2 7
#define FAN_PIN 8

#define CLOSE 0
#define OPENING 1
#define CLOSING 2
#define OPEN 3
OneWire oneWire(DS18B20_PIN);
SoftwareSerial simSerial(9, 10);
DallasTemperature sensors(&oneWire);

char flag_door = CLOSE;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  pinMode(SPEAKER_PIN,OUTPUT);
  pinMode(RF_PIN,INPUT);
  pinMode(HN_PIN,INPUT);
  pinMode(DOOR_PIN1,OUTPUT);
  pinMode(DOOR_PIN2,OUTPUT);
  pinMode(FAN_PIN,OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
  digitalWrite(FAN_PIN, HIGH);
  digitalWrite(DOOR_PIN1, HIGH);
  digitalWrite(DOOR_PIN2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int dis = khoang_cach();
  if(dis <= 5)
  {
    digitalWrite(SPEAKER_PIN, HIGH);
    delay(3000);
    digitalWrite(SPEAKER_PIN, LOW);
  }

  int temp = temp_ds18b20();
  int gas = analogRead(GAS_PIN);
  Serial.println(gas);
  if((temp > 40)&&(gas < 600))
  {
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(SPEAKER_PIN, HIGH);
    send_sms();
  }

  char flag_rf = digitalRead(RF_PIN);
  if(flag_rf == HIGH)
  {
    Serial.println("hello");
    switch(flag_door)
    {
      case CLOSE:
        flag_door = OPENING;
        Serial.println("open");
      break;
      case OPEN:
        flag_door = CLOSING;
        Serial.println("close");
      break;
      default:
      break;
    }
  }

  if(flag_door == OPENING)
  {
    digitalWrite(DOOR_PIN1, HIGH);
    for(int i = 0; i < 10;i++)
    {
    digitalWrite(DOOR_PIN2, LOW);
    delay(10);
    digitalWrite(DOOR_PIN2, HIGH);
    delay(100);
    }
    
    flag_door = OPEN;
  }
  else if(flag_door == CLOSING)
  {
    digitalWrite(DOOR_PIN2, HIGH);
    for(int i = 0; i < 3 0;i++)
    {
      char hn = digitalRead(HN_PIN);
      if(hn == LOW)
      {
        Serial.println("co");
        digitalWrite(DOOR_PIN1, HIGH);
      }
      else
      {
        digitalWrite(DOOR_PIN1, LOW);
        delay(10);
        digitalWrite(DOOR_PIN1, HIGH);
        delay(100);
      }
    }
    flag_door = CLOSE;
  }
  else
  {
    
  }
  //chờ 1 s rồi đọc để bạn kiệp thấy sự thay đổi
  //delay(1000);
}

int temp_ds18b20(void)
{
  int temp = 0;
  sensors.requestTemperatures();  
  temp = sensors.getTempCByIndex(0); // vì 1 ic nên dùng 0
  Serial.println("temp:");
  Serial.println(temp);
  return temp;
}

int khoang_cach(void)
{
  unsigned long duration;
  int distance = 0;
    digitalWrite(TRIG_PIN,0);   // tắt chân tri
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN,1);   // phát xung từ chân tri
    delayMicroseconds(5);   // xung có độ dài 5 microSecond
    digitalWrite(TRIG_PIN,0);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = int(duration/2/29.412);
    Serial.println("distance:");
    Serial.println(distance);
    return distance;
}

void send_sms(void)
{
    simSerial.begin(9600);
    simSerial.write("AT");
    simSerial.write(10);
    delay(1000);
    simSerial.write("AT+CMGF=1");
    simSerial.write(10);
    delay(1000);
    simSerial.write("AT+CMGS=");
    simSerial.write(34);
    simSerial.write("0852909331");
    simSerial.write(34);
    simSerial.write(10);
    delay(1000);
    simSerial.write("chay");
    delay(1000);
    simSerial.write(26);
    delay(1000);
}
