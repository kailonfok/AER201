#include <Servo.h>
Servo leftServo;
Servo rightServo;
Servo armServo;

int leftPos = 0; // Starting positions for servo claws
int rightPos = 180;
int armPos = 170;
boolean start;
boolean inPosition = 0;

// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin
const int LEDPin1 = 52; // red LED for out of range
const int LEDPin2 = 53; // blue LED for in range
const float distanceConstant = 58.2;

int maxRange = 60;

//define constants for motors
const int motorPin[] = {
  30,31,32,33,34,35,36,37};
const int enablePin[] = {
  2,3,4,5}; // Starts from top, going clockwise

long duration, distance;

int dir = 3; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 3;
boolean highLow = 0;

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
  
  for(int i = 0; i < 8; i++)
  {
    pinMode(motorPin[i], OUTPUT);
    if (i < 4)
    {
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
//      pinMode(enablePin[i], OUTPUT);
    }  
  }
  
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
  
  Serial.print("Start: ");
  Serial.println(start);
  
//  if(start)
//  {
    if(!inPosition)
    {
      sensor(sensorNum);
      Serial.print("Sensor Number: ");
      Serial.println(sensorNum);
      
      inPosition = keepDriving();
      
      Serial.print("The distance is: ");  
      Serial.println(distance);
      
      Serial.print("The direction is: ");
      Serial.println(dir);
    }
    else
    {
      if(moveArm(-1))
      {
        Serial.println("First checkpoint");
    //    temp = openClaw();
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
//  }
}

// Function to turn off motors, before switching directions
void turnMotorsOff()
{
  // Write low to all enable pins on H-bridge (0 volts)
  analogWrite(enablePin[0], 0);
  analogWrite(enablePin[1], 0);
  analogWrite(enablePin[2], 0);
  analogWrite(enablePin[3], 0);

  for(int i = 0; i < 8; i++)
  {
    digitalWrite(motorPin[i], LOW);
  }  
  
  Serial.println("Motors off?");
  delay(1000);
}

boolean keepDriving()
{
  // At designated location
  if (distance <= maxRange)
  {
    // Switch LED outputs
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);   
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
    digitalWrite(LEDPin1, HIGH); 
    digitalWrite(LEDPin2, LOW);
    movement(dir);
    return 0;    
  }  
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  // structure for designating current flow through H-bridges
  if(motorDirection == 0)
  {
    highLow = 1;
    Serial.println("Moving forward");     
  }
  else if (motorDirection == 1)
  {
    Serial.println("Moving right");
    highLow = 1;
  }
  else if (motorDirection == 2)
  {
    Serial.println("Moving backwards");
    highLow = 0;
  }
  else if (motorDirection == 3)
  {
    Serial.println("Moving left");     
    highLow = 0;
  }
  else
  {
    Serial.println("Don't need to move");
  }

  // Write voltages to enable pins, and direct current through H-bridge (need to modify analogWrite values)
  if(motorDirection == 0 || motorDirection == 2)
  {
    analogWrite(enablePin[0], 175); // direct left wheel
    analogWrite(enablePin[1], 175); // direct right wheel
    digitalWrite(motorPin[0], !highLow);
    digitalWrite(motorPin[1], highLow);
    digitalWrite(motorPin[2], highLow);
    digitalWrite(motorPin[3], !highLow);
    // turn unwanted motors off (safety check)
    digitalWrite(motorPin[4], LOW);
    digitalWrite(motorPin[5], LOW);
    digitalWrite(motorPin[6], LOW);
    digitalWrite(motorPin[7], LOW);            
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    analogWrite(enablePin[2], 150); // Direct front wheel
    analogWrite(enablePin[3], 200); // Direct back wheel         
    // turn unwanted motors off (safety check)    
    digitalWrite(motorPin[0], LOW);
    digitalWrite(motorPin[1], LOW);
    digitalWrite(motorPin[2], LOW);
    digitalWrite(motorPin[3], LOW);    
    digitalWrite(motorPin[4], !highLow);
    digitalWrite(motorPin[5], highLow);
    digitalWrite(motorPin[6], !highLow);
    digitalWrite(motorPin[7], highLow);    
  }
  else
  {
    for(int i = 0; i < 8; i++)
    {
      digitalWrite(motorPin[i], 0);
    }
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
         
  }while(leftPos != 70 && rightPos != 110);  

  return 0; 
}
