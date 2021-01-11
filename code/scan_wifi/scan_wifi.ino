#include <ESP8266WiFi.h>

void setup()
 {
    Serial.begin(115200);

    Serial.println("** Scan Networks **");

    int numSsid = WiFi.scanNetworks();

    Serial.print("SSID List:");
    Serial.println(numSsid);


 }
void loop()

 {

 }
