#include <Wire.h>
#include <math.h> 
int BH1750address = 0x23; //i2c address
 
byte buff[2];
void setup()
{
  Wire.begin();
  Serial.begin(9600);
}
 
void loop()
{
int i;
  uint16_t val1=0;
  uint16_t val2=0;
  int BH1750address1 = 0x23;
  //int BH1750address2 = 0x5C;
//delay(200);
  BH1750_Init(BH1750address1);
  //BH1750_Init(BH1750address2);
  if(2==BH1750_Read(BH1750address1))
  {
    val1=((buff[0]<<8)|buff[1])/1.2;
    Serial.println(val1);
  }
 /* if(2==BH1750_Read(BH1750address2))
  {
    val2=((buff[0]<<8)|buff[1])/1.2;
    Serial.println(val2);
  }*/
}
 
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}
 
void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  //Wire.write(0x10);
  Wire.endTransmission();
}
