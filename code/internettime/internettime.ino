#include <NTPtimeESP.h>
#include <SoftwareSerial.h>
NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
char *ssid      = "dienthoai";               // Ten WiFi SSID
char *password  = "10101010";               // Mat khau wifi

/*
 * Cac truong co trong struct DateTime:
 * struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};
 */
strDateTime dateTime;
SoftwareSerial mySerial(D6, D5);
void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
}

void loop() {

  // Tham so dau tien la Time zone duoi dang floating point (7.0 la VN); 
  // Tham so thu hai la DayLightSaving: 1 voi thoi gian; 2 la thoi gian US (o VN khong su dung)
  dateTime = NTPch.getNTPtime(7.0, 0);
  
  // Kiem tra dateTime.valid truoc khi dua ra gia tri thoi gian
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Gio
    byte actualMinute = dateTime.minute;  // Phut
    byte actualsecond = dateTime.second;  // Giay
    int actualyear = dateTime.year;       // Nam
    byte actualMonth = dateTime.month;    // Thang
    byte actualday =dateTime.day;         // Ngay
    byte actualdayofWeek = dateTime.dayofWeek;
    if(actualsecond < 10)
    {
      mySerial.print('0');
      mySerial.print(actualsecond);
    }
    else
    {
      mySerial.print(actualsecond);
    }

    if(actualMinute < 10)
    {
      mySerial.print('0');
      mySerial.print(actualMinute);
    }
    else
    {
      mySerial.print(actualMinute);
    }

    if(actualHour < 10)
    {
      mySerial.print('0');
      mySerial.print(actualHour);
    }
    else
    {
      mySerial.print(actualHour);
    }

    if(actualday < 10)
    {
      mySerial.print('0');
      mySerial.print(actualday);
    }
    else
    {
      mySerial.print(actualday);
    }
    mySerial.print(actualdayofWeek);

    if(actualMonth < 10)
    {
      mySerial.print('0');
      mySerial.print(actualMonth);
    }
    else
    {
      mySerial.print(actualMonth);
    }
    
    mySerial.print(actualyear);
  }
}
