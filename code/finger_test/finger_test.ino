#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>

DS3231 rtc(20, 21);
const int DHTPIN = 11;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
int gas = 7;
LiquidCrystal_I2C lcd(0x27,16,2);
Time real_time;


#define CHANGE_PASS 0
#define ADD_FINGER 1
#define RUN_NORMAL 2
#define CHECK_PASS 3
#define FIRST_PASS 4
#define SECOND_PASS 8

#define SET_PASS_KEY 5
#define NORMAL_KEY 6
#define NO_KEY     7

#define PASS_OK 9
#define PASS_NOT_OK 10
#define COMPARE_PASS 11
#define COM_PASS 12
#define FIRST_FINGER 13
#define SENCOND_FINGER 14
#define CLEAR_FINGER 15
void get_pass(void);
void Smart_door(void);
void show_less_ten(int value);
void Send_To_Esp(void);

byte value0 = 0;
byte value1 = 0;
byte value2 = 0;
byte value3 = 0;
byte buff[2];
uint32_t value = 0;

char pass[4] = {0};
char val[2] = {0,0};
int count_pass = 0;
char key = 0;
char state = PASS_NOT_OK;
//char status_pass = NOT_OK;
uint32_t pas = 1234;
uint32_t pass_finger = 0;
uint32_t buffer1_pass = 0;
uint32_t buffer2_pass = 0;
char is_First = FIRST_PASS;
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
volatile int mode = RUN_NORMAL;
char status_finger = FIRST_FINGER;
int lcd_print = 0;
volatile int ID = 0;
char a=0;
char p;
int pre = 0;
char status_add = 0;
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

byte rowPins[rows] = {46, 48, 50, 52};
byte columnPins[columns] = {38, 40, 42, 44};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
SoftwareSerial mySerial(10, 3);
Servo myservo;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int humidity = 0;
int temperature = 0;
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

char LCD_SHOW = 0;
char LCD_CLEAR = 0;
uint32_t show_time = 0;

uint16_t lux1 = 0;
uint16_t lux2 = 0;

void setup()  
{
  Timer1.initialize(100000); 
  Timer1.attachInterrupt(get_pass);
  finger.begin(57600);
  Serial.begin(9600);
  Serial2.begin(9600);
  while (!Serial);
  

  Wire.begin();
  dht.begin();
  
  delay(5);
  pinMode(gas, INPUT);
  rtc.begin(); 
  delay(1000);
  Serial.println("\n\nAdafruit finger detect test");
  p = FINGERPRINT_PACKETRECIEVEERR;
  while(p != FINGERPRINT_OK)
  {
    p = finger.getTemplateCount();
  }
  Serial.println(finger.templateCount);

  lcd.begin();  
  lcd.backlight();
  lcd.createChar(1, degree);
  //lcd.print("Nhiet do: ");
}

void loop()                  
{
  Send_To_Esp();
  Smart_door();
  delay(500);
  /*if(show_time < 30)
  {
    Real_time();
  }
  else if((show_time == 30)||(show_time == 60))
  {
    if(LCD_SHOW == 0)
    {
    lcd.clear();
    }
  }
  else if((show_time > 30)&&(show_time < 60))
  {
    DHT_read();
    Light_Sensor();
  }
  else if(show_time > 60)
  {
    show_time = 0;
  }
  else
  {
    
  }
  show_time++;*/

}
void Send_To_Esp(void)
{
  Serial2.write("h:");
  Serial2.write(humidity);
  Serial2.write("t:");
  Serial2.write(temperature);
  Serial2.write("lux1:");
  Serial2.write(lux1);
  Serial2.write("lux2:");
  Serial2.write(lux2);
}
void Real_time(void)
{
  real_time = rtc.getTime();
  if(LCD_SHOW == 0)
  {
    lcd.setCursor(1,0);
    switch(real_time.dow)
    {
      case 1:
      lcd.print("Mon");
      break;
      case 2:
      lcd.print("Tus");
      break;
      case 3:
      lcd.print("Wed");
      break;
      case 4:
      lcd.print("Thu");
      break;
      case 5:
      lcd.print("Fri");
      break;
      case 6:
      lcd.print("Sat");
      break;
      case 7:
      lcd.print("Sun");
      break;
      default:
      break;
    }
    lcd.setCursor(5,0);
    show_less_ten(real_time.date);
    lcd.print("/");
    
    lcd.setCursor(8,0);
    show_less_ten(real_time.mon);
    lcd.print("/");
    
    lcd.setCursor(11,0);
    lcd.print(real_time.year);
    
    lcd.setCursor(4,1);
    show_less_ten(real_time.hour);
    lcd.print(":");
    
    lcd.setCursor(7,1);
    show_less_ten(real_time.min);
    lcd.print(":");

    lcd.setCursor(10,1);
    show_less_ten(real_time.sec);
  }
}
void show_less_ten(int value)
{
  if(value < 10)
  {
    lcd.print("0");
    lcd.print(value);
  }
  else
  {
    lcd.print(value);
  }
}
/*int gas(void)
{
  int value = digitalRead(gas); 
  Serial.println(value); 
}*/

void DHT_read(void)
{
  humidity = (int)dht.readHumidity();
  temperature = (int)dht.readTemperature();
if(LCD_SHOW == 0)
{
  lcd.setCursor(0,0);
  lcd.print("Tem:");
  lcd.print(temperature);
  lcd.write(1);
  lcd.print("C");

  lcd.setCursor(9,0);
  lcd.print("Hum:");
  lcd.print(humidity);
  lcd.print("% ");  
} 
}

void Light_Sensor(void)
{
  int i;
  uint16_t val1=0;
  uint16_t val2=0;
  int BH1750address1 = 0x23;
  int BH1750address2 = 0x5C;
  
  BH1750_Init(BH1750address1);
  BH1750_Init(BH1750address2);
  if(2==BH1750_Read(BH1750address1))
  {
    val1=((buff[0]<<8)|buff[1])/1.2;
    if(LCD_SHOW == 0)
    {
      lcd.setCursor(0,1);
      lcd.print("Lux1:");
      lcd.print(val1);
      lux1 = val1;
    }
  }
  if(2==BH1750_Read(BH1750address2))
  {
    val2=((buff[0]<<8)|buff[1])/1.2;
    if(LCD_SHOW == 0)
    {
      lcd.setCursor(8,1);
      lcd.print("Lux2:");
      lcd.print(val2);
      lux2 = val2;
    }
  }
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
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}

void Smart_door(void)
{
  char k;
  if(LCD_SHOW == 1)
  {
    lcd.clear();
    LCD_CLEAR = 1;
    
  }
if(mode != RUN_NORMAL)
{
  if(LCD_CLEAR == 1)
  {
    LCD_SHOW = 2;
  }
  else
  {
    LCD_SHOW = 1;
  }
  if((mode == CHANGE_PASS)&&(state == PASS_NOT_OK))
  {
    lcd.setCursor(2,0);
    lcd.print("Doi mat Khau");
    lcd.setCursor(0,1);
    lcd.print("Mat Khau Cu:");
    if((lcd_print != 0)&&(pre != lcd_print))
    {
       lcd.setCursor(lcd_print,1);
       lcd.print("*");
       Serial.println("here");
       pre = lcd_print;
       if(lcd_print == 15)
       {
        lcd_print = 0;
        pre = 0;
        delay(500);
        lcd.clear();
       }
    }
  }
  else if((mode == ADD_FINGER)&&(state != CHECK_PASS))
  {
    lcd.setCursor(2,0);
    lcd.print("Them van tay");
    lcd.setCursor(0,1);
    lcd.print("Mat Khau:");
    if((lcd_print != 0)&&(pre != lcd_print))
    {
       lcd.setCursor(lcd_print,1);
       lcd.print("*");
       pre = lcd_print;
       if(lcd_print == 15)
       {
        lcd_print = 0;
        pre = 0;
        delay(500);
        lcd.clear();
       }
    }
  }
  //else if((mode == CLEAR_FINGER)&&(state == PASS_NOT_OK))
  //{
    
  //}
  else
  {
    
  }
  if(state == CHECK_PASS)
  {
    value0 = EEPROM.read(0);
    value1 = EEPROM.read(1);
    value2 = EEPROM.read(2);
    value3 = EEPROM.read(3);
    value = 1000*value0 + 100*value1 + 10*value2 + value3;
    Serial.println(value);
    if(pass_finger == value)
    {
        Serial.println("Password is correct!");
        if(mode == CHANGE_PASS)
        {
            lcd.setCursor(2,0);
            lcd.print("Doi mat Khau");
            lcd.setCursor(0,1);
            lcd.print("Mat Khau Moi:");
        }
        else if(mode == ADD_FINGER)
        {
            Serial.println("Please choose the ID for finger");
            state = ADD_FINGER;
        }
        else if(mode == CLEAR_FINGER)
        {
            Serial.println("clear all finger");
            p = finger.emptyDatabase();
            if(p == FINGERPRINT_OK)
            {
              Serial.println("clear done");
            }
            mode = RUN_NORMAL;
            state = PASS_NOT_OK;
            LCD_SHOW = 0;
            LCD_CLEAR = 0;
        }
        else
        {
          
        }
        while(mode == ADD_FINGER)
        {
            lcd.setCursor(2,0);
            lcd.print("Them van tay");
            lcd.setCursor(0,1);
            lcd.print("ID:");
            if((lcd_print != 0)&&(pre != lcd_print))
              {
                lcd.setCursor(lcd_print,1);
                lcd.print(key);
                pre = lcd_print;
                if(lcd_print == 18)
                {
                  lcd_print = 0;
                  pre = 0;
                  delay(500);
                  lcd.clear();
                }
               }
            while(ID != 0)
            {
                if(status_finger == FIRST_FINGER)
                {
                  lcd.setCursor(0,1);
                  lcd.print("Dat ngon tay");
                    p = finger.getImage();
                    if(p == FINGERPRINT_OK)
                    {
                        delay(500);
                        p = finger.image2Tz(1);
                        if(p == FINGERPRINT_OK)
                        {
                            lcd.clear();
                            lcd.setCursor(2,0);
                            lcd.print("Them van tay");
                            lcd.setCursor(0,1);
                            lcd.print("Success");
                            delay(1000);
                            lcd.setCursor(0,1);
                            lcd.print("dat lan nua");
                            delay(3000);
                            status_finger = SENCOND_FINGER;
                        }
                    }
                }
                else
                {
                  while(status_add == 0)
                  {
                    p = finger.getImage();
                    if(p == FINGERPRINT_OK)
                    {
                        p = finger.image2Tz(2);
                        if(p == FINGERPRINT_OK)
                        {
                            p = finger.createModel();
                            if(p == FINGERPRINT_OK)
                            {
                                lcd.clear();
                                p = finger.storeModel(ID);
                                while(p != FINGERPRINT_OK)
                                {
                                  p = finger.storeModel(ID);
                                  status_add = 1;
                                }
                                lcd.setCursor(0,1);
                                lcd.print("success");
                                mode = RUN_NORMAL;
                                ID = 0;
                                state = PASS_NOT_OK;
                                LCD_SHOW = 0;
                                LCD_CLEAR = 0;
                            }
                            else if(p == FINGERPRINT_ENROLLMISMATCH)
                            {
                                lcd.clear();
                                lcd.setCursor(2,0);
                                lcd.print("Them van tay");
                                lcd.setCursor(0,1);
                                lcd.print("Not match!");
                                delay(1000);
                                lcd.setCursor(0,1);
                                lcd.print("dat lan nua");
                            }
                            else
                            {
                                
                            }
                            status_finger = FIRST_FINGER;
                        }
                    }
                  }
                  status_add = 0;
                }
            }
        }
        state = PASS_OK;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Wrong password!");
        delay(2000);
        lcd.clear();
        Serial.println("Password is incorrect!");
        Serial.println(pass_finger);
        state = PASS_NOT_OK;
        mode = RUN_NORMAL;
        LCD_SHOW = 0;
        LCD_CLEAR = 0;
    }
  }
  if((state == PASS_OK)&&(mode == CHANGE_PASS))
  {
    if((lcd_print != 0)&&(pre != lcd_print))
      {
         lcd.setCursor(lcd_print,1);
         lcd.print("*");
         pre = lcd_print;
         if(lcd_print == 15)
         {
          lcd_print = 0;
          pre = 0;
          delay(500);
          lcd.clear();
          state = COMPARE_PASS;
         }
      }
  }
  else if((state == COMPARE_PASS)||(state == COM_PASS))
  {
    lcd.setCursor(2,0);
    lcd.print("Doi mat Khau");
    lcd.setCursor(0,1);
    lcd.print("Nhap lai:");
    if((lcd_print != 0)&&(pre != lcd_print))
    {
       lcd.setCursor(lcd_print,1);
       lcd.print("*");
       pre = lcd_print;
       if(lcd_print == 15)
       {
        lcd_print = 0;
        pre = 0;
        delay(500);
        lcd.clear();
       }
    }
    if(state == COM_PASS)
    {
      Serial.println(buffer2_pass);
    if(buffer1_pass == buffer2_pass)
    {
      value0 = buffer2_pass/1000;
      buffer2_pass = buffer2_pass - value0*1000;
      value1 = buffer2_pass/100;
      buffer2_pass = buffer2_pass - value1*100;
      value2 = buffer2_pass/10;
      buffer2_pass = buffer2_pass - value2*10;
          EEPROM.write(0, value0);
          EEPROM.write(1, value1);
          EEPROM.write(2, value2);
          EEPROM.write(3, buffer2_pass);
          lcd.setCursor(2,0);
          lcd.print("Doi mat Khau");
          lcd.setCursor(0,1);
          lcd.print("Success");
          delay(3000);
          LCD_SHOW = 0;
          LCD_CLEAR = 0;
          lcd.clear();
    }
    else
    {
          lcd.setCursor(2,0);
          lcd.print("Doi mat Khau");
          lcd.setCursor(0,1);
          lcd.print("khong khop");
          delay(3000);
          LCD_SHOW = 0;
          LCD_CLEAR = 0;
          lcd.clear();
    }
    mode = RUN_NORMAL;
    state = PASS_NOT_OK;
    }
  }
}
else
{
  k = finger.getImage();
  if(k == FINGERPRINT_OK)
  {
    LCD_SHOW = 1;
    Serial.println("get finger success");
    k = finger.image2Tz();
    if(k == FINGERPRINT_OK)
    {
      k = finger.fingerFastSearch();
      lcd.clear();
      if(k == FINGERPRINT_OK)
      {
        lcd.print("Success");
        Serial.println(finger.fingerID);
        LCD_SHOW = 0;
      }
      else
      {
        lcd.print("Not found");
        LCD_SHOW = 0;
      }
      
      delay(2000);
      lcd.clear();
    }
  }
  
}
}

void get_pass(void)
{
  
  char i=0;
  uint32_t convert_pass = 0;
  char temp = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      Serial.println(temp);

  if((temp == '*')&&(mode == RUN_NORMAL))
  {
    mode = CHANGE_PASS;
    Serial.println("nhap mat khau:");
  }
  else if((temp == '#')&&(mode == RUN_NORMAL))
  {
      mode = ADD_FINGER;
      Serial.println("nhap mat khau:");
  }
  else if((temp == 'B')&&(mode == RUN_NORMAL))
  {
      mode = CLEAR_FINGER;
      Serial.println("nhap mat khau:");
  }
  else if(((temp < 58)&&(temp > 47)&&(mode != RUN_NORMAL))||(temp == 'A'))
  {
      if(state == ADD_FINGER)
      {
          if((temp < 58)&&(temp > 47))
          {
            Serial.println("ID");
            val[count_pass] = temp-48;
            lcd_print = count_pass + 5;
            count_pass++;
          }
          if(temp == 'A')
          {
              ID = (val[1])*10 + val[0];
              count_pass = 0;
              Serial.println("ID");
              Serial.println(ID);
              for(i = 0; i < 2; i++)
              {
                val[i] = 0;
              }
              lcd_print = 18;
          }
      }
      else
      {
        if((count_pass<4)&&(temp < 58)&&(temp > 47))
        {
            pass[count_pass] =  temp - 48;
            lcd_print = count_pass + 12;
            count_pass++;
        }
        if(count_pass == 4)
        {
            convert_pass = 1000*(pass[0]) + 100*(pass[1]) + 10*(pass[2]) + pass[3];
            count_pass = 0;
            if(((mode == CHANGE_PASS)||(mode == ADD_FINGER)||(mode == CLEAR_FINGER))&&(state == PASS_NOT_OK))
            {
                state = CHECK_PASS;
                pass_finger = convert_pass;
            }
            else if((mode == CHANGE_PASS)&&(state == PASS_OK))
            {
                buffer1_pass = convert_pass;
            }
            else if((mode == CHANGE_PASS)&&(state == COMPARE_PASS))
            {
                buffer2_pass = convert_pass;
                state = COM_PASS;
            }
            else
            {
            }
            for(i = 0; i < 4; i++)
            {
                pass[i] = 0;
            }
        }
      }
  }
  else
  {
      
  }
  }
  }
  key = temp;
  temp = 0;
}
