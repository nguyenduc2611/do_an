#include <Wire.h>
#include <SPI.h>

#include <DS3231.h>
DS3231 rtc(20, 21);
Time thoi;
int i = 0;
void setup () {
  rtc.begin();      // Khoi dong RTC
  delay(500);
  Serial.begin(9600); // Khoi dong serial port de lay du lieu
  rtc.setTime(16, 14,59);
  rtc.setDate(28,10,2019);
  rtc.setDOW(1);

}
void loop() {
  // Send date
  thoi = rtc.getTime();

  // Send time
  Serial.println(thoi.sec);


  // Wait one second before repeating
  delay (500);
}
