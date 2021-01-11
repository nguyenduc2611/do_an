#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
float avr = 0;
float show = 0;
int i = 0;
void setup() {
  // put your setup code here, to run once:
lcd.begin();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
            lcd.setCursor(0,0);
            lcd.print("Nong do con:");
            float analog_record = analogRead(A1);
            Serial.println(analog_record);
            float map_analog = map(analog_record, 250, 1023, 1, 100);
            float ND = map_analog/100;
           Serial.println(ND);
            if(ND > 0)
            {
              lcd.setCursor(4,1);
              lcd.print(ND);
              lcd.print("mg/L");
            }
            else
            {
              lcd.setCursor(4,1);
              lcd.print("0.00");
              lcd.print("mg/L");
            }
            Serial.println(analog_record);
            delay(1000);
}
