int ds = 2;
int st = 3;
int sh = 4;
int a[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16};
void setup() {
  // put your setup code here, to run once:
  pinMode(ds, OUTPUT);
  pinMode(st, OUTPUT);
  pinMode(sh, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0;i<10;i++) {
      digitalWrite(st, LOW);
      shiftOut(ds, sh, MSBFIRST, a[i]);  
      //Note 3: MSBFIRST có thể đổi thành LSBFIRST và ngược lại.
      digitalWrite(st, HIGH);
      delay(500);
  }
}
