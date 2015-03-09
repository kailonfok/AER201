#include <Servo.h>
Servo leftServo;
Servo rightServo;

int leftPos = 0; // Starting positions for servo claws
int rightPos = 180;
boolean onOff = 1;
boolean start;

const int echoPin[] = {26,22};
const int trigPin[] = {27,23};
const float distanceConstant = 58.2;
const int maxRange = 10;

const int motorPin[] = {30,31,32,33};
const int enablePin[] = {2, 3};

const int enableButPin = 50;
boolean enableState = 0;
boolean prevEnableState = 0;

long duration, distance;

int dir = 1;
int sensorNum = 0;

void setup()
{
  Serial.begin(9600);
  leftServo.attach(52);
  rightServo.attach(53);
  
  for(int i = 0; i < 4; i++)
  {
    pinMode(motorPin[i], OUTPUT);
    if (i < 2)
    {
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
//      pinMode(enablePin[i], OUTPUT);
    }
    pinMode(enableButPin, INPUT);
  }
  // set servos into appropriate position
  leftServo.write(leftPos);
  rightServo.write(rightPos);
}

void loop()
{
  enableState = digitalRead(enableButPin);
  delay(1);
  
  if(enableState != prevEnableState) // if button is pressed and released
  {
    if(enableState) // if switched to on
    {
      start = !start;
    }
  }
  
  Serial.print("Start: ");
  Serial.println(start);

  if(start)
  {  
    if (dir == 1) // first retrieve the ball
    {
      dir = closeClaw(); // change direction to forward      
    }
    else
    {
      sensor(sensorNum);
      Serial.print("Sensor Number: ");
      Serial.println(sensorNum);
      
      keepDriving();
      
      Serial.print("The distance is: ");  
      Serial.println(distance);
      
      Serial.print("The direction is: ");
      Serial.println(dir);
    }  
  }
  else
  {
    analogWrite(enablePin[0], LOW);
    analogWrite(enablePin[1], LOW);    
  }
  
  prevEnableState = enableState;
}

void keepDriving()
{
  digitalWrite(enablePin[0], LOW);
  digitalWrite(enablePin[1], LOW);
  if (distance >= maxRange)
  {   
    Serial.println("Done!");
  }
  else if (distance <= maxRange){
    movement(dir);    
  }
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  boolean highLow = 0;

  analogWrite(enablePin[0], 175);
  analogWrite(enablePin[1], 175);  

  if(motorDirection == 0)
  {
    highLow = 1;
    Serial.println("Moving forward");     
  }
  else if (motorDirection == 2)
  {
    Serial.println("Moving backwards");
    highLow = 0;
  }
    digitalWrite(motorPin[0], !highLow);
    digitalWrite(motorPin[1], highLow);
    digitalWrite(motorPin[2], highLow);
    digitalWrite(motorPin[3], !highLow);
}

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
         
  }while(leftPos != 40 && rightPos != 140);

  return 0; 
}
