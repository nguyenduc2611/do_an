
#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define mySerial Serial1

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
#define PASS_RECORDED 13
#define PASS_CHECK_NOT 14

int buzzer = 4;

void get_pass(void);

byte value0 = 0;
byte value1 = 0;
byte value2 = 0;
byte value3 = 0;

uint32_t value = 0;

char pass[4] = {0};
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
const byte columns = 3; //số cột
volatile int mode = RUN_NORMAL;
int lcd_print = 0;
int record_fail = 0;
int dis = 0;
char keys[rows][columns] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
};
int servoPos = 0;
byte rowPins[rows] = {3, 7, 6, 8};
byte columnPins[columns] = {10, 9, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
SoftwareSerial mySerial(2, 5);
void setup()  
{
  Timer1.initialize(100000); 
  Timer1.attachInterrupt(get_pass);
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  lcd.begin();  
  delay(50);
  lcd.backlight();
  delay(50);
  lcd.setCursor(0,0);
  lcd.print("Nhap mat khau:");
  
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop()                  
{
  if(mode != RUN_NORMAL)
  {
    if((dis == 0)&&(mode == CHANGE_PASS))
    {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cai Dat Mat Khau");
    delay(3000);
    lcd.clear();
    dis = 1;
    }
    if((dis == 1)&&(mode == CHANGE_PASS))
    {
    lcd.setCursor(0,0);
    lcd.print("Nhap mat khau:");
    if(lcd_print != 0)
    {
       lcd.setCursor(lcd_print,1);
       lcd.print("*");
       if(lcd_print == 9)
       {
        lcd_print = 0;
        dis = 2;
        lcd.clear();
       }
    }
    
    }
  if(state == CHECK_PASS)
  {
    value0 = EEPROM.read(0);
    value1 = EEPROM.read(1);
    value2 = EEPROM.read(2);
    value3 = EEPROM.read(3);
    value = 1000*value0 + 100*value1 + 10*value2 + value3;
    if(pass_finger == value)
    {
        state = PASS_OK;
    }
    else
    {
        lcd.setCursor(0,0);
        lcd.print("Sai Mat Khau:");
        state = PASS_NOT_OK;
        if(mode == PASS_RECORDED)
        {
          state = PASS_CHECK_NOT;
        }
        mode = RUN_NORMAL;
        dis = 0;
    }
    
  }
  if((mode == PASS_RECORDED)&&(state == PASS_OK))
  {
    mode = RUN_NORMAL;
  }
  if((mode == CHANGE_PASS)&&(state == PASS_OK))
  {
      lcd.setCursor(0,0);
      lcd.print("Mat Khau Moi:");
      if(lcd_print != 0)
      {
        lcd.setCursor(lcd_print,1);
        lcd.print("*");
        if(lcd_print == 9)
        {
          lcd_print = 0;
          state = FIRST_PASS;
          lcd.clear();
        }
      }
  }
  if((state == FIRST_PASS))
  {
    lcd.setCursor(0,0);
    lcd.print("Nhap Lai Mat Khau:");
    if(lcd_print != 0)
    {
      lcd.setCursor(lcd_print,1);
      lcd.print("*");
      if(lcd_print == 9)
      {
        lcd_print = 0;
      } 
    }      
  }
  if(state == COM_PASS)
  {
    if(state == COM_PASS)
    {
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
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Thanh Cong");
          delay(3000);
          lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Nhap mat khau:");
          dis = 0;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Khong Khop");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nhap mat khau:");
      dis = 0;
    }
    mode = RUN_NORMAL;
    state = PASS_NOT_OK;
    delay(3000);
    }
  }
  }
  else
  {
    if(lcd_print != 0)
    {
      lcd.setCursor(lcd_print,1);
      lcd.print("*");

        if(state == PASS_OK)
        {
          lcd.clear();
          delay(100);
          lcd.setCursor(0,0);
          lcd.print("Chinh xac");
          state = PASS_NOT_OK;
          lcd_print = 0;
          record_fail = 0;
          Serial.write('1');
          delay(3000);
          if(state == PASS_NOT_OK)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Nhap mat khau:");
          }
        }
        else if(state == PASS_CHECK_NOT)
        {
          record_fail++;
          state = PASS_NOT_OK;
          lcd_print = 0;
          if(record_fail == 3)
          {
            digitalWrite(buzzer, HIGH);
            record_fail = 0;
            delay(3000);
            digitalWrite(buzzer, LOW);
          }
          
          if(state == PASS_NOT_OK)
          {
            delay(3000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Nhap mat khau:");
          }
        }
        else
        {
          
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
      key = temp;
    }
  }
  if((temp == '*')&&(mode == RUN_NORMAL))
  {
    mode = CHANGE_PASS;
  }
  else if((temp == '#')&&(mode == RUN_NORMAL))
  {
      mode = ADD_FINGER;
  }
  else if((temp < 58)&&(temp > 47)&&(mode != RUN_NORMAL))
  {
      if(count_pass<4)
      {
        pass[count_pass] =  temp - 48;
        count_pass++;
        lcd_print = count_pass + 5;
      }
      if(count_pass == 4)
      {
        convert_pass = 1000*(pass[0]) + 100*(pass[1]) + 10*(pass[2]) + pass[3];
        count_pass = 0;
        if(((mode == CHANGE_PASS)||(mode == ADD_FINGER))&&(state == PASS_NOT_OK))
        {
            state = CHECK_PASS;
            pass_finger = convert_pass;
        }
        else if((mode == CHANGE_PASS)&&(state == PASS_OK))
        {
            state = FIRST_PASS;
            buffer1_pass = convert_pass;
        }
        else if((mode == CHANGE_PASS)&&(state == FIRST_PASS))
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
  else if(((temp < 58)&&(temp > 47))&&((mode == RUN_NORMAL))||(mode == PASS_RECORDED))
  {
      if(count_pass<4)
      {
        pass[count_pass] =  temp - 48;
        count_pass++;
        lcd_print = count_pass + 5;
        
      }
      if(count_pass == 4)
      {
        convert_pass = 1000*(pass[0]) + 100*(pass[1]) + 10*(pass[2]) + pass[3];
        count_pass = 0;
        for(i = 0; i < 4; i++)
        {
            pass[i] = 0;
        }
        state = CHECK_PASS;
        pass_finger = convert_pass;
        mode = PASS_RECORDED;
      }
  }
  else
  {
      
  }
  
  temp = 0;
}
