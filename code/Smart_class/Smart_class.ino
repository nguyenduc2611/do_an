#include <Wire.h>
#include <math.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Filters.h>
#include <DLE_BH1750.h>
#include <Stepper.h>

#define LIGHT_ON 10
#define LIGHT_OFF 11
#define AUTO 8
#define MANUAL 9
#define UP 5
#define DOWN 6

int FAN1 = 33;
int FAN2 = 31;
int FAN3 = 39;
int FAN4 = 37;

float testFrequency = 60;                     // nhà bạn dùng điện bao nhiêu Hz? Ở VN là 60Hz
float windowLength = 20.0/testFrequency;     // mỗi tín hiệu thu về cách nhau bao nhiêu thời gian
int sensorValue = 0;
float intercept = -0.1310; //  cẩn sửa trong quá trình hiệu chuẩn
float slope = 0.04099; // cẩn sửa trong quá trình hiệu chuẩn
float current_amps; // giá trị cường độ dòng điện thực sự được đo lường
int lamp = 15;

DLE_BH1750 soft_BH1750;
DS3231 rtc(20, 21);
const int DHTPIN = 11;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);
Time real_time;
Time Time_Light;
Time Time_Curent;

void Real_time(void);
void DHT_read(void);
void Light_Sensor(void);
SoftwareSerial mySerial(10, 9);

int humidity = 0;
int temperature = 0;

int IR = 12;
char lcd_clear = 0;
int LIGHT = 0;
uint32_t MIN = 0;
uint32_t BUFF_MIN = 0;

uint32_t HOU = 0;
uint32_t BUFF_HOU = 0;

char read_time = 0;
float Wh = 0;

byte buff[2];
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

uint16_t lux1 = 0;
uint16_t lux2 = 0;
int change_lcd = 0;
char thres_gas = 0;

uint16_t lux3 = 0;
uint16_t lux4 = 0;

char send_data = 0;
char contro_light = 0;

char status_window1 = DOWN;
char status_window2 = DOWN;

int i = 0;
int buzer = 24;

RunningStatistics inputStats; 

const int stepsPerMotorRevolution = 32;
const int stepsPerOutputRevolution = 32*64;

Stepper myStepper1(stepsPerMotorRevolution, 53,49,51,47);
Stepper myStepper2(stepsPerMotorRevolution, 39,43,41,45);

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  dht.begin();
  
  rtc.begin(); 

  lcd.begin();  
  delay(100);
  lcd.backlight();
  lcd.createChar(1, degree);
  lcd.clear();
 //lcd.print("Smart Class");
  pinMode(IR, INPUT);
  inputStats.setWindowSecs( windowLength );

  myStepper1.setSpeed(1000);
  myStepper2.setSpeed(1000);

  pinMode(lamp,OUTPUT);
  pinMode(IR,INPUT);
  digitalWrite(lamp, HIGH);
  pinMode(buzer, OUTPUT);
  digitalWrite(buzer, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t value;
  value = analogRead(A0);
    if(value < 500)
  {
    thres_gas = 1;
    digitalWrite(buzer, LOW);
  }
  else if((value > 500)&&(value < 600))
  {
    thres_gas = 2;
    digitalWrite(buzer, LOW);
  }
  else if(value > 600)
  {
    thres_gas = 3;
    digitalWrite(buzer, HIGH);
  }
  else
  {
    
  }
if(change_lcd < 30)
{
  Serial.println("time");
  Real_time();
}
else if((change_lcd>30)&&(change_lcd < 60))
{
  DHT_read();
  lcd.setCursor(8,1);
  lcd.print("Lux2:");
  lcd.print(lux2);
  lcd.setCursor(0,1);
      lcd.print("Lux1:");
      lcd.print(lux1);
  Serial.println("anh sang");
}
else if((change_lcd == 30)||(change_lcd == 60)||(change_lcd == 90)||(change_lcd == 120))
{
  lcd.clear();
  Serial.println("clear");
}
else if((change_lcd>60)&&(change_lcd < 90))
{
  Serial.println("khi");
  value = analogRead(A0);
  if(value < 500)
  {
    thres_gas = 1;
    lcd.setCursor(2,0);
    lcd.print("Nong do khi");
    lcd.setCursor(2,1);
    lcd.print("Binh thuong");
  }
  else if((value > 500)&&(value < 600))
  {
    thres_gas = 2;
    lcd.setCursor(2,0);
    lcd.print("Nong do khi");
    lcd.setCursor(2,1);
    lcd.print(" Bao Dong ");
    delay(1000);
  }
  else if(value > 600)
  {
    thres_gas = 3;
    lcd.setCursor(2,0);
    lcd.print("Nong do khi");
    lcd.setCursor(2,1);
    lcd.print(" Co Chay   ");
    digitalWrite(buzer, HIGH);
  }
  else
  {
    
  }
}
else if((change_lcd>90)&&(change_lcd < 120))
{
  Serial.println("dien");
  lcd.setCursor(0,0);
  lcd.print("Dien Tieu Thu");
  lcd.setCursor(3,1);
  lcd.print(Wh);
  lcd.setCursor(8,1);
  lcd.print("WH");
}
else if(change_lcd > 120)
{
  change_lcd = 0;
}
else
{
  
}
change_lcd++;
Tranfer_data();
Serial.println("tran");
Window_Control();
Serial.println("win");
Measure_Current();
Serial.println("do-dien");
Light_Sensor();
Serial.println("do-anh");
light();
Serial.println("den");
if(LIGHT == 1)
{
  digitalWrite(lamp, LOW);
}
else
{
  digitalWrite(lamp, HIGH);
}

}
void Window_Control(void)
{
  int i = 0;
  soft_BH1750.begin(0x23);
  Serial.println("begin1");
  delay(100);
  soft_BH1750.Read(&lux3);
  Serial.println("read1");
  delay(100);
  soft_BH1750.begin(0x5C);
  Serial.println("begin2");
  delay(100);
  soft_BH1750.Read(&lux4);
  Serial.println("read2");
  delay(100);
  Serial.println(lux3);

  if((lux3 > 800)&&(status_window1 == DOWN))
  {
    for(i = 0; i < 3; i++)
    {
      myStepper1.step(-stepsPerOutputRevolution);
      status_window1 = UP;
    }
  }
  else if((lux3 < 800)&&(status_window1 == UP))
  {
    for(i = 0; i < 3; i++)
    {
      myStepper1.step(stepsPerOutputRevolution);
      status_window1 = DOWN;
    }
  }
  else
  {
    
  }
  
  if((lux4 > 800)&&(status_window2 == DOWN))
  {
    for(i = 0; i < 3; i++)
    {
      myStepper2.step(stepsPerOutputRevolution);
      status_window2 = UP;
    }
  }
  else if((lux4 < 800)&&(status_window2 == UP))
  {
    for(i = 0; i < 3; i++)
    {
      myStepper2.step(-stepsPerOutputRevolution);
      status_window2 = DOWN;
    }
  }
  else
  {
    
  }
}

void Tranfer_data(void)
{
  char Esp_data = 0;
  if(mySerial.available() > 0)
  {
    Esp_data = mySerial.read();
    if(Esp_data == 'S');
    {
      send_data = 1;
    }
    delay(100);
  }
  
  Serial.println("tran1");
  if(send_data == 1)
  {
    mySerial.write("B");
    delay(50);
    mySerial.write("T");
    delay(50);
    mySerial.write(temperature);
    delay(50);
    mySerial.write("H");
    delay(50);
    mySerial.write(humidity);
    delay(50);
    mySerial.write("G");
    mySerial.write(thres_gas);
    delay(50);
    mySerial.write("A");
    delay(50);
    mySerial.write(Wh);
    delay(50);
    mySerial.write("L");
    delay(50);
    mySerial.write(lux1);
    delay(50);
    mySerial.write("L");
    delay(50);
    mySerial.write(lux2);
    delay(50);
    mySerial.write("E");
    send_data = 0;
  }
}
void Measure_Current(void)
{
  sensorValue = analogRead(A0);
  inputStats.input(sensorValue);
  Time_Curent = rtc.getTime();
 if(read_time == 0)
 {
    HOU = Time_Curent.sec;
    read_time = 1;
 }
 BUFF_HOU = Time_Curent.sec;
 if(BUFF_HOU != HOU)
 {
  Wh =Wh+((inputStats.sigma())*220)/3600;
  read_time = 0;
 }
}
void light(void)
{
  char sen = digitalRead(IR);
  Time_Light = rtc.getTime();
  switch(Time_Light.dow)
    {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
  
        if(sen== 1)
        {
          if(contro_light == 0)
          {
            LIGHT = 1;
          }
          MIN = 3600*(Time_Light.hour) + 60*(Time_Light.min)+Time_Light.sec;
        }
        BUFF_MIN = 3600*(Time_Light.hour) + 60*(Time_Light.min)+Time_Light.sec;
        if((BUFF_MIN - MIN)==5)
        {
          MIN = 0;
          if(contro_light == 0)
          {
            LIGHT = 0;
          }
        }
      
      if(Time_Light.hour == 6)
      {
        if(sen== 1)
        {
          if(contro_light == 0)
          {
            LIGHT = 1;
          }
        }
      }
      else
      {
        if((status_window1 == UP)||(status_window2 == UP))
        {
          if(contro_light == 0)
          {
            LIGHT = 0;
          }
        }
        else
        {
        if((lux1<500)&&(lux2<500))
        {
          if(contro_light == 0)
          {
            LIGHT = 1;
          }
        }
        else if(((lux1<500)&&(lux2>500))||((lux1>500)&&(lux2<500)))
        {
          if(contro_light == 0)
          {
            LIGHT = 0;
          }
        }
        else
        {
    
        }
        }
        
      }
      break;
      case 6:
      case 7:
      Serial.println(sen);
      if(sen == 1)
        {
          if(contro_light == 0)
          {
            LIGHT = 1;
          }
          MIN = 3600*(Time_Light.hour) + 60*(Time_Light.min)+Time_Light.sec;
        }
        BUFF_MIN = 3600*(Time_Light.hour) + 60*(Time_Light.min)+Time_Light.sec;
        if((BUFF_MIN - MIN)==5)
        {
          MIN = 0;
          if(contro_light == 0)
          {
            LIGHT = 0;
          }
        }
      break;
      default:
      break;
    }

}

void Real_time(void)
{
  real_time = rtc.getTime();
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

void DHT_read(void)
{
  humidity = (int)dht.readHumidity();
  temperature = (int)dht.readTemperature();

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

void Light_Sensor(void)
{
  int i;
  uint16_t val1=0;
  uint16_t val2=0;
  int BH1750address1 = 0x23;
  int BH1750address2 = 0x5C;
  delay(200);
  BH1750_Init(BH1750address1);
  BH1750_Init(BH1750address2);
  if(2==BH1750_Read(BH1750address1))
  {
    val1=((buff[0]<<8)|buff[1])/1.2;
      lux1 = val1;
  }
  
  if(2==BH1750_Read(BH1750address2))
  {
    val2=((buff[0]<<8)|buff[1])/1.2;
      lux2 = val2;
  }

}

int BH1750_Read(int address)
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}

void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}
