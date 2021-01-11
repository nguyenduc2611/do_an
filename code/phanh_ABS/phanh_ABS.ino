#include <LiquidCrystal_I2C.h>

#define CHU_VI 10
#define PHANH 3
#define RELAY1 4
#define RELAY2 5
LiquidCrystal_I2C lcd(0x20,16,2);

char pin_inter = 2;
int i = 0;
char print_lcd = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t = 0;
int v = 0;
unsigned long v_sum = 0;
int count = 0;
int v_dai = 0;
int v_goc = 0;
char status_phanh = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(pin_inter, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_inter), call_inter, FALLING);
  lcd.begin();
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("hello");
  Serial.println("hello");
  pinMode(PHANH, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY2, LOW);
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V_dai:");
    lcd.print(v_dai);
    lcd.print("m/s");
    lcd.setCursor(0,1);
    lcd.print("V_goc:");
    lcd.print(v_goc);
    lcd.print("rad/s");
}

void call_inter(void)
{
    if(i == 29)
    {
      print_lcd = 1;
    }
    else if(i == 0)
    {
      t1 = millis();
    }
    else
    {
      
    }
    i++;
}

void loop() {
  char phanh = digitalRead(PHANH);
  // put your main code here, to run repeatedly:
  if((print_lcd)&&(phanh == LOW))
  {
    
    t2 = millis();
    t = t2 - t1;
    v = 1000/t;
    v_sum = v_sum + v;
    count++;
    if(count == 100)
    {
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V_dai:");
    v_dai = (v_sum*CHU_VI)/100;
    lcd.print(v_dai);
    lcd.print("m/s");
    lcd.setCursor(0,1);
    lcd.print("V_goc:");
    v_goc = (7*v_sum)/100;
    lcd.print(v_goc);
    lcd.print("rad/s");
    v_sum = 0;
    count = 0;
    }
    i = 0;
    print_lcd = 0;
  }

  if((phanh == HIGH)&&(v_dai > 7)&&(v_goc > 5))
  {
    v_dai = v_dai - 7;
    v_goc = v_goc - 5;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V_dai:");
    lcd.print(v_dai);
    lcd.print("m/s");
    lcd.setCursor(0,1);
    lcd.print("V_goc:");
    lcd.print(v_goc);
    lcd.print("rad/s");
    status_phanh = 1;
    delay(500);
  }
  else if ((status_phanh)&&((v_dai < 7)||(v_goc < 5)))
  {
    status_phanh = 0;
    v_dai = 0;
    v_goc = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V_dai:");
    lcd.print(v_dai);
    lcd.print("m/s");
    lcd.setCursor(0,1);
    lcd.print("V_goc:");
    lcd.print(v_goc);
    lcd.print("rad/s");
    for(int i = 0; i < 7; i++)
    {
      digitalWrite(RELAY1, HIGH);
      delay(100);
      digitalWrite(RELAY1, LOW);
      delay(100);
    }

    digitalWrite(RELAY2, HIGH);
  }
  else
  {
    
  }
}
