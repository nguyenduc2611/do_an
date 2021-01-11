#define TRIG_PIN 13
#define ECHO_PIN 12
#define TIME_OUT 5000

int analogvalue = 0;
int val = 0;
float GetDistance()
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}
 
void setup() {  
  Serial.begin(9600);
 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}
 
void loop() {
  long distance = GetDistance();

 Serial.write('D');
  if (distance <= 0)
  {
  }
  else
  {   
    Serial.write(distance);
  }

  analogvalue = analogRead(A3);
  if(analogvalue < 100)
  {
    val = 1;
  }
  else
  {
    val = 2;
  }
  Serial.write('C');
  Serial.write(val);
  Serial.write('Z');
  delay(1000);
}
