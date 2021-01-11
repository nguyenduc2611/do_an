#include <SoftwareSerial.h>
char buff[20];
unsigned char i=0;
unsigned char chieu_sau = 0;
unsigned char banh_rang = 0;
SoftwareSerial mySerial(10, 11);
void setup() {
 Serial.begin(9600);
 mySerial.begin(9600);
}

void loop() {
  receivedata();
}

void receivedata() {
  if(mySerial.available() >  0) {
  char c = mySerial.read();
  Serial.print(c);
  if(c == '*') {
    i = 0;  
  }
  if(c != '*' && c!='#'){
    buff[i++] = c;
  }
  if(c == '#') {
    Serial.print("hello");
    Serial.print(buff);
    Serial.print("hi");
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
    Serial.println(chieu_sau);
    Serial.println(banh_rang);

      memset(buff,0,20);
    } 
  }
}
