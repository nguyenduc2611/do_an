void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial2.available() > 0) //Nếu có tín hiệu gửi đến Serial
  {
    int ledState = Serial2.read(); //Đọc giá trị gửi
    delay(100);
      Serial.println(ledState);
  }
}
