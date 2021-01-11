int k = 0;
char incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(5000);
  Serial.print('A');
  delay(5000);
  Serial.end();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(k == 0)
  {
    Serial.begin(9600);
    Serial.print('A');
    k = 1;
    Serial.end();
  }
  while('B' != incomingByte)
  {
    Serial.begin(9600);
    if(Serial.available() > 0)
    {
    incomingByte = Serial.read();
    }
    Serial.print(incomingByte);
    k = 0;
    Serial.end();
  }
  delay(1000);
}
