#include <TimerOne.h>

int CLK = 5;
int EN = 6;
int DR = 7;

int previous = 0;
int current = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DR, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(EN, HIGH);
  digitalWrite(DR, HIGH);

  Serial.begin(9600);
  
}

void loop() {
  int value = analogRead(A0);
  int feq;
  feq = map(value, 0, 1023, 1000, 5000);
  
  if(feq < 2000 )
  {
    current = 2000;
  }
  else if(feq < 3000)
  {
    current = 3000;
  }
  else if(feq < 4000)
  {
    current = 4000;
  }
  else
  {
    current = 5000;
  }

  if (previous != current)
  {
    previous = current;
    Timer1.initialize(current);
    Timer1.attachInterrupt(Blink);
  }
}

void Blink()
{
  digitalWrite(CLK,!digitalRead(CLK));
}
