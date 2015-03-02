// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin
const int LEDPin1 = 52; // red LED for out of range
const int LEDPin2 = 53; // blue LED for in range
const float distanceConstant = 58.2;

int maxRange = 12;

//define constants for motors
const int motorPin[] = {
  30,31,32,33,34,35,36,37};
const int enablePin[] = {
  2,3,4,5}; // Starts from top, going clockwise

long duration;
long distance = 15;

int dir = 1; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 1;
boolean highLow = 0;

//enable pin to start
boolean start;
const int enableButPin = 50;
int enableState = 0;
int prevEnableState = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);  

  for (int i = 0; i < 9; i++)
  {
    pinMode(motorPin[i], OUTPUT);
    if (i < 4)
    {
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
    }
  }
  pinMode(enableButPin, INPUT);
}

void loop()
{
  enableState = digitalRead(enableButPin);
  delay(1);
  
  if(enableState != prevEnableState)
  {
    if(enableState)
      start = 1;
  }
  
  Serial.print("Start: ");
  Serial.println(start);
  
//  if(start)
//  {  
    //Every iteration, check distance to wall
    sensor(sensorNum);
    Serial.print("Sensor Number: ");
    Serial.println(sensorNum);
  
    // diagnostic outputs
    Serial.print("The distance is: ");    
    Serial.println(distance);  
  
    // function call to determine if at wall or not
    keepDriving();    
    Serial.print("Previous direction: ");
    Serial.println(prevDir);
  
    Serial.print("Motor Direction:  ");
    Serial.println(dir);
//  }
//  else
//  {
//    digitalWrite(LEDPin1, HIGH);
//    digitalWrite(LEDPin2, HIGH);       
//    Serial.println("Don't do anything");
//    delay(1000);
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

void keepDriving()
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
      dir = 0;
      sensorNum = 0;
    }
    else if (dir == 0) // If going forward, switch to going left
    {
        Serial.println("Going to board");
        delay(1000);
        prevDir = dir;
        dir = 3;
        sensorNum = 3;
        maxRange = 65;
    }
    else if (dir == 2)
    {
      Serial.println("At game board!");
      delay(1000);
      prevDir = dir;
      dir = -1;
      sensorNum = 1; // There needs to be some logic here to then place ball and return to last hopper
      maxRange = 12;
    }
  }
  else if (distance >= maxRange){ // Not close enough, keep driving in direction
    digitalWrite(LEDPin1, HIGH); 
    digitalWrite(LEDPin2, LOW);
    movement(dir);    
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
    analogWrite(enablePin[0], 200); // direct left wheel
    analogWrite(enablePin[1], 200); // direct right wheel
    digitalWrite(motorPin[0], highLow);
    digitalWrite(motorPin[1], !highLow);
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
    analogWrite(enablePin[2], 150); // direct front wheel
    analogWrite(enablePin[3], 200); // direct back wheel         
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
