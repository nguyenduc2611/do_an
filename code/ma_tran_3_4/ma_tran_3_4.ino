#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'} 
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad
char customKey = 0;
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("bat dau");
}

void loop() {
  // put your main code here, to run repeatedly:

customKey = customKeypad.getKey();
if(customKey != 0){
Serial.println(customKey);
customKey = 0;}
delay(500);
}
