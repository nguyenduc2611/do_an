// Arduino Energy Meter V2.0
// This code is for This code is for Wemos(ESP8266) based Energy monitoring Device
// This code is a modified version of sample code from https://github.com/pieman64/ESPecoMon
// Last updated on 30.05.2018
 
//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27,16,2);

char auth[]       = "vnFeI5vB0bymOOP26GANZe9pe0WFnpPv";
char ssid[]       = "BCHTS";
char pass[]       = "12345678";

const int Sensor_Pin = A0;
unsigned int Sensitivity = 185;   // 185mV/A for 5A, 100 mV/A for 20A and 66mV/A for 30A Module
float Vpp = 0; // peak-peak voltage 
float Vrms = 0; // rms voltage
float Irms = 0; // rms current
float Supply_Voltage = 233.0;           // reading from DMM
float Vcc = 5.0;         // ADC reference voltage // voltage at 5V pin 
float power = 0;         // power in watt              
float Wh =0 ;             // Energy in kWh
unsigned long last_time =0;
unsigned long current_time =0;
unsigned long interval = 100;
unsigned int calibration = 100;  // V2 slider calibrates this
unsigned int pF = 85;           // Power Factor default 95
float bill_amount = 0;   // 30 day cost as present energy usage incl approx PF 
unsigned int energyTariff = 8.0; // Energy cost in INR per unit (kWh)
char button = 0;

void getACS712() {  // for AC
  Vpp = getVPP();
  Vrms = (Vpp/2.0) *0.707; 
  Vrms = Vrms - (calibration / 10000.0);     // calibtrate to zero with slider
  Irms = (Vrms * 1000)/185 ;
  if((Irms > -0.015) && (Irms < 0.008)){  // remove low end chatter
    Irms = 0.0;
  }
  if(Irms < 0.1)
  {
    Irms = 0;
  }
  power= (Supply_Voltage * Irms) * (pF / 100.0); 
  last_time = current_time;
  current_time = millis();    
  Wh = Wh+  power *(( current_time -last_time) /3600000.0) ; // calculating energy in Watt-Hour
  Serial.println(String(Wh, 3)); 
  Blynk.virtualWrite(V0, String (Wh));  // gauge 
}

float getVPP()
{
  float result; 
  int readValue;                
  int maxValue = 0;             
  int minValue = 1024;          
  uint32_t start_time = millis();
  while((millis()-start_time) < 950) //read every 0.95 Sec
  {
     readValue = analogRead(Sensor_Pin);    
     if (readValue > maxValue) 
     {         
         maxValue = readValue; 
     }
     if (readValue < minValue) 
     {          
         minValue = readValue;
     }
  } 
   result = ((maxValue - minValue) * Vcc) / 1024.0;  
   return result;
 }

BLYNK_WRITE(V1)
{
  button= param.asInt();
}

void setup() { 
  Serial.begin(115200); 
  Wire.begin(D2, D1);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, getACS712); // get data every 2s
  pinMode(D4, OUTPUT);
  //digitalWrite(D4, LOW);
  lcd.begin();
  lcd.home();
  lcd.setCursor(0,0);
  lcd.print("Luong Dien Tieu Thu");
}


void loop() {


  lcd.setCursor(4,1);
  lcd.print(Wh); 
  lcd.print("W/h");


  if(button == 1)
  {
    digitalWrite(D4, LOW);
  }
  else
  {
    digitalWrite(D4, HIGH);
  }
  Blynk.run();
  timer.run();
 
}
