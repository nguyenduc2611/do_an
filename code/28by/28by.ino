#include <AccelStepper.h>
#define HALFSTEP 8
char status_run1 = 0;
char status_run2 = 0;
unsigned long var = 0;
unsigned int pre_var = 0;
unsigned int pre_var1 = 0;
char status_var = 0;
char status_break = 0;
// Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(100);
  pinMode(13, OUTPUT); 
  Serial.begin(9600);
  var = analogRead(A2);
  pre_var = var;
  pre_var1 = var;

}//--(end setup )---

void loop() {
  var = analogRead(A2);
  while(((var > (pre_var + 10))||(pre_var > (var + 10)))&&(!status_break))
  {
    if(var < pre_var1 + 4)
    {
      status_break = 1;
    }
    else if(pre_var1 < var + 4)
    {
      status_break = 1;
    }
    else
    {
      
    }
    pre_var1 = var;
    delay(500);
    var = analogRead(A2);
  }
  status_break = 0;
  Serial.println(var);
  if(var > (pre_var + 10))
  {
    if(!status_run1)
    {
      stepper1.move((var - pre_var)*4);
      status_run1 = 1;
    }
    while(stepper1.distanceToGo() != 0) {
      stepper1.run();
    }
    stepper1.stop();
    pre_var = var;
    status_run1 = 0;
  }
  else if(pre_var > (var + 10))
  {
    if(!status_run2)
    {
      stepper1.move(-((pre_var - var)*4));
      status_run2 = 2;
    }
    while(stepper1.distanceToGo() != 0) {
      stepper1.run();
    }
    stepper1.stop();
    pre_var = var;
    status_run2 = 0;
  }
  else
  {
    
  }
  //Change direction when the stepper reaches the target position
  /*if (stepper1.distanceToGo() == 0) {
    stepper1.moveTo(-2000);
  }
  stepper1.run();*/
}
