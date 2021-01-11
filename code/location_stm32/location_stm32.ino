#include <LiquidCrystal.h>     //Library for using LCD display functions                      
#include <TinyGPS++.h>         //Library for using GPS functions

const int rs = PB11, en = PB10, d4 = PB0, d5 = PB1, d6 = PC13, d7 = PC14;  //LCD pins connected with STM32
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

TinyGPSPlus gps;               //Object gps for class TinyGPSPlus

void setup()
{
  Serial1.begin(9600);         //Begins Serial comunication at Serial Port 1 at 9600 baudrate
  lcd.begin(16,2);             //Sets display in 16x2 Mode
  delay(4000);
  lcd.clear();      
}

void loop()
{
  GPSDelay(1000);
  double lat_val, lng_val;
  bool loc_valid;
  lat_val = gps.location.lat();        //Gets the latitude
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();        //Gets the longitude
 
  if (!loc_valid)                     //Excecutes when invalid data is received from GPS
  {
    lcd.setCursor(4,0);
    lcd.print("Waiting");
    delay(4000);
    lcd.clear();
  }
  else                              //Excutes when valid data is received from GPS
  {
    lcd.clear();
   
    lcd.setCursor(0,0);
    lcd.print("LAT:  ");
    lcd.print(lat_val,6);         //Prints latitude at LCD display

    lcd.setCursor(0,1);
    lcd.print("LONG:");
    lcd.print(lng_val,6);          //Prints longitude at LCD display

    delay(4000);
  }
}

static void GPSDelay(unsigned long ms)          //Delay for receiving data from GPS
{
  unsigned long start_time = millis();
  do
  {
    while (Serial1.available()) 
    gps.encode(Serial1.read());
  } while (millis() - start_time < ms);
}
