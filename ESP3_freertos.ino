#define BLYNK_TEMPLATE_ID "TMPL6dvQWpIU9"
#define BLYNK_TEMPLATE_NAME "dat"
#define BLYNK_AUTH_TOKEN "wVVstedmE9KQ9x_vO-Ln12fBS0thL_tQ"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "time.h"
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#include <Arduino.h>
QueueHandle_t queue1;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 7;
const int   daylightOffset_sec = 0;

char ssid[] = "TP-LINK_436132"; // ten wifi
char pass[] = "123456789"; // mat khau wifi
char timeHour[3];
char timeMin[4];
#define LED_BUILTIN 4
int hum = 0;
char status_bom = 0;
int a = 0;
int hour_temp = 0;
int pinValue = 0;
int ViMode = 0;
// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogReadA3( void *pvParameters );
void TaskRole( void *pvParameters );
// the setup function runs once when you press reset or power the board

BLYNK_WRITE(V1) // this command is listening when something is written to V1
{
  pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V3) // this command is listening when something is written to V1
{
  ViMode = param.asInt(); // assigning incoming value from pin V1 to a variable
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  Serial.println("Time variables");
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  strftime(timeMin,4, "%M", &timeinfo);
  Serial.println(timeMin);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(9600);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  queue1 = xQueueCreate(5, sizeof(int));

  if (queue1 == NULL) {

    Serial.println("Queue can not be created");

  }
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskBlink
    ,  "TaskBlink"   // A name just for humans
    ,  8192  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskAnalogReadA3
    ,  "AnalogReadA3"
    ,  8192  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(
    TaskRole
    ,  "TaskBlink"   // A name just for humans
    ,  8192  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  vTaskStartScheduler();
}

void loop()
{
  
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
	int hum = 0;
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
    
  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  

  for (;;) // A Task shall never return or exit.
  {
	  if (xQueueReceive(queue1, &hum, portMAX_DELAY) == pdPASS) {
		printLocalTime();
		Blynk.virtualWrite(V0, hum);
    
		Blynk.run();
	  }
  }
}

void TaskRole(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
    
  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  

  for (;;) // A Task shall never return or exit.
  {
    if((pinValue == 0)&&(ViMode == 0)&&(status_bom == 1))
    {
      digitalWrite(LED_BUILTIN, LOW);
      status_bom = 0;
      Blynk.virtualWrite(V1, status_bom);
    }
    if((pinValue == 1)&&(ViMode == 0)&&(status_bom == 0))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      status_bom = 1;
      Blynk.virtualWrite(V1, status_bom);
    }
    if((ViMode == 1)&&(hour_temp >= 8)&&(hour_temp <= 17)&&(hum < 50)&&(status_bom == 0))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      status_bom = 1;
      Blynk.virtualWrite(V1, status_bom);
    }
    if((ViMode == 1)&&(hour_temp >= 8)&&(hour_temp <= 17)&&(hum > 50)&&(status_bom == 1))
    {
      digitalWrite(LED_BUILTIN, LOW);
      status_bom = 0;
      Blynk.virtualWrite(V1, status_bom);
    }
  }
}


void TaskAnalogReadA3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  pinMode(LED_BUILTIN, OUTPUT);
  for (;;)
  {
    hour_temp = timeHour[3];
    // read the input on analog pin A3:
    int sensorValueA3 = analogRead(34);
    hum = 100*(4095 - sensorValueA3)/4095;
    // print out the value you read:
    //Serial.println(hum);
    xQueueSend(queue1, &hum, portMAX_DELAY);
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}
