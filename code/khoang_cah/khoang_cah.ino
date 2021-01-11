#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setCursor(0,1);
  lcd.print("Nhap:");
}
