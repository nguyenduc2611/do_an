int in = 7;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(digitalRead(in));
delay(1000);
}
