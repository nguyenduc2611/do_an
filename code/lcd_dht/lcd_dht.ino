#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int DHTPIN = 2;
int PUMP = 8;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000, 
  0B00000,
  0B00000
};

void setup() {
  pinMode(PUMP, OUTPUT);
  lcd.begin();  
  lcd.backlight();
  
  lcd.print("Nhiet do: ");
  lcd.setCursor(0,1);
  lcd.print("Doam:");
  
  lcd.createChar(1, degree);

  dht.begin();  
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int voltage = analogRead(A0);
  float threshold = 0;
  int a = 0;
  delay(200);
  threshold = (float(voltage)/1024)*100;
  a = threshold;
  if(h <= a)
  {
    digitalWrite(PUMP, HIGH);
  }
  else
  {
    digitalWrite(PUMP, LOW);
  }
  if (isnan(t) || isnan(h)) { // Kiểm tra xem thử việc đọc giá trị có bị thất bại hay không
  } 
  else {
    lcd.setCursor(10,0);
    lcd.print(round(t));
    lcd.print(" ");
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(5,1);
    lcd.print(round(h));
    lcd.setCursor(7,1);
    lcd.print("% ");   
    lcd.setCursor(9,1); 
    lcd.print("Bom:");
    if(a<10)
    {
      lcd.print("0");
      lcd.print(a);
    }
    else
    {
      lcd.print(a);
    }
    lcd.setCursor(15,1);
    lcd.print("% "); 
  }
}
