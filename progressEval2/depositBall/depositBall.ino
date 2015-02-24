#include <Servo.h>
Servo leftServo;
Servo rightServo;
Servo armServo;

int leftPos = 7; // Starting positions for servo claws
int rightPos = 208;
int armPos = 165;
boolean start;
int temp;

const int enableButPin = 50;
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
  delay(50);
  
  closeClaw();
  
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
      start = !start;
  }
  
  if(start)
  {
//    temp = moveArm(-1);
    if(moveArm(-1))
    {
      Serial.println("First checkpoint");
  //    temp = openClaw();
      if(openClaw())
      {
        moveArm(1);
      }
    }
    else
    {
      Serial.println("Done!");
    }
  }
}

boolean moveArm(int upDown) // -1 for up, 1 for down
{
  boolean value;
  boolean exit = 1;
  // loop until all the way up/down
  if(upDown == -1)
  {
    Serial.println("Raising arm");
  }
  else if (upDown == 1)
  {
    Serial.println("Lowering arm");
  }
  
  do
  {
    armServo.write(armPos);
    delay(15);
    armPos += upDown;
    Serial.println("Stuck??");
    if(armPos == 40 || armPos == 170)
      exit = 0;
  }while(exit == 1);
  
  Serial.println("Reaching end of loop");
  
  if(upDown == -1)
    value = 1;
  else
  {
    value = 0;
    start = 0;
  }
    
  return value;   
}

boolean openClaw()
{
  Serial.println("Opening claw");
  do // loop to open the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    rightServo.write(rightPos);
    delay(15);
    //increment/decrement the right and left claw positions each iteration
    rightPos += 2;
    leftPos--;
      
  }while(leftPos != 7 && rightPos != 208);
  
  if (moveArm(1))
  {
    return 1;   
  }
  else
  {
    return 0;
  }
}

int closeClaw()
{
  Serial.println("Closing claw");
  
  do // loop to close the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    delay(15);
    rightServo.write(rightPos);
    rightPos -= 2;
    leftPos++;
         
  }while(leftPos != 47 && rightPos != 98);  

  return 0; 
}
