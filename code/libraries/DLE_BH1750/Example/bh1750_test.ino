
#include <math.h> 
#include "DLE_BH1750.h"


DLE_BH1750  DucLong_BH1750;

int BH1750address = 0x23; //i2c address
 
unsigned int lightData;

void setup()
{
  Serial.begin(57600);
  if(DucLong_BH1750.begin(BH1750address))
  {
      Serial.println("Initialize module BH1750 successful ");
  }
  
  
}
 
void loop()
{
  if(DucLong_BH1750.Read(&lightData))
  {
    Serial.print(lightData);
    Serial.println(" Lux");
  }
  delay(1000);
}
 
