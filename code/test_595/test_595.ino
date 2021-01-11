#define _clock D2  //SHCP
#define _latch D1  //STCP
#define _data  D0  //Ds
int i = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(_latch, OUTPUT);
  pinMode(_clock, OUTPUT);
  pinMode(_data, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(_latch, LOW);
for(i=0;i<8;i++)
{
  digitalWrite(_clock, LOW);
  if(i == 2)
  {
    digitalWrite(_data, LOW);
  }
  else
  {
    digitalWrite(_data, HIGH);
  }
  digitalWrite(_clock, HIGH);
}
digitalWrite(_latch, HIGH);
}
