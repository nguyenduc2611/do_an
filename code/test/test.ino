char Esp_data = 0;
void setup() {
Serial3.begin(9600);
Serial.begin(9600);
}
 
void loop() {
  
if(Serial3.available() > 0)
  {
    Esp_data = Serial3.read();
    Serial.println(Esp_data);
  }
}
