#include "PCD8544_HOANGSA.h"//a
PCD8544 lcd(3,4,5,6);//b
void setup()   {
  lcd.ON();//c
  lcd.SET(50,0,0,0,4); //d
}
void loop(){
 lcd.Asc_Char(60,30,'A', BLACK);               
 lcd.Display();
} 
