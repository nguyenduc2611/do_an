int RS = 3;
int EN = 4;
int D4 = 5;
int D5 = 6;
int D6 = 7;
int D7 = 8;
int DHT = 2;

void read_temp_DHT11(int &T, int &H)
{ 
  char temp[5] = {0,0,0,0,0};
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

  T = temp[2];
  H = temp[0];
}

void Write_pin(int a)
{
  int b[8];
  for (int i=0; i<8;i++)
  {
    b[i] = ((a & (1 << i)));
  }
  digitalWrite(EN, HIGH);
  digitalWrite(D7, b[7]);
  digitalWrite(D6, b[6]);
  digitalWrite(D5, b[5]);
  digitalWrite(D4, b[4]);
  digitalWrite(EN, LOW);
  delay(15);
  digitalWrite(EN, HIGH);
  digitalWrite(D7, b[3]);
  digitalWrite(D6, b[2]);
  digitalWrite(D5, b[1]);
  digitalWrite(D4, b[0]);
  digitalWrite(EN, LOW);
  delay(5);
}

void setup_LCD()
{
  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  digitalWrite(RS, LOW);
  /* Interface 4 bit*/
  for (int i=0; i<2;i++)
  {
  Write_pin(3);
  }
  Write_pin(2);
  /*Function set DL=0; N=1; F=0*/
  Write_pin(40);
  /* Display On*/
  Write_pin(12);
  /*Display clear*/
  Write_pin(1);
  /*Emtry mode*/
  Write_pin(6);
}

void lcd_clear()
{
  digitalWrite(RS, LOW);
  Write_pin(1);
}

void lcd_positition(int row, int pos)
{
  digitalWrite(RS, LOW);
  if (1 == row)
  {
    Write_pin((128 + pos));
  }
  else
  {
    Write_pin((192 + pos));
  }
}
void lcd_print(char *p)
{
  int b[8];
  int i = 0;
  digitalWrite(RS, HIGH);

  while(0 != (*p))
  {
    b[i] = (*p);
    Write_pin(b[i]);
    i++;
    p++;
  }
  digitalWrite(RS, LOW);
  Write_pin(2);
}

void lcd_printnum(int p)
{
  int a,b = 0;
  a = p/10;
  b = p - (a*10);
  digitalWrite(RS, HIGH);
  Write_pin((a + 48));
  Write_pin((b + 48));
  digitalWrite(RS, LOW);
  Write_pin(2);
}


void setup() {
  // put your setup code here, to run once:
  setup_LCD();
}

void loop() {
  int nhiet,do_am = 0;
  read_temp_DHT11(nhiet, do_am);
  lcd_print("Nhiet do:");
  lcd_positition(1,10);
  lcd_printnum(nhiet);

  
  lcd_positition(2,0);
  lcd_print("Do am:");
  lcd_positition(2,10);
  lcd_printnum(do_am);
  lcd_positition(2,12);
  lcd_print("%");
  delay(1000);
}
