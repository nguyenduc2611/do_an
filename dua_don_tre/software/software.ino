#include <LiquidCrystal_I2C.h>
#include <Servo.h> 
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define SERVO 10
#define BUTTON_1 7
#define BUTTON_2 6
#define BUTTON_3 5
#define BUTTON_4 4
#define BUTTON_5 A0
#define BUTTON_6 A1
#define PIR 8
#define BUZ 13
#define QUAT 3
#define LED 9
#define b5 analogRead(A0)
#define b6 analogRead(A1)
Servo myservo;
SoftwareSerial simSerial(11, 12);
int count_up = 0;
int count_down = 0;
char flag_don = 0;
char flag_tra = 1;
char flag_pir = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(SERVO);
  Serial.println("khoa xe bang servo co mo hinh se chinh");
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(PIR, INPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(QUAT, OUTPUT);
  digitalWrite(BUZ, LOW);
  digitalWrite(LED, LOW);
  digitalWrite(QUAT, LOW);
  delay(3000);
  digitalWrite(QUAT, HIGH);
  lcd.begin();
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
}

void loop() {
  // put your main code here, to run repeatedly:
  int staus_b1 = digitalRead(BUTTON_1);
  int staus_b2 = digitalRead(BUTTON_2);
  int staus_b3 = digitalRead(BUTTON_3);
  int staus_b4 = digitalRead(BUTTON_4);
  int staus_b5 = b5;
  int staus_b6 = b6;
  if((staus_b1 == LOW)||(staus_b2 == LOW)||(staus_b3 == LOW)||(staus_b4 == LOW)||(staus_b5 < 512)||(staus_b6 < 512))
  {
    delay(150);
    staus_b1 = digitalRead(BUTTON_1);
    staus_b2 = digitalRead(BUTTON_2);
    staus_b3 = digitalRead(BUTTON_3);
    staus_b4 = digitalRead(BUTTON_4);
    staus_b5 = b5;
    staus_b6 = b6;

    if((staus_b1 == LOW)&&(flag_don == 0))
    {
      count_up++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Da len xe:");
      lcd.setCursor(8,1);
      lcd.print(count_up);
    }

    if((staus_b2 == LOW)&&(flag_don == 0))
    {
      flag_don = 1;
      flag_tra = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("So luong don:");
        lcd.setCursor(8,1);
        lcd.print(count_up);
    }
    if((staus_b4 == LOW)&&(flag_tra == 0))
    {
      flag_don = 0;
      flag_tra = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("So luong tra:");
        lcd.setCursor(8,1);
        lcd.print(count_down);
        if(count_down != count_up)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("khong khop");
          digitalWrite(BUZ, HIGH);
          digitalWrite(LED, HIGH);
          delay(3000);
          digitalWrite(BUZ, LOW);
          digitalWrite(LED, LOW);
          count_down = 0;
          count_up = 0;
        }
    }

    if((staus_b3 == LOW)&&(flag_tra == 0))
    {
      count_down++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Da xuong xe:");
      lcd.setCursor(8,1);
      lcd.print(count_down);
    }

    if((staus_b5 < 512)&&(flag_tra == 1))
    {
      Serial.println("khoa xe bang servo co mo hinh se chinh");
      flag_pir = 1;
    }

    while((staus_b6 > 512)&&(flag_pir))
    {
      Serial.println("da");
      int pir = digitalRead(PIR);
      Serial.println(pir);
      if(pir == LOW)
      {
        digitalWrite(BUZ, HIGH);
        digitalWrite(LED, HIGH);
        digitalWrite(QUAT, LOW);
        simSerial.begin(9600);
        simSerial.write("AT");
        simSerial.write(10);
        delay(1000);
        simSerial.write("AT+CMGF=1");
        simSerial.write(10);
        delay(1000);
        simSerial.write("AT+CMGS=");
        simSerial.write(34);
        simSerial.write("0852909331");
        simSerial.write(34);
        simSerial.write(10);
        delay(1000);
        simSerial.write("nguoi tren xe");
        delay(1000);
        simSerial.write(26);
      }
    }

    if(staus_b6 < 512)
    {
      Serial.println("mo xe, co mo hinh se dieu chinh servo ");
      digitalWrite(BUZ, LOW);
      digitalWrite(LED, LOW);
      digitalWrite(QUAT, HIGH);
      flag_pir = 0;
    }
  }

}
