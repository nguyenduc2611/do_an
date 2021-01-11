#include <SPI.h>
#include <RFID.h>

#define SS_PIN 53

#define RST_PIN 49

RFID rfid(SS_PIN, RST_PIN);
unsigned char reading_card[5];
unsigned char i, j;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}
void loop()
{
  if (rfid.isCard())
  {
    if (rfid.readCardSerial())
    {
      for (i = 0; i < 5; i++)
      {
        reading_card[i] = rfid.serNum[i];
        Serial.print((reading_card[i]));
      }
    }
    rfid.halt();
  }
  Serial.println("here");
  delay(1000);
}
