#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define GPS_Stt 12
#define SIM_Stt 9
#define	Led_Stt 11
#define PIN_TX    5
#define PIN_RX    6
#define BAUDRATE  9600
#define PHONE_NUMBER "0367988906"
#define MESSAGE  "He thong khoi dong"

String text="Location";

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
unsigned int messageIndex = 0;

char phone[16];
char datetime[24];

TinyGPS gps;
SoftwareSerial ss(2, 3);
GPRS gprs(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

char buffer_send_sms[50];

void clear_buff_message(void)
{
  int i=0;
  for(i=0;i<MESSAGE_LENGTH;i++)
  {
    message[i]='\0';
  }
}

void Get_GPS();
void Get_SMS();

void setup()
{
	pinMode(GPS_Stt,OUTPUT);
	pinMode(Led_Stt,OUTPUT);
	pinMode(SIM_Stt,OUTPUT);
	digitalWrite(GPS_Stt,HIGH);
	digitalWrite(Led_Stt,HIGH);
	digitalWrite(SIM_Stt,HIGH);
	gprs.checkPowerUp();
	Serial.begin(9600);
	ss.begin(9600);	
	GPRS gprs(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate
	while(!gprs.init()) {
      delay(1000);
      Serial.println("Initialization failed!");
	} 
	Serial.println("gprs initialize done!");
	Serial.println("start to send message ...");
	digitalWrite(SIM_Stt,LOW);
  
	/* if(gprs.sendSMS(PHONE_NUMBER,MESSAGE)) //define phone number and text
	{
		digitalWrite(Led_Stt,LOW);
		Serial.print("Send SMS Succeed!\r\n");
	}
	else {
		digitalWrite(Led_Stt,HIGH);
		Serial.print("Send SMS failed!\r\n");
	} */
  
}

void loop()
{
	Get_GPS();
	//delay(500);
	Get_SMS();
}

void Get_GPS()
{
	ss.begin(9600);	
	Serial.println("Get_GPS");
	char kinhdo_t[10];
	char vido_t[10];
	bool newData = false;
	unsigned long chars;
    unsigned short sentences, failed;
	for (unsigned long start = millis(); millis() - start < 1000;)
	{
		while (ss.available())
		{
		char c = ss.read();
		//Serial.write(c); // uncomment this line if you want to see the GPS data flowing
		if (gps.encode(c)) // Did a new valid sentence come in?
			newData = true;
		}
	}
	if (newData)
	{
		float flat, flon; 
		float kinhdo, vido;
		unsigned long age;
		gps.f_get_position(&flat, &flon, &age);
		Serial.print("LAT=");
		kinhdo=(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon);
		vido=(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat);
		//Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
		Serial.print(vido, 6);
		Serial.print(" LON=");
		//Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
		Serial.print(kinhdo, 6);
		//Serial.print(" SAT=");
		//Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
		//Serial.print(" PREC=");
	    //Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop()); 
		digitalWrite(GPS_Stt,LOW);
		dtostrf(kinhdo,4,6,kinhdo_t);
		dtostrf(vido,4,6,vido_t);
		sprintf(buffer_send_sms,"%s,%s",kinhdo_t,vido_t);
		//Serial.println(buffer_send_sms);
	  }
	  else
	  {
		gps.stats(&chars, &sentences, &failed);
		/* Serial.print(" CHARS=");
		Serial.print(chars);
		Serial.print(" SENTENCES=");
		Serial.print(sentences);
		Serial.print(" CSUM ERR="); */
		//Serial.println(failed);
		if (chars == 0)
			Serial.println("** No characters received from GPS: check wiring **");
		sprintf(buffer_send_sms,"Chua xac dinh duoc toa do\r\n");
	}
	
}
void Get_SMS()
{
	GPRS gprs(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate
	Serial.println("Get_SMS");
	messageIndex = gprs.isSMSunread();
	Serial.print("messageIndex: ");
	Serial.println(messageIndex);
		
	gprs.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
	//Serial.print("Recieved Message: ");
	//Serial.println(message);
	//Serial.println(strlen(message));
	  if(strlen(message)>0)
	  {
		  digitalWrite(Led_Stt,LOW);
		/* Serial.println("Recieved Message: ");
		Serial.println(message);
		Serial.println(strlen(message));
		Serial.print("Phone number: ");
		Serial.println(phone); */
      if(text.equalsIgnoreCase(message))
		{
			sprintf(buffer_send_sms,buffer_send_sms);
			gprs.sendSMS(phone,buffer_send_sms);
			digitalWrite(Led_Stt,HIGH);
			
		}
	  else
		{
			gprs.sendSMS(PHONE_NUMBER,"Sai cu phap. Vui long gui tin nhan theo cu phap: Location");
			digitalWrite(Led_Stt,HIGH);
		}
		gprs.deleteSMS(messageIndex);
		clear_buff_message();
  }	

}
