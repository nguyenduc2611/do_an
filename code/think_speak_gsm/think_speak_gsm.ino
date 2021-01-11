#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial mySerial(2, 3);

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

int led1 =  10;
int led2 =  11;
int MyValue1, MyValue2;
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
float phValue;
void setup() {
 mySerial.begin(9600);
 ss.begin(GPSBaud);
 Serial.begin(9600);
 pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  DO_PH();
Connect2Server();       // Establishing connection
Field1(phValue);       // Uploading to field 1 of Thingspeak
EndConnection();        // Terminating connection

Connect2Server();       // Establishing connection
Field2(gps.location.lat());
EndConnection();        // Terminating connection


Connect2Server();       // Establishing connection
Field3(gps.location.lng());
EndConnection();        // Terminating connection

  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  smartDelay(1000);

/*Connect2Server();
Field2(MyValue2);     // Uploading to field 2 of Thingspeak 
EndConnection(); */

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
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
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
  String str = "GET http://api.thingspeak.com/update?api_key=BOI36T6ZEB427OG7&field1=" + String (data);
  mySerial.println(str);//begin send data to remote server

  delay(2000);
  ShowSerialData();
  mySerial.println((char)26);//sending
  delay(4000);//waitting for reply, important! the time is base on the condition of internet
  mySerial.println();

  ShowSerialData();

}

void Field2(float data) {
  String str = "GET http://api.thingspeak.com/update?api_key=BOI36T6ZEB427OG7&field2=" + String (data);
  mySerial.println(str);//begin send data to remote server

  delay(2000);
  ShowSerialData();
  mySerial.println((char)26);//sending
  delay(4000);//waitting for reply, important! the time is base on the condition of internet
  mySerial.println();

  ShowSerialData();

}


void Field3(float data) {
  String str = "GET http://api.thingspeak.com/update?api_key=BOI36T6ZEB427OG7&field3=" + String (data);
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
void DO_PH(void)
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(A4);
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
  phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.println(phValue);
if(phValue < 5.7)
{
  phValue = phValue -1;
}
else if(phValue > 6.8)
{
  phValue = phValue + 2;
}
else
{
  phValue = phValue + 1;
}

  if(phValue<6)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  else if((phValue > 6)&&(phValue < 8))
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
  else
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
}
