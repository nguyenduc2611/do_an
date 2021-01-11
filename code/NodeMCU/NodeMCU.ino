#include <ThingerESP8266.h>

#define USERNAME "hieu"
#define DEVICE_ID "IPCS"
#define DEVICE_CREDENTIAL "5HnjMo?tEYo6"

#define SSID "AndroidAP"
#define SSID_PASSWORD "hieuhieu2"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["led"] << digitalPin(LED_BUILTIN);

  // resource output example (i.e. reading a sensor value)
  thing["millis"] >> outputValue(millis());
thing["data"] >> [](pson& out)
  {
  out = 2;
  };
  // more details at http://docs.thinger.io/arduino/
}

void loop() {
  thing["data"] >> [](pson& out)
  {
  out = 2;
  };
  thing.handle();
}
