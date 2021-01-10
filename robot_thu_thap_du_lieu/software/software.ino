#include <WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>

#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#define SECRET_SSID "MySSID"    // replace MySSID with your WiFi network name
#define SECRET_PASS "MyPassword"  // replace MyPassword with your WiFi password

#define SECRET_CH_ID 000000     // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "XYZ"   // replace XYZ with your channel write API Key
//DHT config
#define DHTPIN 33     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
// Wi-Fi Settings
const char* ssid = "AE-Cay-chuoi"; // your wireless network name (SSID)
const char* password = "minhchixauxa"; // your Wi-Fi network password

WiFiClient client;

// ThingSpeak Settings
unsigned long channelID = 1270884; //
const char * writeAPIKey = "36B28JNUBJ9QF40N"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 2 * 1000; // post data every 2 seconds

void setup() {
        Serial.begin(115200);
        dht.begin();
        Serial.print("Connecting");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(100);
        }
        Serial.println("\r\nWiFi connected");
        ThingSpeak.begin(client);
}

void loop() {
        // wait and then post again
        delay(postingInterval);

        float temp = dht.readTemperature();
        float humi = dht.readHumidity();
        Serial.println(temp);
        Serial.println(humi);
        if (isnan(temp) || isnan(humi)) {
                Serial.println("Failed to read from DHT sensor!");
                return;
        }

        else
        {
          int x = ThingSpeak.writeField(channelID, 1, temp, writeAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
        }

}
