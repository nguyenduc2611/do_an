
 int DHT = 2;
 char temp[5] = {0,0,0,0,0};
//Khởi tạo với các chân
void setup() {
  Serial.begin(9600);
}

void read_temp_DHT11()
{ 
  /*Send request*/
  pinMode(DHT, OUTPUT);
  digitalWrite(DHT, LOW);
  delay(18);
  digitalWrite(DHT, HIGH);
  delayMicroseconds(40);
  /* Wait for response*/
  pinMode(DHT, INPUT);
  while((digitalRead(DHT)) == HIGH);
  delayMicroseconds(160);
  /* Read data*/
  for(int j=0; j<5; j++)
  {
      for(int i = 0; i<8;i++)
      {
        while((digitalRead(DHT)) == LOW);
        delayMicroseconds(54);
        if((digitalRead(DHT))==HIGH)
        {
          temp[j] = ((temp[j]) << 1)|(0x01);
        }
        else
        {
          temp[j] = ((temp[j]) << 1);
        }
        while((digitalRead(DHT)) == HIGH);
      }
  }
  /*check sum*/
  while(temp[4] != ((temp[0]+temp[1]+temp[2]+temp[3]) & 0xFF));
}
    

 
void loop() {
  int a,b,c = 0;
  read_temp_DHT11();
  a = temp[0];
  b = temp[2];
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  delay(2000);
}
