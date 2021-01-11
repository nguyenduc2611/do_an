int DS = 8;
int SH = 9;
int ST = 10;
int i = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(DS, OUTPUT);
pinMode(SH, OUTPUT);
pinMode(ST, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ST, LOW);
  for(i = 0;i<8;i++)
  {
    digitalWrite(SH, LOW);
    if(i == 4)
    {
      digitalWrite(DS, HIGH);
    }
    else
    {
      digitalWrite(DS, LOW);
    }
    digitalWrite(SH, HIGH);
  }
  digitalWrite(ST, HIGH);
}
