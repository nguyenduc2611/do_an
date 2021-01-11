#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
 
// only used if we are using SPI to communicate with sensor
 
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
 
// Here we are using IIC
 
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);
 
void setup() {
Serial.begin(9600);

Serial.println(F("BMP280 test"));
lcd.begin();
 
while (!bmp.begin(0x76)) {
Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
//while (1);
}
}
 
void loop() {
  lcd.setCursor(0,0);
lcd.print(F("Temp = "));
lcd.print(bmp.readTemperature());
lcd.print(" *C ");

 lcd.setCursor(0,1);
 uint32_t p = bmp.readPressure();
 
lcd.print(F("Pre = "));
lcd.print(p);
lcd.print(" Pa ");
 
}
