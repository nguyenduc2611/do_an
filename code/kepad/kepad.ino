#include <Keypad.h>
#include <TimerOne.h>

const byte rows = 3; //số hàng
const byte columns = 4; //số cột
char key = 0;
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  };

  byte rowPins[rows] = {6, 7, 8};
byte columnPins[columns] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
void setup() {
  // put your setup code here, to run once:

  Timer1.initialize(100000); 
  Timer1.attachInterrupt(get_pass);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void get_pass(void)
{
  char temp = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      key = temp;
      Serial.println(key);
    }
  }
}
