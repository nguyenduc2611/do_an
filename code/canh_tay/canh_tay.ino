#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

#define SERVOR_1 5
#define SERVOR_2 6
#define SERVOR_3 9
#define SERVOR_4 10

#define S0 2
#define S1 3
#define S2 4
#define S3 7
#define sensorOut 8

#define YES 0
#define NO 1
#define GREEN 2
#define RED 3
#define BLUE 4
#define PIN_SCAN 12

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int i = 0;
void setup() {
  // put your setup code here, to run once:
  myservo1.attach(SERVOR_1);
  myservo2.attach(SERVOR_2);
  myservo3.attach(SERVOR_3);
  myservo4.attach(SERVOR_4);

  pinMode(PIN_SCAN, INPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
  //
  //delay(5000);
  
//delay(5000);
  first_pos();
}

void loop() {
  //Pick_Position_1();
  /*for(i = 20; i < 60; i++)
  {
    myservo1.write(i);
    Serial.println(i);
    delay(50);
  }*/
  
  char color_obj = NO;
  color_obj = scan_obj();

  switch(color_obj)
  {
    case RED:
    Serial.println("RED");
    Pick_Position_1();
    myservo4.write(150);
    delay(100);
    Release_Position_2();
    delay(100);
    myservo4.write(90);
    break;
    case GREEN:
    Serial.println("GREEN");
    Pick_Position_1();
    delay(200);
    myservo4.write(180);
    Release_Position_2();
    delay(100);
    myservo4.write(90);
    break;
    case BLUE:
    Serial.println("BLUE");
    Pick_Position_1();
    myservo4.write(30);
    delay(100);
    Release_Position_2();
    delay(100);
    myservo4.write(90);
    break;
    default:
    Serial.println("DEFAULT");
    break;
  }
}

char scan_obj(void)
{
    char status_obj = NO;
    int time_scan = 0;
    char red_time = 0;
    char green_time = 0;
    char blue_time = 0;
    
    while(time_scan != 10)
    {
        while(status_obj == NO)
        {
            status_obj = digitalRead(PIN_SCAN);
            delay(1000);
        }
        
        // Setting RED (R) filtered photodiodes to be read
        digitalWrite(S2,LOW);
        digitalWrite(S3,LOW);
        
        // Reading the output frequency
        redFrequency = pulseIn(sensorOut, LOW);
        // Remaping the value of the RED (R) frequency from 0 to 255
        // You must replace with your own values. Here's an example: 
        redColor = map(redFrequency, 70, 120, 255,0);
        delay(100);
        
        // Setting GREEN (G) filtered photodiodes to be read
        digitalWrite(S2,HIGH);
        digitalWrite(S3,HIGH);
        
        // Reading the output frequency
        greenFrequency = pulseIn(sensorOut, LOW);
        // Remaping the value of the GREEN (G) frequency from 0 to 255
        // You must replace with your own values. Here's an example: 
        greenColor = map(greenFrequency, 100, 199, 255, 0);
        delay(100);
        
        digitalWrite(S2,LOW);
        digitalWrite(S3,HIGH);
        
        // Reading the output frequency
        blueFrequency = pulseIn(sensorOut, LOW);
        // Remaping the value of the BLUE (B) frequency from 0 to 255
        // You must replace with your own values. Here's an example: 
        blueColor = map(blueFrequency, 100, 199, 255, 0);
        delay(100);
        
        // Checks the current detected color and prints
        // a message in the serial monitor
        if((redColor > greenColor) && (redColor > blueColor)){
            Serial.println(" - RED detected!");
            red_time++;
        }
        else if((greenColor > redColor) && (greenColor > blueColor)){
            Serial.println(" - GREEN detected!");
            green_time++;
        }
        else if((blueColor > redColor) && (blueColor > greenColor)){
            Serial.println(" - BLUE detected!");
            blue_time++;
        }
        else
        {
          Serial.println("khong tim dc mau");
        }
        time_scan++;
    }
    
    if(7 <= red_time)
    {
        status_obj = RED;
    }
    else if(7 <= blue_time)
    {
        status_obj = BLUE;
    }
    else if(7 <= green_time)
    {
        status_obj = GREEN;
    }
    else
    {
    }
    
    return status_obj;
}

void first_pos(void)
{
  myservo1.write(20);
  delay(500);
  myservo2.write(90);
  delay(500);
  myservo3.write(90);
  delay(500);
  myservo4.write(93);
  delay(500);
}

void Pick_Position_1(void)
{
  Serial.println("here");
  myservo4.write(90);
  delay(100);
  for(i=90; i < 145; i++)
  {
    myservo3.write(i);
    delay(10);
  }
  for(i = 90; i > 10; i--)
  {
    myservo2.write(i);
    delay(15);
  }

  for(i = 20; i<50;i++)
  {
    myservo1.write(i);
    delay(50);
  }

for(i=150; i > 145; i--)
  {
    myservo3.write(i);
    delay(15);
  }
myservo2.write(10);
delay(100);
  for(i=145; i > 90; i--)
  {
    myservo3.write(i);
    delay(15);
  }

}

void Release_Position_2(void)
{
  for(i=90; i < 150; i++)
  {
    myservo3.write(i);
    delay(15);
  }
  
  for(i = 50; i>20;i--)
  {
    myservo1.write(i);
    delay(30);
  }
  for(i = 10; i < 90; i++)
  {
    myservo2.write(i);
    delay(15);
  }

  for(i=150; i > 90; i--)
  {
    myservo3.write(i);
    delay(15);
  }
}
