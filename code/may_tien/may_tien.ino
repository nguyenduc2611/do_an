#include <TimerOne.h>
#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>
char buff[20];
unsigned char i=0;
unsigned char chieu_sau = 0;
unsigned char banh_rang = 0;
SoftwareSerial mySerial(10, 11);
int x = 0; 
#define EN1 16
#define DIR1 17
#define PUL1 18

#define EN3 19
#define DIR3 20
#define PUL3 21

#define EN2 2
#define DIR2 14
#define PUL2 15 

#define EN4 3
#define DIR4 4
#define PUL4 5

#define RELAY 7
#define BUTTON_STOP  8
#define BUTTON_START 9
char stop_fac = 0;
char flag_start = 0;
/*motor 2: LOW - trái sang phải
 * HIGH - phải sang trái
 * 2800 đến vị trí cắt
 * motor 3: HIGH - trên xuống
 * LOW - dưới lên
 * 100 đến vị trí cắt
 * motor 1: LOW ngoài đẩy vào trong
 * HIGH : ngoài vào trong
 * 500 cat xong
 * motor 4: 8000
 */

void setup() 
{
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);
  Timer1.initialize(500000); 
  Timer1.attachInterrupt(fun_stop);
  pinMode(EN1,OUTPUT); // Enable pin 
  pinMode(PUL1,OUTPUT); // Step pin
  pinMode(DIR1,OUTPUT); // Dir - pin

  pinMode(EN2,OUTPUT); // Enable pin 
  pinMode(PUL2,OUTPUT); // Step pin
  pinMode(DIR2,OUTPUT); // Dir - pin

  pinMode(EN3,OUTPUT); // Enable pin 
  pinMode(PUL3,OUTPUT); // Step pin
  pinMode(DIR3,OUTPUT); // Dir - pin

  pinMode(EN4,OUTPUT); // Enable pin 
  pinMode(PUL4,OUTPUT); // Step pin
  pinMode(DIR4,OUTPUT); // Dir - pin
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  mySerial.begin(9600);
  //pinMode(BUTTON, INPUT);
}

void fun_stop(void)
{
  char button_status = digitalRead(BUTTON_STOP);
  char status_start = digitalRead(BUTTON_START);
  if(status_start == LOW)
  {
    flag_start = 1;
  }
  if(button_status == LOW)
  {
    stop_fac = 1;
  }

  if(stop_fac == 1)
  {
    digitalWrite(EN1,HIGH);
    digitalWrite(EN2,HIGH);
    digitalWrite(EN3,HIGH);
    digitalWrite(EN4,LOW);
    digitalWrite(RELAY, HIGH);
  }
}

void turn_round(int fre, char motor, char dir)
{
  switch(motor)
  {
    case 1:
        if(!stop_fac)
        {
          digitalWrite(EN1,LOW); // Đặt Enable ở trạng thái LOW
          digitalWrite(DIR1,dir); // Đặt Dir  ở trạng thái HIGH
          for(int x = 0; x < fre; x++) // Cho chay 1 vong
          {
            digitalWrite(PUL1,HIGH); // Output high
            delay(8);// chờ
            digitalWrite(PUL1,LOW); // Output low
            delay(8); // chờ
          }
        }
        digitalWrite(EN1,HIGH);
    break;
    case 2:
        if(!stop_fac)
        {
          digitalWrite(EN2,LOW); // Đặt Enable ở trạng thái LOW
          digitalWrite(DIR2,dir); // Đặt Dir  ở trạng thái HIGH
          for(int x = 0; x < fre; x++) // Cho chay 1 vong
          {
            digitalWrite(PUL2,HIGH); // Output high
            delay(1);// chờ
            digitalWrite(PUL2,LOW); // Output low
            delay(1); // chờ
          }
        }
        digitalWrite(EN2,HIGH);
    break;
    case 3:
        if(!stop_fac)
        {
          digitalWrite(EN3,LOW); // Đặt Enable ở trạng thái LOW
          digitalWrite(DIR3,dir); // Đặt Dir  ở trạng thái HIGH
          for(int x = 0; x < fre; x++) // Cho chay 1 vong
          {
            digitalWrite(PUL3,HIGH); // Output high
            delay(1);// chờ
            digitalWrite(PUL3,LOW); // Output low
            delay(1); // chờ
          }
        }
        digitalWrite(EN3,HIGH);
    break;
    case 4:
        if(!stop_fac)
        {
          digitalWrite(EN4,HIGH); 
          digitalWrite(DIR4,dir); // Đặt Dir  ở trạng thái HIGH
          for(x = 0; x < fre; x++) // Cho chay 1 vong
          {
            digitalWrite(PUL4,HIGH); // Output high
            delayMicroseconds(500); // chờ
            digitalWrite(PUL4,LOW); // Output low
            delayMicroseconds(500); // chờ
          }
        }
      digitalWrite(EN4,LOW); 
    break;
    default:
    break;
  }
}

void run_fac(char lat_cat, char banh_rang)
{
  int x = 0;
  int j = 0;
  turn_round(2800, 2, HIGH);
  delay(500);
  if(!stop_fac)
  {
    digitalWrite(RELAY, LOW);
  }
  turn_round(100, 3, HIGH);
  delay(500);
  for(j = 0; j < lat_cat; j++)
  {
    turn_round(12, 3, HIGH);
    delay(500);
    Serial.println("dg");
    for(int k = 0; k < (banh_rang/2); k++)
    {
       turn_round(500, 1, HIGH);
      delay(500);

      turn_round(8000/banh_rang, 4, HIGH); 
      delay(500);

      turn_round(500, 1, LOW);
      delay(500);

      turn_round(8000/banh_rang, 4, HIGH);
      delay(500);
    }
  }
  turn_round(100, 3, LOW);
  delay(500);
  turn_round(j*12, 3, LOW);
  delay(500);
  digitalWrite(RELAY, HIGH);
  turn_round(2800, 2, LOW);
  delay(500);
}

void loop() 
{
  receivedata();
   if(flag_start)
   {
    Serial.println(chieu_sau + 48);
      run_fac(chieu_sau*2,banh_rang);
      stop_fac = 0;
      flag_start = 0;
   } 
   /*delay(10000);
  turn_round(100, 3, LOW);*/
  
}

void receivedata() {
  if(mySerial.available() >  0) {
  char c = mySerial.read();
  if(c == '*') {
    i = 0;  
  }
  if(c != '*' && c!='#'){
    buff[i++] = c;
  }
  if(c == '#') {
    if(buff[1] == ',')
    {
      chieu_sau = buff[0] - '0';
      if(buff[3] == ',')
      {
        banh_rang = buff[2] - '0';
      }
      else if(buff[4] == ',')
      {
        banh_rang = (buff[2]-'0')*10 + buff[3] - '0';  
      }
      else
      {
        
      }
    }
    else if(buff[2] == ',')
    {
      chieu_sau = (buff[0] - '0')*10 + buff[1]-'0';
      if(buff[4] == ',')
      {
        banh_rang = buff[3] - '0';
      }
      else if(buff[5] == ',')
      {
        banh_rang = (buff[3]-'0')*10 + buff[4] - '0';  
      }
    }
    else
    {
      
    }
    Serial.println(chieu_sau + 48);
    Serial.println(banh_rang + 48);
      memset(buff,0,20);
    } 
  }
}
