#define data 13
void setup() {
  // put your setup code here, to run once:
  pinMode(data, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(data, LOW);
  delay(500);
  digitalWrite(data, HIGH);
  delay(500);
}
