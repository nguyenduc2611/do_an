/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
/*Servo 1 100 - 5*/
/*Servo 2 90 -180*/
/*Servo 3 100 - 5*/
/*Servo 4 85 -180*/
#include <Servo.h>
#include <TimerOne.h>

#define CB_1 6
#define CB_2 2
#define NO_TRAIN     0
#define TRAIN_PASS_1 1
#define TRAIN_PASS_2 2
#define BUZZER 11
#define LED 7

Servo myservo_1;  // create servo object to control a servo
Servo myservo_2;
Servo myservo_3;
Servo myservo_4;
uint32_t t_1 = 0;
uint32_t t_2 = 0;
char train = NO_TRAIN;
char start_train_1 = 0;
char start_train_2 = 0;
char status_cb1 = 0;
char status_cb2 = 0;
char train_dir = 0;
char close_barie = 0;

void setup() {

  Serial.begin(9600);
  pinMode(CB_1, INPUT);
  pinMode(CB_2, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

    myservo_1.attach(9); 
    myservo_2.attach(10);
    myservo_3.attach(3);
    myservo_4.attach(5);

    myservo_1.write(5);
    myservo_2.write(180);
    myservo_3.write(5);
    myservo_4.write(180);
    delay(1000);
    myservo_1.detach();
    myservo_2.detach();
    myservo_3.detach();
    myservo_4.detach();

      Timer1.initialize(10000); 
  Timer1.attachInterrupt(count_time);
}

void loop() {
  status_cb1 = digitalRead(CB_1);
  status_cb2 = digitalRead(CB_2);
  char status_train1 = is_train1();
  char status_train2 = is_train2();
  if((status_train1 == 1)&&(status_train2 == 0)&&(train_dir == 0))
  {
    Serial.println("Huong 1");
    train_dir = 1;
  }
  else if((status_train1 == 0)&&(status_train2 == 1)&&(train_dir == 0))
  {
    Serial.println("Huong 2");
    train_dir = 2;
  }
  else
  {
    
  }
  if(train_dir == 1)
  {
    if(close_barie == 0)
    {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED, HIGH);
      myservo_1.attach(9); 
      myservo_2.attach(10);
      myservo_3.attach(3);
      myservo_4.attach(5);
    
      myservo_2.write(90);
      myservo_4.write(85);
      delay(500);
      myservo_1.write(100);
      myservo_3.write(100);
      delay(500);
      myservo_1.detach();
      myservo_2.detach();
      myservo_3.detach();
      myservo_4.detach();
      Timer1.initialize(10000); 
      Timer1.attachInterrupt(count_time);
      close_barie = 1;
    }
    if(status_train2 == 1)
    {
      close_barie = 0;
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      myservo_1.attach(9); 
      myservo_2.attach(10);
      myservo_3.attach(3);
      myservo_4.attach(5);
    
      myservo_1.write(5);
      myservo_2.write(180);
      myservo_3.write(5);
      myservo_4.write(180);
      delay(500);
      myservo_1.detach();
      myservo_2.detach();
      myservo_3.detach();
      myservo_4.detach();
      Timer1.initialize(10000); 
      Timer1.attachInterrupt(count_time);
      train_dir = 0;
    }
  }
  else if(train_dir == 2)
  {
    if(close_barie == 0)
    {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED, HIGH);
      myservo_1.attach(9); 
      myservo_2.attach(10);
      myservo_3.attach(3);
      myservo_4.attach(5);
    
      myservo_2.write(90);
      myservo_4.write(85);
      delay(500);
      myservo_1.write(100);
      myservo_3.write(100);
      delay(500);
      myservo_1.detach();
      myservo_2.detach();
      myservo_3.detach();
      myservo_4.detach();
      Timer1.initialize(10000); 
      Timer1.attachInterrupt(count_time);
      close_barie = 1;
    }
    if(status_train1 == 1)
    {
      close_barie = 0;
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      myservo_1.attach(9); 
      myservo_2.attach(10);
      myservo_3.attach(3);
      myservo_4.attach(5);
    
      myservo_1.write(5);
      myservo_2.write(180);
      myservo_3.write(5);
      myservo_4.write(180);
      delay(500);
      myservo_1.detach();
      myservo_2.detach();
      myservo_3.detach();
      myservo_4.detach();
      Timer1.initialize(10000); 
      Timer1.attachInterrupt(count_time);
      train_dir = 0;
    }
  }
  else
  {
    
  }
}

char is_train1(void)
{
  char status_train = 0;
  
  if(start_train_1 == 2)
  {
    Serial.println(t_1);
    start_train_1 = 0;
    if((t_1 > 40)&&(t_1 < 80))
    {
      status_train = 1;
    }
    t_1 = 0;
  }

  return status_train;
}

char is_train2(void)
{
  char status_train = 0;
  
  if(start_train_2 == 2)
  {
    Serial.println(t_2);
    start_train_2 = 0;
    if((t_2 > 40)&&(t_2 < 80))
    {
      status_train = 1;
    }
    t_2 = 0;
  }

  return status_train;
}

void count_time(void)
{

  if(status_cb1 == LOW)
  {
    start_train_1 = 1;
    t_1++;
  }
  else
  {
    if(start_train_1 == 1)
    {
      start_train_1 = 2;
    }
  }

  if(status_cb2 == LOW)
  {
    start_train_2 = 1;
    t_2++;
  }
  else
  {
    if(start_train_2 == 1)
    {
      start_train_2 = 2;
    }
  }
}
