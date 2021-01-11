#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <Servo.h> 

/*mode của password*/
#define MODE_RUN 0
#define MODE_CHANGE 1
#define MODE_CANCEL 2
#define MODE_INPUT_PASS 3

/*define chân*/
#define FAN1_PIN 35
#define FAN2_PIN 31
#define LAMP1_PIN 37
#define LAMP2_PIN 33
#define BUZZER    8
#define IR_SENSOR A1

#define CHECKING  2
#define NOT_OK    1
#define OK        0

#define ENABLE    1
#define DISABLE   0

#define ON  1
#define OFF 2
/*global variable*/
char key = 0;
char mode_pass = MODE_RUN;
char count_key = 0;
char count_key_in = 0;
char buffer_pass[5] = {0};
char new_pass1[5] = {0};
char new_pass2[5] = {0};
char count_enter_pass = 0;
char status_recieve = 0;
char buffer_recieve[] = {0};
char count_data = 0;
char mode_door = 0;
char status_light = OFF;
char status_door = OFF;
char status_fan = OFF;
char status_mode_door = OFF;
int temp = 0;
int hum = 0;
char string_data[4]={0};
/*cau hinh servo*/
#define SERVO_PIN 25
Servo gServo;
/*cấu hình DHT*/
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
char run_CheckPass(void);
/*software serial dùng giao tiếp node MCU*/
SoftwareSerial mySerial(12, 3);
SoftwareSerial simSerial(11, 10);

/*cấu hình LCD*/
LiquidCrystal_I2C lcd(0x3f,16,2);
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

/*cấu hình bàn phím*/
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
  };
byte rowPins[rows] = {45, 43, 41, 39};
byte columnPins[columns] = {53, 51, 49, 47};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
char data[5] = {0};

void setup() {
  // put your setup code here, to run once:
  //Timer1.initialize(100000); 
  //Timer1.attachInterrupt(get_key);
  Serial.begin(9600);
  mySerial.begin(9600);
  //simSerial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, degree);
  Wire.begin();
  gServo.attach(SERVO_PIN);
  gServo.write(80);
  delay(500);
  gServo.detach();
  mode_door = EEPROM.read(4);

/* setting the direction of the control pin*/
  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);
  pinMode(LAMP1_PIN, OUTPUT);
  pinMode(LAMP2_PIN, OUTPUT);

  digitalWrite(FAN1_PIN, HIGH);
  digitalWrite(FAN2_PIN, HIGH);
  digitalWrite(LAMP1_PIN, HIGH);
  digitalWrite(LAMP2_PIN, HIGH);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  
}
/*check bàn phím*/
void get_key(void)
{
  char buffer_key = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (buffer_key != 0) {
      key = buffer_key;
      Serial.println("key:");
      Serial.println(key);
    }
  }

  /* set up password*/   
  if((key == '*')&&(mode_pass == MODE_RUN))
  {
      mode_pass = MODE_CHANGE;
      Serial.println("change pass");
  }
  /* Cancel*/
  else if((key == 'B')&&(mode_pass == MODE_RUN))
  {
      mode_pass = MODE_CANCEL;
      Serial.println("cancel");
  }
  else
  {
    
  }

  buffer_key = 0;
}
/*Đo nhiệt độ và độ ẩm, return nhiệt độ để bật tắt quạt*/
int DHT_Report(void)
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print((int)t);
  lcd.write(1);
  lcd.print("C");

  lcd.setCursor(8,0);
  lcd.print("H:");
  lcd.print((int)h);
  lcd.print("% ");

  temp = (int)t;
  hum = (int)h;
  //mySerial.write(t);
  //mySerial.write("H");
  //mySerial.write(h);
  //mySerial.write("T");
  
  return t;
}

/*chạy tại mode normal*/
void run_Normal(void)
{
  /* read and check temperature*/
  temp = DHT_Report();
  if(temp > 25)
  {
    /*On fan*/
    digitalWrite(FAN1_PIN, LOW);
	digitalWrite(FAN2_PIN, LOW);
  }
  else if(temp < 15)
  {
    digitalWrite(FAN1_PIN, HIGH);
	digitalWrite(FAN2_PIN, HIGH);
  }
  else
  {
    if(status_fan == ON)
    {
      digitalWrite(FAN1_PIN, LOW);
	  digitalWrite(FAN2_PIN, LOW);
    }
    else if(status_fan == OFF)
    {
      digitalWrite(FAN1_PIN, HIGH);
	  digitalWrite(FAN2_PIN, HIGH);
    }
    else
    {
    
    }
  }
  /*Xử lý cảm biến Khí gas*/
  int status_fire = analogRead(A0);
  if(status_fire > 400)
  {
    digitalWrite(BUZZER, HIGH);
    simSerial.begin(9600);
    simSerial.write("AT");
    simSerial.write(10);
    delay(1000);
    simSerial.write("AT+CMGF=1");
    simSerial.write(10);
    delay(1000);
    simSerial.write("AT+CMGS=");
    simSerial.write(34);
    simSerial.write("0356325993");
    simSerial.write(34);
    simSerial.write(10);
    delay(1000);
    simSerial.write("chay");
    delay(1000);
    simSerial.write(26);
    delay(5000);
    digitalWrite(BUZZER, LOW);
    mySerial.begin(9600);
  }
  if(status_mode_door == ON)
  {
    //Serial.println("mat khau cua");
    /*Kiem tra mat khau*/
    char check_pass = run_CheckPass();
    if(check_pass == OK)
    {
      count_enter_pass = 0;
      gServo.attach(SERVO_PIN);
      gServo.write(150);
      delay(500);
      gServo.detach();
      delay(3000);
      gServo.attach(SERVO_PIN);
      gServo.write(80);
      delay(500);
      gServo.detach();
      mode_pass = MODE_RUN;
      Serial.println("xong pass");
    }
    else if(check_pass == NOT_OK)
    {
      count_enter_pass++;
      if(count_enter_pass == 3)
      {
        digitalWrite(BUZZER, HIGH);
        delay(2000);
        digitalWrite(BUZZER, LOW);
        count_enter_pass = 0;
      }
      mode_pass = MODE_RUN;
    }
    else
    {
        /*Mở của dựa cảm biến hồng ngoại*/
    }
    
  }
  else if(status_mode_door == OFF)
  {
    int ir_sensor = analogRead(IR_SENSOR);
    if(ir_sensor < 512)
    {
      gServo.attach(SERVO_PIN);
      gServo.write(150);
      delay(500);
      gServo.detach();
      delay(3000);
      gServo.attach(SERVO_PIN);
      gServo.write(80);
      delay(500);
      gServo.detach();
      
    }
    else
    {
      /*no else*/
    }
  }
  else
  {
    
  }

  run_ReciveMcu();

  if(status_door == ON)
  {
      gServo.attach(SERVO_PIN);
      gServo.write(150);
      delay(500);
      gServo.detach();
      delay(3000);
      gServo.attach(SERVO_PIN);
      gServo.write(80);
      delay(500);
      gServo.detach();
      status_door = OFF;
  }

  if(status_light == ON)
  {
    //Serial.println("light_on");
      digitalWrite(LAMP2_PIN, LOW);
      digitalWrite(LAMP1_PIN, LOW);
  }
  else if(status_light == OFF)
  {
    //Serial.println("light_off");
      digitalWrite(LAMP2_PIN, HIGH);
      digitalWrite(LAMP1_PIN, HIGH);
  }
  else
  {
  }
  
}


void run_ReciveMcu(void)
{
  
  while(mySerial.available() > 0)
  {
    data[count_data] = mySerial.read()-48;
    //Serial.println(data[count_data]);
    count_data++;
    delay(10);
  }

    if(count_data != 0)
    {
      Serial.println(count_data+48);
      Serial.println("dd");
      Serial.println(data[1] + 48);

      status_light = data[0];
      status_door = data[1];
      status_fan = data[2];
      status_mode_door = data[3];
      /*switch(data[0])
      {
        case 3:
          status_light = data[1];
        break;
        case 4:
          status_door = data[1];
        break;
        case 5:
          status_fan = data[1];
        break;
        case 6:
          status_mode_door = data[1];
        break;
        default:
        break;
      }*/
      sprintf(string_data,"%d%d",hum,temp);
      mySerial.write(string_data);
      count_data = 0;
    }

}

/*chạy mode đổi password*/
void run_ChangePass(void)
{
  lcd.setCursor(0,0);
  lcd.print("Doi Mat Khau:  ");
  lcd.setCursor(0,1);
  lcd.print("Nhap:");

  char status_pass = run_CheckPass();
  if(status_pass == OK)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("xac nhan mat khau");
    mode_pass = MODE_INPUT_PASS;
  }
  else if(status_pass == NOT_OK)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("mat khau sai");
    delay(3000);
    lcd.clear();
    mode_pass = MODE_RUN;
  }
  else
  {
    /* code */
  }
  
}

/*lay du lieu pass va chec*/
char run_CheckPass(void)
{
  char key_pass[5] = {0};
  char status_pass = CHECKING;
  int i = 0;

  if((key < 58)&&(key > 47))
  {
    if(count_key == 0)
    {
    lcd.clear();
    }
    if(count_key < 4)
    {
      buffer_pass[count_key] =  key - 48;
      count_key++;
      lcd.setCursor((count_key + 6),1);
      lcd.print("*");
      key = 0;
    }

    if(count_key == 4)
    {
      lcd.clear();
      count_key = 0;
      key_pass[0] = EEPROM.read(0);
      key_pass[1] = EEPROM.read(1);
      key_pass[2] = EEPROM.read(2);
      key_pass[3] = EEPROM.read(3);

      for(i = 0; i < 4; i++)
      {
        if(key_pass[i] != buffer_pass[i])
        {
          break;
        }
      }

      if(i == 4)
      {
        status_pass = OK;
      }
      else
      {
        status_pass = NOT_OK;
      }
      
    }
  }

  return status_pass;
}

/*get new password*/
void run_InputPass(void)
{
  int i = 0;

  if(count_key_in < 4)
  {
    lcd.setCursor(0,0);
    lcd.print("Nhap password 1:");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Nhap password 2:");
  }
  if((key < 58)&&(key > 47))
  {
    if(count_key_in < 4)
    {
      new_pass1[count_key_in] =  key - 48;
      count_key_in++;
      lcd.setCursor((count_key_in + 6),1);
      lcd.print("*");
      if(count_key_in == 4)
      {
        lcd.clear();
      }
      key = 0;
    }
    else
    {
      new_pass2[count_key_in - 4] =  key - 48;
      count_key_in++;
      lcd.setCursor((count_key_in + 2),1);
      lcd.print("*");
      key = 0;
    }
    
    if(count_key_in == 8)
    {
      lcd.clear();
      count_key_in = 0;
      for(i = 0; i < 4; i++)
      {
        if(new_pass1[i] != new_pass2[i])
        {
          break;
        }
      }
      
      if(i == 4)
      {
        EEPROM.write(0,new_pass1[0]);
        EEPROM.write(1,new_pass1[1]);
        EEPROM.write(2,new_pass1[2]);
        EEPROM.write(3,new_pass1[3]);
        lcd.setCursor(0,0);
        lcd.print("Doi thanh cong  ");
      }
      else
      {
        lcd.setCursor(0,0);
        lcd.print("  khong khop       ");
      }
      delay(2000);
      lcd.clear();
      mode_pass = MODE_RUN;
    }
  }

}
void loop() {
  get_key();
  switch(mode_pass)
  {
    case MODE_RUN:
        run_Normal();
        break;
    case MODE_CHANGE:
        run_ChangePass();
        break;
    case MODE_INPUT_PASS:
        run_InputPass();
        break;
    default:
        break;
  }

}
