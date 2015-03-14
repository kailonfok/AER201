#include <AFMotor.h>
#include <Servo.h>

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

Servo leftServo;
Servo rightServo;
Servo armServo;

int leftPos = 0; // Starting positions for servo claws
int rightPos = 180;
int armPos = 165;
boolean start;
boolean inPosition = 0;

// Define pins for wing pins
const int leftSwitchPin = 33;
const int rightSwitchPin = 32;

// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin

const float distanceConstant = 58.2;

int maxRange = 55;

long duration, distance;

int dir = 3; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 3;

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
  
  pinMode(rightSwitchPin, INPUT);
  
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);    
}

void loop()
{
  enableState = digitalRead(rightSwitchPin);
  delay(1);
  
  Serial.print("Enable State: ");
  Serial.println(enableState);
  
  // on/off switch
  if(enableState != prevEnableState)
  {
    if(enableState)
      start = 1;
  }
  Serial.println(start);
  
  if(start)
  {
    if(!inPosition)
    {
      inPosition = keepDriving();
    }
    else
    {
      if(moveArm(-1))
      {
        Serial.println("First checkpoint");
        if(armPos == 0)
        {
          if(openClaw())
          {
            moveArm(1);
          }
        }
      }
      else
      {
        Serial.println("Done!");
      }
    }
  }
}

// Function to turn off motors, before switching directions
void turnMotorsOff()
{
  // Write low to all enable pins on H-bridge (0 volts)
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);  
  
  Serial.println("Motors off?");
  delay(1000);
}

boolean keepDriving()
{
  // At designated location
  if (distance <= maxRange)
  { 
    turnMotorsOff(); // turn motors off before making decision, so it doesn't keep driving
    
    if(dir == 1) // If going right, switch to going forward (for first hopper and first instance)
    {
      Serial.println("Got to first checkpoint");
      delay(1000);
      prevDir = dir;
      dir = -1;
      sensorNum = 0;
    }
    else if (dir == -1) // stop running
    {
      turnMotorsOff();
    }
    return 1;
  }
  else if (distance >= maxRange){ // Not close enough, keep driving in direction
    movement(dir);
    return 0;    
  }  
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  // direct motors to turn in appropriate direction and speed
  if (motorDirection == 0 || motorDirection == 2)
  {
    leftMotor.setSpeed(220);
    rightMotor.setSpeed(255);
    if(motorDirection == 0)
    {
      Serial.println("Moving forward");
      delay(1000);      
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
    }
    else
    {
      Serial.println("Moving back");
      delay(1000);      
      leftMotor.run(BACKWARD);
      rightMotor.run(BACKWARD);
    }
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    frontMotor.setSpeed(255);
    backMotor.setSpeed(255);
    if(motorDirection == 1)
    {
      Serial.println("Moving right");
      delay(1000);
      frontMotor.run(BACKWARD);
      backMotor.run(BACKWARD);
    }
    else
    {
      Serial.println("Moving left");
      delay(1000);      
      frontMotor.run(FORWARD);
      backMotor.run(FORWARD);
    }
  }
  else
  {
    Serial.println("Don't need to move");
    turnMotorsOff();
  }
}

// function for determining distance to wall/objects
void sensor(int sensorNum)
{
  digitalWrite(trigPin[sensorNum], LOW);
  delayMicroseconds(2); 

  digitalWrite(trigPin[sensorNum], HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigPin[sensorNum], LOW);
  duration = pulseIn(echoPin[sensorNum], HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/distanceConstant;  

  delay(50);
}


//Functions for depositing ball start here
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
    if(armPos == 0 || armPos == 170)
      exit = 0;
  }while(exit == 1);
  
  Serial.println("Reaching end of loop");
  
  if(upDown == -1)
  {
    value = 1;
  }
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
    rightPos++;
    leftPos--;
      
  }while(leftPos != 0 && rightPos != 180);
  return 1;
}

int closeClaw()
{
  Serial.println("Closing claw");
  do // loop to close the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    delay(15);        
    rightServo.write(rightPos);
    
    rightPos--;
    leftPos++;
         
  }while(leftPos != 110 && rightPos != 70);

  return 0; 
}
