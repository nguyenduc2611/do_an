#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>
#define BUTTON 7
SoftwareSerial mySerial(10, 11);
LiquidCrystal_I2C lcd(0x27,16,2);
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

float lo = 0;
float la = 0;
int c = 0;
int buf[10],temp;
char sim_AT[100] = {0};
float phValue;
char lcd_flag = 0;
unsigned long int avgValue; 
void setup() {
  // put your setup code here, to run once:
  //delay(20000); 
Serial.begin(9600);
Serial.println("ret");
  mySerial.begin(9600);
  delay(500);
  mySerial.write("AT+CMGF=1");
  mySerial.write(10);
  delay(500);
  mySerial.write("AT+CNMI=2,2");
  mySerial.write(10);
  delay(3000);
  while(mySerial.available())
  {
    sim_AT[c] = mySerial.read();
    Serial.println(sim_AT[c]);
    c++;
  }
  pinMode(BUTTON, INPUT);
  lcd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float PH = 0;
  char flag_content = 0;
  char a = digitalRead(BUTTON);
  int count_gps = 0;

Connect2Server();       // Establishing connection
Field1(PH);       // Uploading to field 1 of Thingspeak
EndConnection();        // Terminating connection

  if(!lcd_flag)
  {
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Welcome!");
    lcd_flag = 1;
  }
  if(a == HIGH)
  {
    delay(200);
      a = digitalRead(BUTTON);
      if(a == HIGH)
      {
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Waitting!");
        PH = Do_PH();
        while((!flag_content)&&(count_gps < 100))
        {
          Serial.println("hello1");
          ss.begin(GPSBaud);
          printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
          printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
          delay(50);
          count_gps++;
          if(lo!= 0)
          {
            Serial.println("hello");
            send_sms(PH);
            flag_content = 1;
          }
        }
        lcd_flag = 0;
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Gia Tri PH:");
        lcd.setCursor(5,1);
        lcd.print(PH, 2);
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Long: ");
        lcd.print(lo, 6);
        lcd.setCursor(0,1);
        lcd.print("Lat:  ");
        lcd.print(la, 6);
        delay(3000);
      }
    
  }
  
}

float Do_PH(void)
{
    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(A1);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float pHVol=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=5.70 * pHVol - 21.34;;                      //convert the millivolt into pH value
  Serial.println(phValue);
  return phValue;
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid) //neu 0 bt dc toa do
  {
    while (len-- > 1); //show len debug kí tu *
  }
  else //bat dc toa do
  {
    lo = gps.location.lng();
    la = gps.location.lat();
  }
  smartDelay(0);
}

void send_sms(float value_PH)
{
    mySerial.begin(9600);
    Serial.println("SEND ok");
    mySerial.write("AT");
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGF=1"); //cai dat module sim che do nhan tin
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGS="); //batdau cu phap text(AT+CMGS="sdt"
    mySerial.write(34);
    mySerial.write("0355673366");
    mySerial.write(34);
    mySerial.write(10); //dau enter
    delay(1000);
    mySerial.write("Gia tri PH la:");
    mySerial.print(value_PH,2);
    mySerial.write(10);
    mySerial.write("Tai vi tri:");
    mySerial.print(gps.location.lng(),6); //so nguyen toa do
    mySerial.write(",");
    mySerial.print(gps.location.lat(),6);
    delay(1000);
    mySerial.write(26); //ket thuc text =send
}


void Connect2Server()
{
  mySerial.println("AT");
  delay(200);

  mySerial.println("AT+CPIN?");
  delay(200);

  mySerial.println("AT+CREG?");
  delay(200);

  mySerial.println("AT+CGATT?");
  delay(200);

  mySerial.println("AT+CIPSHUT");
  delay(200);

  mySerial.println("AT+CIPSTATUS");
  delay(1000);

  mySerial.println("AT+CIPMUX=0");
  delay(1000);

  ShowSerialData();

  mySerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(200);

  ShowSerialData();
   
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(2000);

  ShowSerialData();

  mySerial.println("AT+CIFSR");//get local IP adress
  delay(2000);

  ShowSerialData();

  mySerial.println("AT+CIPSPRT=0");
  delay(1000);

  ShowSerialData();

  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(2000);

  ShowSerialData();

  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(2000);
  ShowSerialData();
}

void Field1(float data) {
  String str = "GET http://api.thingspeak.com/update?api_key=QR2E7IAMSB72SCVY&field1=" + String (data);
  mySerial.println(str);//begin send data to remote server

  delay(2000);
  ShowSerialData();
  mySerial.println((char)26);//sending
  delay(4000);//waitting for reply, important! the time is base on the condition of internet
  mySerial.println();

  ShowSerialData();

}

void EndConnection() {
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();

}

void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
