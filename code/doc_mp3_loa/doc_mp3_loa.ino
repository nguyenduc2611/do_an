#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial DFmp3(3, 4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int LEDpin = 13;
int inputPin = 2;
int PIRs = LOW;
int val = 0;

void setup()
{
 pinMode(LEDpin, OUTPUT);
 pinMode(inputPin, INPUT);
 
 DFmp3.begin(9600);
 Serial.begin(9600);
 
 Serial.println();
 Serial.println(F("DFRobot DFPlayer Mini Demo"));
 Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
 
 if (!myDFPlayer.begin(DFmp3)) { //Use softwareSerial to communicate with mp3.
 Serial.println(F("Unable to begin:"));
 Serial.println(F("1.Please recheck the connection!"));
 Serial.println(F("2.Please insert the SD card!"));
 while(true);
 }
 Serial.println(F("DFPlayer Mini online."));
 
 myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
 
 //----Set volume----
 myDFPlayer.volume(20); //Set volume value (0~30).
 
 //----Set different EQ----
 myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
 
 //----Set device we use SD as default----
 myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

//----Mp3 play testing ----
 //play file in SD:/01/009.mp3; Folder Name(1~99); File Name(1~255)
 myDFPlayer.playFolder(1,9); 
 delay(2000);
 
 //----Read imformation----
 Serial.println(myDFPlayer.readState()); //read mp3 state
 Serial.println(myDFPlayer.readVolume()); //read current volume
 Serial.println(myDFPlayer.readEQ()); //read EQ setting
}

void loop()
{
 val = digitalRead(inputPin);
 if (val == HIGH)
 {
 digitalWrite(LEDpin, HIGH);
 //
 myDFPlayer.playFolder(1,2); 
 delay(1500);
 }
 else
 {
 digitalWrite(LEDpin, LOW);
 }
 delay(2000);
 
}
