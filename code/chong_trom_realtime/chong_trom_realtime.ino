#define BLYNK_PRINT Serial
#include <NTPtimeESP.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#define PIR_SENSOR D7
#define RELAY_BUZZER D3

NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
char *ssid      = "Duc Long Electronics";               // Ten WiFi SSID
char *password  = "linhkienhadong.vn";               // Mat khau wifi
char auth[] = "fpFEAnETEyCzBftHFCXW1AJFTguJAh01";    //AuthToken copy ở Blynk Project
char status_update = 0;
int hour_time1 = 0;
int min_time1 = 0;
int sec_time1 = 0;
int hour_time2 = 0;
int min_time2 = 0;
int sec_time2 = 0;
char send_time1 = 0;
int time1 = 0;
int time2 = 0;
int curent_time = 0;
char status_light = 0;
int time_run = 0;
char status_led = 0;
char mode_time_handle = 0;
char mode_time_auto = 0;
char amount_time = 0;
byte actualHour = 0;
byte actualMinute = 0;


SoftwareSerial simSerial(D6, D5);
strDateTime dateTime;
void setup() {
  char t = 0;
  Serial.begin(115200);
  simSerial.begin(9600);
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  /* khai báo chân cảm biến*/
  pinMode(PIR_SENSOR, INPUT);
  pinMode(RELAY_BUZZER, OUTPUT);

/*kết nối wifi*/
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
/* kết nối blynk*/
  Blynk.begin(auth, ssid, password);

  /*Khởi tạo eeprom*/
  EEPROM.begin(512);

/*đọc dữ liệu eeprom*/
  hour_time1 = char(EEPROM.read(0));
  min_time1 = char(EEPROM.read(1));
  hour_time2 = char(EEPROM.read(2));
  min_time2 = char(EEPROM.read(3));
/*mode hệ thống*/
  mode_time_handle = char(EEPROM.read(9));
  mode_time_auto = char(EEPROM.read(10));


}

/*blynk bật tắt hệ thống bằng tay*/
BLYNK_WRITE(V3)
{
  status_led= param.asInt();
}

/*Mode tự động*/
BLYNK_WRITE(V1)
{
  mode_time_auto= param.asInt();
  EEPROM.write(10, mode_time_auto);
    EEPROM.commit();
  if(mode_time_auto == 1)
  {
    mode_time_handle = 0;
    Blynk.virtualWrite(V5, mode_time_handle);
  }
}

/*gửi dữ liệu thời gian xuống ESP*/
BLYNK_WRITE(V4)
{
  send_time1= param.asInt();
}

/*mode hệ thống điều khiển bằng tay*/
BLYNK_WRITE(V5)
{
  mode_time_handle= param.asInt();
  Serial.println("hello");
  EEPROM.write(9, mode_time_handle);
  EEPROM.commit();
  if(mode_time_handle == 1)
  {
    mode_time_auto = 0;
    Blynk.virtualWrite(V1, mode_time_auto);
  }
}
/* cài đặt thời gian*/
BLYNK_WRITE(V6)
{
  Serial.println("send");
  TimeInputParam t(param);
  if(t.hasStartTime())
  {
    hour_time1 = t.getStartHour();
    min_time1 = t.getStartMinute();
    sec_time1 = t.getStartSecond();
    
  }
  if(t.hasStopTime())
  {
    hour_time2 = t.getStopHour();
    min_time2 = t.getStopMinute();
    sec_time2 = t.getStartSecond();
  }
}

void loop() {

  // Tham so dau tien la Time zone duoi dang floating point (7.0 la VN); 
  // Tham so thu hai la DayLightSaving: 1 voi thoi gian; 2 la thoi gian US (o VN khong su dung)
  /*lấy thời gian từ internet*/
  dateTime = NTPch.getNTPtime(7.0, 0);
  
  // Kiem tra dateTime.valid truoc khi dua ra gia tri thoi gian
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

     actualHour = dateTime.hour;      // Gio
     actualMinute = dateTime.minute;  // Phut
    byte actualsecond = dateTime.second;  // Giay
    int actualyear = dateTime.year;       // Nam
    byte actualMonth = dateTime.month;    // Thang
    byte actualday =dateTime.day;         // Ngay
    byte actualdayofWeek = dateTime.dayofWeek;
  }

/*xư lý chế độ điều khiển bằng tay*/
  if(mode_time_handle == 1)
  {
    if(status_led == 0)
    {
      status_light = 0;
    }
    else
    {
      status_light = 1;
    }
  }

  /*Xử lý dữ liệu điều khiển tự động*/
  if(mode_time_auto == 1)
  {
    if(send_time1 == 1)
  {
    /*khi có dữ liệu cài đặt mới gửi từ blynk sẽ lưu lại trong eeprom để đề phòng mất điện sẽ ko phải cài đặt lại*/
    EEPROM.write(0, hour_time1);
    EEPROM.write(1, min_time1);
    EEPROM.write(2, hour_time2);
    EEPROM.write(3, min_time2);
    EEPROM.commit();
    send_time1 = 0;
    Blynk.virtualWrite(V4, send_time1);
  }
/* tính thời gian quy đổi ra phút*/
  curent_time = 60 * actualHour+ actualMinute;
  time1 = 60 * hour_time1 + min_time1;
  time2 = 60 * hour_time2 + min_time2;
    if((curent_time >= time1)&&(curent_time <= time2))
    {
      Serial.println("ONLED");
      status_light = 1;
    }
    else
    {
      Serial.println("OFFLED");
      status_light = 0;
    }
  
  
  }


  /* đọc giá trị cảm biến chuyển động*/
  char ir_status = digitalRead(PIR_SENSOR);
  /*nếu hệ thống bật*/
  if(status_light == 1)
  {
    /*check giá trị cảm biến chuyển đông*/
    if(ir_status == 1)
    {/* còi báo động*/
      digitalWrite(RELAY_BUZZER, HIGH);
      /*Kiểm tra giao tiếp với module sim*/
      simSerial.write("AT");
      simSerial.write(10);
      delay(1000);
      /*cấu hình module Sim gửi tin nhắn*/
      simSerial.write("AT+CMGF=1");
      simSerial.write(10);
      delay(1000);
      /*bắt đầu soạn tin nhắn*/
      simSerial.write("AT+CMGS=");
      simSerial.write(34);
      simSerial.write("0356325993");
      simSerial.write(34);
      simSerial.write(10);
      delay(1000);
      simSerial.write("Co trom");
      delay(1000);
      simSerial.write(26);
      delay(5000);
      digitalWrite(RELAY_BUZZER, LOW);
    }
    /*hiển thị trên blynk có trộm hay ko*/
    Blynk.virtualWrite(V0, 1);
  }
  else
  {
    Blynk.virtualWrite(V0, 0);
  }
  delay(500);
  /*hàm để run blunk*/
  Blynk.run();
}
