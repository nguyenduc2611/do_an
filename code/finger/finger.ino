#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>

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
volatile uint32_t pass_finger = 0;
uint32_t buffer1_pass = 0;
uint32_t buffer2_pass = 0;
char is_First = FIRST_PASS;
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
volatile int mode = RUN_NORMAL;
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

byte rowPins[rows] = {5, 6, 7, 8};
byte columnPins[columns] = {9, 10, 11, 12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial,pass_finger);

void setup()  
{
  Timer1.initialize(200000); 
  Timer1.attachInterrupt(get_pass);
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  finger.begin(57600);
  delay(5);
}

void loop()                  
{
  if(mode != RUN_NORMAL)
  {
  if(state == CHECK_PASS)
  {
    Serial.println(pass_finger);
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial,pass_finger);
    if(finger.verifyPassword())
    {
        Serial.println("Password is correct!");
        if(mode == CHANGE_PASS)
        {
            Serial.println("Enter a new password");
        }
        state = PASS_OK;
    }
    else
    {
        Serial.println("Password is incorrect!");
        Serial.println(pass_finger);
        state = PASS_NOT_OK;
        mode = RUN_NORMAL;
    }
  }
  if(state == FIRST_PASS)
  {
      Serial.println("Enter a new password again");
      state = COMPARE_PASS;
      
  }
  else if((state == COMPARE_PASS)||(state == COM_PASS))
  {
    if(state == COM_PASS)
    {
      Serial.println(buffer2_pass);
    if(buffer1_pass == buffer2_pass)
    {
      Serial.println(buffer2_pass);
          finger.setPassword(buffer2_pass);
          Serial.println("change password success");
    }
    else
    {
          Serial.println("confirm not ok");
    }
    mode = RUN_NORMAL;
    state = PASS_NOT_OK;
    }
  }
  }
  else
  {
    //Serial.println(pass);
    delay(500);
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
      Serial.println(temp);
    }
  }
  if((temp == '*')&&(mode == RUN_NORMAL))
  {
    mode = CHANGE_PASS;
    Serial.println("nhap mat khau:");
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
  else
  {
      
  }
  
  temp = 0;
}
