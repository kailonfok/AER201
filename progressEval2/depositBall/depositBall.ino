#include <Servo.h>
Servo leftServo;
Servo rightServo;
Servo armServo;

int leftPos = 180; // Starting positions for servo claws
int rightPos = 0;
int armPos = 0;
boolean start;

const int enableButPin = 21;
int enableState = 0;
int prevEnableState = 0;

void setup()
{
  Serial.begin(9600);
  
  // attach servo objects to respective pins
  armServo.attach(51);
  leftServo.attach(52);
  rightServo.attach(53);  
  
  // set servos to designated starting positions
  leftServo.write(leftPos);
  rightServo.write(rightPos);
  armServo.write(armPos);
  
  pinMode(enableButPin, INPUT);
}

void loop()
{
  enableState = digitalRead(enableButPin);
  delay(1);
  
  // on/off switch
  if(enableState != prevEnableState)
  {
    if(enableState)
      start = 1;
    else
      start = 0;
  }
  
  if(start)
  {
    if(moveArm(1)) //raise arm, when raised go into next step
    {
      if(openClaw()) // open claw, when opened go to next step
      {
        moveArm(-1); // lower arm back down
      }
    }
  }
}

boolean moveArm(int upDown) // 1 for up, -1 for down
{
  // loop until all the way up/down
  if(upDown)
  {
    Serial.println("Raising arm");
  }
  else
  {
    Serial.println("Lowering arm");
  }
  
  do
  {
    armServo.write(armPos);
    delay(15);
    armPos += upDown;
  }while(armPos != 0 || armPos != 180);
   
  return 1;
}

boolean openClaw()
{
  Serial.println("Opening claw");
  do // loop to open the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    delay(15);
    rightServo.write(rightPos);
    delay(15);
    //increment/decrement the right and left claw positions each iteration
    rightPos++;
    leftPos--;
      
  }while(leftPos != 0 && rightPos != 180);

  return 1;   
}
