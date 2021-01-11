#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <math.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


File myFile;
DS3231 rtc(20, 21);
LiquidCrystal_I2C lcd(0x27,16,2);
Time real_time;
char filename[7]={0};
char filename_open[7]={0};


#define MODE_CHANGE 1
#define MODE_ADD 2
#define MODE_RUN 3
#define MODE_CLEAR 4
#define MODE_CANCEL 20

#define MODE_FIRST_PASS 6
#define MODE_SECOND_PASS 7

#define MODE_FIRST_FINGER 8
#define MODE_SECOND_FINGER 9
#define MODE_ID_FINGER_ADD 5
#define MODE_ID_FINGER_CLEAR 12

#define OK 10
#define NOT_OK 11



char key = 0;
char mode = MODE_RUN;
int count_key = 0;
char buffer_pass[4];
char id_array[4]={0};
int id_count = 0;
int first_pass = 0;
int second_pass = 0;
int id = 0;
int buzzer = 39;
char p = FINGERPRINT_PACKETRECIEVEERR;
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
  };

byte rowPins[rows] = {23, 25, 27, 29};
byte columnPins[columns] = {31, 33, 35, 37};

//byte rowPins[rows] = {37, 35, 33, 31};
//byte columnPins[columns] = {29, 27, 25, 23};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
SoftwareSerial mySerial(10, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

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

char card[25];
int Export_day[10] = {0};
char time_lcd = 0;
char update_time = 0;
uint16_t rev[15] = {0};
int count_time = 0;
int time_check = 0;
void setup()  
{
  
  Timer1.initialize(100000); 
  Timer1.attachInterrupt(get_pass);
  finger.begin(57600);
  Serial.begin(9600);
  Serial3.begin(9600);
  lcd.begin();
  Wire.begin();
  if (!SD.begin(53)) {
    lcd.print("init failed!");
    while (1);
  }
  rtc.begin(); 
  //Serial.println("a");
  while(p != FINGERPRINT_OK)
  {
   p = finger.getTemplateCount();
   Serial.println(p);
  }
  Serial.println(finger.templateCount);
  Serial.println("b");
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);


  while((count_time != 15)&&(time_check != 1000))
  {
    if((Serial3.available() > 0))
    {
      rev[count_time] = Serial3.read()-48;
      count_time++;
    }
    if(count_time == 15)
    {
    rtc.setTime((10*rev[4] + rev[5]), (10*rev[2] + rev[3]),(10*rev[0] + rev[1]));
    rtc.setDate((10*rev[6] + rev[7]),(10*rev[9] + rev[10]),(1000*rev[11] + 100*rev[12] + 10*rev[13] + rev[14]));
    rtc.setDOW(rev[8]);
    }
    time_check++;
  }
  

}

void loop()                  
{
    process_key();
    process_mode();
    //log_data();
    recieve_data();
}
void process_key(void)
{
    char value_0 = 0;
    char value_1 = 0;
    char value_2 = 0;
    int i = 0;
    switch(mode)
    {
        case MODE_ID_FINGER_ADD:
            lcd.setCursor(0,0);
            lcd.print("Them Van Tay");
            lcd.setCursor(0,1);
            lcd.print("ID:");
            if((key < 58)&&(key > 47))
            {
                id_array[id_count] = key - 48;
                id_count++;
                lcd.setCursor((6+id_count),1);
                lcd.print(key);
                key = 0;
            }
            else if(key == 'D')
            {
                switch(id_count)
                {
                    case 1:
                    id = id_array[0];
                    break;
                    case 2:
                    id = 10*(id_array[0]) + id_array[1];
                    break;
                    case 3:
                    id = 100*(id_array[0]) + 10*(id_array[1]) + id_array[2];
                    break;
                    case 4:
                    id = 1000*(id_array[0]) + 100*(id_array[1]) + 10*(id_array[2]) + id_array[3];
                    break;
                    default:
                    break;
                }
                mode = MODE_FIRST_FINGER;
            }
            else
            {
                
            }
        break;
        case MODE_FIRST_FINGER:
            lcd.setCursor(0,1);
            lcd.print("Dat ngon tay");
            p = finger.getImage();
            if(p == FINGERPRINT_OK)
            {
                digitalWrite(buzzer, HIGH);
                delay(100);
                digitalWrite(buzzer, LOW);
                delay(1000);
                p = finger.image2Tz(1);
                if(p == FINGERPRINT_OK)
                {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Them van tay");
                    lcd.setCursor(0,1);
                    lcd.print("Dat lai");
                    delay(2000);
                    mode = MODE_SECOND_FINGER;
                }
            }
        break;
        case MODE_SECOND_FINGER:
            p = finger.getImage();
            if(p == FINGERPRINT_OK)
            {
                digitalWrite(buzzer, HIGH);
                delay(100);
                digitalWrite(buzzer, LOW);
                delay(1000);
                p = finger.image2Tz(2);
                if(p == FINGERPRINT_OK)
                {
                    p = finger.createModel();
                    if(p == FINGERPRINT_OK)
                    {
                        p = finger.storeModel(id);
                        if(p == FINGERPRINT_OK)
                        {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Them van tay");
                            lcd.setCursor(2,1);
                            lcd.print("Thanh Cong");
                            delay(2000);
                            lcd.clear();
                            mode = MODE_RUN;
                        }
                    }
                    else if(p == FINGERPRINT_ENROLLMISMATCH)
                    {
                        lcd.setCursor(0,1);
                        lcd.print("Khong Khop");
                        delay(2000);
                        lcd.clear();
                        mode = MODE_RUN;
                    }
                    else
                    {
                      
                    }
                    id_count = 0;
                    id = 0;
                }
            }
        break;
        case MODE_RUN:
            real_time = rtc.getTime();
            lcd.setCursor(4,0);
            lcd.print("Welcome");
            lcd.setCursor(0,1);
            if(real_time.hour < 10)
            {
              lcd.print('0');
              lcd.print(real_time.hour);
            }
            else
            {
              lcd.print(real_time.hour);
            }
            lcd.print(":");
            if(real_time.min < 10)
            {
              lcd.print('0');
              lcd.print(real_time.min);
            }
            else
            {
              lcd.print(real_time.min);
            }
            lcd.print(":");
            if(real_time.sec < 10)
            {
              lcd.print('0');
              lcd.print(real_time.sec);
            }
            else
            {
              lcd.print(real_time.sec);
            }
            
            lcd.print(" ");
            if(real_time.date < 10)
            {
              lcd.print('0');
              lcd.print(real_time.date);
            }
            else
            {
              lcd.print(real_time.date);
            }
            lcd.print("/");
            if(real_time.mon < 10)
            {
              lcd.print('0');
              lcd.print(real_time.mon);
            }
            else
            {
              lcd.print(real_time.mon);
            }
            p = finger.getImage();
            if(p == FINGERPRINT_OK)
            {
                p = finger.image2Tz();
                if(p == FINGERPRINT_OK)
                {
                    p = finger.fingerFastSearch();
                    lcd.clear();
                    if(p == FINGERPRINT_OK)
                    {
                        digitalWrite(buzzer, HIGH);
                        delay(100);
                        digitalWrite(buzzer, LOW);
                        lcd.print("Success");
                        lcd.setCursor(6,1);
                        lcd.print("ID:");
                        lcd.print(finger.fingerID);
                        strore_time(finger.fingerID);
                    }
                    else
                    {
                        digitalWrite(buzzer, HIGH);
                        delay(100);
                        digitalWrite(buzzer, LOW);
                        delay(100);
                        digitalWrite(buzzer, HIGH);
                        delay(100);
                        digitalWrite(buzzer, LOW);
                        lcd.print("Not found");
                    }
                    
                    delay(2000);
                    lcd.clear();
                }
            }
        break;
        case MODE_ID_FINGER_CLEAR:
            lcd.setCursor(0,0);
            lcd.print("Xoa Van Tay");
            lcd.setCursor(0,1);
            lcd.print("ID:");
            if((key < 58)&&(key > 47))
            {
                id_array[id_count] = key - 48;
                id_count++;
                lcd.setCursor((6+id_count),1);
                lcd.print(key);
                key = 0;
            }
            else if(key == 'D')
            {
                switch(id_count)
                {
                    case 1:
                    id = id_array[0];
                    break;
                    case 2:
                    id = 10*(id_array[0]) + id_array[1];
                    break;
                    case 3:
                    id = 100*(id_array[0]) + 10*(id_array[1]) + id_array[2];
                    break;
                    case 4:
                    id = 1000*(id_array[0]) + 100*(id_array[1]) + 10*(id_array[2]) + id_array[3];
                    break;
                    default:
                    break;
                }
                p = finger.deleteModel(id);
                id_count = 0;
                if(p == FINGERPRINT_OK)
                {
                    lcd.setCursor(0,1);
                    lcd.print("Clear");
                    lcd.print(id);
                    lcd.print("success");
                    delay(2000);
                    lcd.clear();
                }
                else if(p == FINGERPRINT_BADLOCATION)
                {
                    lcd.setCursor(0,1);
                    lcd.print("id");
                    lcd.print(id);
                    lcd.print("invalid");
                    delay(2000);
                    lcd.clear();
                }
                else
                {
                    
                }
                key = 0;
                id = 0;
                mode = MODE_RUN;
            }
            else if(key == 'A')
            {
                p = finger.emptyDatabase();
                if(p == FINGERPRINT_OK)
                {
                    lcd.setCursor(0,1);
                    lcd.print("Clear all");
                    delay(2000);
                    lcd.clear();
                }
                key = 0;
                mode = MODE_RUN;
            }
            else
            {
                
            }
        break;
        case MODE_FIRST_PASS:
            /*LCD print*/
            lcd.setCursor(0,0);
            lcd.print("Mat Khau Moi");
            if((key < 58)&&(key > 47))
            {
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
                    first_pass = 1000*(buffer_pass[0]) + 100*(buffer_pass[1]) + 10*(buffer_pass[2]) + buffer_pass[3];
                    count_key = 0;
                    mode = MODE_SECOND_PASS;
                    delay(1000);
                    lcd.clear();
                }
            }
        break;
        case MODE_SECOND_PASS:
            lcd.setCursor(0,0);
            lcd.print("Nhap Lai");
            if((key < 58)&&(key > 47))
            {
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
                    second_pass = 1000*(buffer_pass[0]) + 100*(buffer_pass[1]) + 10*(buffer_pass[2]) + buffer_pass[3];
                    delay(1000);
                    lcd.clear();
                    lcd.setCursor(0,0);
                    if(second_pass == first_pass)
                    {
                        value_0 = second_pass/1000;
                        second_pass = second_pass - value_0*1000;
                        value_1 = second_pass/100;
                        second_pass = second_pass - value_1*100;
                        value_2 = second_pass/10;
                        second_pass = second_pass - value_2*10;
                        
                        EEPROM.write(0, value_0);
                        EEPROM.write(1, value_1);
                        EEPROM.write(2, value_2);
                        EEPROM.write(3, second_pass);
                        lcd.print("Thanh Cong");
                        delay(2000);
                        lcd.clear();
                    }
                    else
                    {
                        lcd.print("Khong Khop");
                        delay(2000);
                        lcd.clear();
                    }
                    count_key = 0;
                    mode = MODE_RUN;
                }
            }
        break;
        default:
        break;
    }
}

void process_mode(void)
{
  char status_pass = 0;
    switch(mode)
    {
        case MODE_CHANGE:
            if(time_lcd == 0)
            {
              lcd.clear();
              time_lcd = 1;
            }
            lcd.setCursor(0,0);
            lcd.print("Doi Mat Khau");
            lcd.setCursor(0,1);
            lcd.print("Nhap:");
            break;
        case MODE_ADD:
            if(time_lcd == 0)
            {
              lcd.clear();
              time_lcd = 1;
            }
            lcd.setCursor(0,0);
            lcd.print("Them van tay");
            lcd.setCursor(0,1);
            lcd.print("Nhap:");
            break;
        case MODE_CLEAR:
            if(time_lcd == 0)
            {
              lcd.clear();
              time_lcd = 1;
            }
            lcd.setCursor(0,0);
            lcd.print("Xoa van tay");
            lcd.setCursor(0,1);
            lcd.print("Nhap:");
            break;
        case MODE_CANCEL:
            mode = MODE_RUN;
              lcd.clear();
              time_lcd = 0;
              id_count = 0;
              count_key = 0;
        break;
        default:
        time_lcd = 0;
            break;
    }
    if(mode != MODE_RUN)
    {
    status_pass = check_password();
    if(status_pass == OK)
    {
        switch(mode)
        {
            case MODE_CHANGE:
                mode = MODE_FIRST_PASS;
                break;
            case MODE_ADD:
                mode = MODE_ID_FINGER_ADD;
                break;
            case MODE_CLEAR:
                mode = MODE_ID_FINGER_CLEAR;
                break;
            default:
                break;
        }
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Mat Khau Dung");
        delay(2000);
        lcd.clear();
    }
    else if(status_pass == NOT_OK)
    {
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Mat Khau Sai");
        delay(2000);
        lcd.clear();
        mode = MODE_RUN;
    }
    else
    {
    
    }
    }
}

char check_password(void)
{
    char value_0 = 0;
    char value_1 = 0;
    char value_2 = 0;
    char value_3 = 0;
    int pass  = 0;
    int pass_convert = 0;
    char status_pass = 0;
  
  if((key < 58)&&(key > 47))
  {
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
        pass_convert = 1000*(buffer_pass[0]) + 100*(buffer_pass[1]) + 10*(buffer_pass[2]) + buffer_pass[3];
        count_key = 0;
        
        value_0 = EEPROM.read(0);
        value_1 = EEPROM.read(1);
        value_2 = EEPROM.read(2);
        value_3 = EEPROM.read(3);
        pass = 1000*value_0 + 100*value_1 + 10*value_2 + value_3;
        if(pass_convert == pass)
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
void get_pass(void)
{
  char temp = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      key = temp;
      Serial.println(key);
    }
  }
  /* set up password*/   
  if((key == '*')&&(mode == MODE_RUN))
  {
      mode = MODE_CHANGE;
      Serial.println("change pass");
  }
  /* Add finger*/
  else if((key == '#')&&(mode == MODE_RUN))
  {
      mode = MODE_ADD;
  }
  /* Clear finger*/
  else if((key == 'C')&&(mode == MODE_RUN))
  {
      mode = MODE_CLEAR;
  }
  else if(key == 'B')
  {
      mode = MODE_CANCEL;
  }
  else
  {
  }
  temp = 0;
}

void strore_time(uint32_t id_finger)
{
  real_time = rtc.getTime();
  if(real_time.mon < 10)
  {
  sprintf(filename,"%d%d-%d",0,real_time.mon,real_time.year);
  }
  else
  {
    sprintf(filename,"%d-%d",real_time.mon,real_time.year);
  }
  while(!myFile)
  {
  myFile = SD.open(filename, FILE_WRITE);
  }
  if (myFile)
  {
      myFile.print("I");
      myFile.print(id_finger);
      myFile.print("S");
      myFile.print(real_time.sec);
      myFile.print("M");
      myFile.print(real_time.min);
      myFile.print("H");
      myFile.print(real_time.hour);
      myFile.print("D");
      myFile.print(real_time.date);
      myFile.print("N");
      myFile.print(real_time.mon);
      myFile.print("Y");
      myFile.print(real_time.year);
      myFile.println("E");
      myFile.close();
  }
  delay(500);
}


void recieve_data(void)
{
  int count = 0;
  while(Serial.available() > 0)
  {
    Export_day[count] = Serial.read() - 48;
    count++;
  }

  if(count != 0)
  {
    if(count == 6)
    {
      sprintf(filename_open,"%d%d-%d%d%d%d",Export_day[0],Export_day[1],Export_day[2],Export_day[3],Export_day[4],Export_day[5]);
      //Serial.write("hello");
        myFile = SD.open(filename_open);
        if (myFile) 
        {
          while (myFile.available()) 
          {
            Serial.write(myFile.read());
          }
          myFile.close();
        }
        else 
        {
          Serial.print("F");
        }
    }
  }

}
