#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SimpleTimer.h>
#include <BlynkSimpleEsp8266.h>
 
#define WIFI_SSID "AE-Cay-chuoi"
#define WIFI_PASS "minhchixauxa"
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "thienthan2105"
#define MQTT_PASS "aio_aAiK62cWzXsTIrjYQiptvgsxtPfF"

#define ON 1
#define OFF 2

char status_light = OFF;
char status_door = OFF;
char status_fan = OFF;
char status_mode_door = OFF;
uint32_t time_run = 0;
SimpleTimer timer;
char data[10] = {0};
char count = 0;
char temp = 0;
char hum = 0;
char string_data[5]={0};
char *ssid      = "AE-Cay-chuoi";               // Ten WiFi SSID
char *password  = "minhchixauxa";               // Mat khau wifi
char auth[] = "4jqveWFsQrX3zJIHfUzNrO4ZXJANYzAq";    //AuthToken copy ở Blynk Project
char count_send = 0;
//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
 
//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe LED = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/LED");
Adafruit_MQTT_Subscribe door = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/door");
Adafruit_MQTT_Subscribe fan = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/fan");
 
SoftwareSerial mySerial(D6, D5);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Blynk.begin(auth, ssid, password);
  timer.setInterval(500, sendUptime);
  Serial.println("OK!");
  //Subscribe to the onoff feed
  mqtt.subscribe(&LED);
  mqtt.subscribe(&door);
  mqtt.subscribe(&fan);
}

void sendUptime()
{
  /*Blynk.virtualWrite(V0, status_light);
  Blynk.virtualWrite(V1, status_door);
  Blynk.virtualWrite(V2, status_fan);*/

  sprintf(string_data,"%d%d%d%d",status_light,status_door,status_fan,status_mode_door);
  mySerial.write(string_data);
  //Serial.println(string_data);
  //Serial.println(status_light+48);
  //Serial.println(status_door+48);
  //Serial.println(status_fan+48);
  //Serial.println(status_mode_door+48);

  /*switch(count_send)
  {
    case 1:
    sprintf(string_data,"%d%d",3,status_light);
      mySerial.write(string_data);
    break;
    case 2:
    sprintf(string_data,"%d%d",4,status_door);
      mySerial.write(string_data);
    break;
    case 3:
    sprintf(string_data,"%d%d",5,status_fan);
      mySerial.write(string_data);
    break;
    case 4:
    sprintf(string_data,"%d%d",6,status_mode_door);
      mySerial.write(string_data);
    break;
    default:
    count_send = 0;
    break;
  }
  count_send++;*/
  status_door = OFF;
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V4, temp);
  Blynk.virtualWrite(V5, hum);
}

BLYNK_WRITE(V0)
{
  char light = param.asInt();
  if(light == 0)
  {
    status_light = OFF;
  }
  else if(light == 1)
  {
    status_light = ON;
  }
  else
  {
    
  }
}

BLYNK_WRITE(V1)
{
  char door = param.asInt();
  if(door == 0)
  {
    status_door = OFF;
    Serial.println("OFFV1");
  }
  else if(door == 1)
  {
    Serial.println("ONV1");
    status_door = ON;
  }
  else
  {
    
  }
}

BLYNK_WRITE(V2)
{
  char fan = param.asInt();
  if(fan == 0)
  {
    status_fan = OFF;
  }
  else if(fan == 1)
  {
    status_fan = ON;
  }
  else
  {
    
  }
}

BLYNK_WRITE(V3)
{
  char door = param.asInt();
  if(door == 0)
  {
    status_mode_door = OFF;
  }
  else if(door == 1)
  {
    status_mode_door = ON;
  }
  else
  {
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while(mySerial.available() > 0)
  {
    data[count] = mySerial.read() - 48;
    count++;
  }

  if(count != 0)
  {
    //Serial.println(data);
    hum = 10*data[0] + data[1];
    temp = 10*data[2] + data[3];
    //Serial.println(hum);
    //Serial.println(temp);
    count = 0;
  }
  Voice_Control();
  Blynk.run();
  timer.run();
}

void Voice_Control(void)
{
    MQTT_connect();
  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(2000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &LED)
    {
      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) LED.lastread);
     //If the new value is  "ON", turn the light on.
     //Otherwise, turn it off.
      if (!strcmp((char*) LED.lastread, "ON"))
      {
        //Active low logic
        Serial.println("ON");
        status_light = ON;
      }
      else
      {
        Serial.println("OFF");
        status_light = OFF;
      }
    }


    if (subscription == &door)
    {
      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) door.lastread);
     //If the new value is  "ON", turn the light on.
     //Otherwise, turn it off.
      if (!strcmp((char*) door.lastread, "ON"))
      {
        //Active low logic
        Serial.println("ON");
        status_door = ON;
      }
      else
      {
        Serial.println("OFF");
        status_door = OFF;
      }
    }


    if (subscription == &fan)
    {
      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) fan.lastread);
     //If the new value is  "ON", turn the light on.
     //Otherwise, turn it off.
      if (!strcmp((char*) fan.lastread, "ON"))
      {
        //Active low logic
        Serial.println("ON");
        status_fan = ON;
      }
      else
      {
        Serial.println("OFF");
        status_fan = OFF;
      }
    }
  }
 
  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

void MQTT_connect()
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0)
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
