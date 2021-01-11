#include <Stepper.h>
#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
const int stepsPerMotorRevolution = 4096; //No of steps per internal revolution of motor,
//4-step mode as used in Arduino Stepper library

const int stepsPerOutputRevolution = 512*64; //no of steps per revolution of the output shaft
int var = 512;

int i = 0;
int sta = 0;
int pre_var = 512;
// initialize the stepper library on pins 8 through 11, Motor rev steps, "Firing" sequence 1-3-2-4,
Stepper myStepper(stepsPerMotorRevolution, 8,9,10,11);
LiquidCrystal_I2C lcd(0x20,16,2);
void setup() {
// Stepper library sets pins as output
    //Timer1.initialize(10000); 
    //Timer1.attachInterrupt(quay);
myStepper.setSpeed(60); //Set the speed
Serial.begin(9600); // initialize the serial port:
lcd.begin();
}

// MAIN LOOP +++++++++++++++++++
void loop() {
// step one revolution in one direction:

myStepper.step(1024);
delay(500);

/*var = analogRead(A2);
if(var > pre_var)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(var);
  myStepper.step(((var - pre_var)/6)*(stepsPerOutputRevolution/100));
  pre_var = var;
}

if(pre_var > var)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(var);
  myStepper.step(-(((pre_var - var)/6)*(stepsPerOutputRevolution/100)));
  pre_var = var;
}*/


}
