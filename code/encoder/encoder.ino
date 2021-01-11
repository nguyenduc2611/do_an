#include <TimerOne.h>
#include <Servo.h> 

Servo gServo;
unsigned long t = 0;
unsigned long t_delay = 0;
char i = 0;
char print_xung = 0;
int pin_inter = 2;
unsigned long t1 = 0;
unsigned long t2 = 0;
int h = 0;
volatile int k = 0;
int pos = 90;
int pre_servo = 512;
int v = 0;
int servo = 512;


void setup() {
  // put your setup code here, to run once:
    //Timer1.initialize(1000); 
    //Timer1.attachInterrupt(quay);
    pinMode(pin_inter, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_inter), get_key, FALLING);

    Serial.begin(9600);

    gServo.attach(9);
    gServo.write(pos);
}


void get_key(void)
{
    
    if(i == 29)
    {
      print_xung = 1;
    }
    else if(i == 0)
    {
      t1 = millis();
    }
    else
    {
      
    }
    i++;

}

void loop() {
  // put your main code here, to run repeatedly:

  servo = analogRead(A1);
  if(print_xung == 1)
  {
    t2 = millis();
    t = t2 - t1;
    Serial.println(t);
    if(t < 100)
    {
      k = 50;
    }
    else
    {
      k = 5;
    }
    t = 0;
    i = 0;
    print_xung = 0;
  }
  if((servo > pre_servo)&&((servo - pre_servo) > 6))
  {
    pre_servo = pre_servo + 6;
    gServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(k); 
    pos++;
  }
  else if((pre_servo > servo)&&((pre_servo - servo) > 6))
  {
    pre_servo = pre_servo - 6;
    gServo.write(pos);                // tell servo to go to position in variable 'pos'
    delay(k);
    pos--;
  }
  else
  {
  }

}
