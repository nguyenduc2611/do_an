#define RELAY 2
void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY, HIGH);
  delay(200);
  digitalWrite(RELAY, LOW);
  delay(200);
}
