//const int motorPin[] = {2,3,4,5};
const int motorPin1 = 2;
const int motorPin2 = 3;
const int motorPin3 = 4;
const int motorPin4 = 4;
const int onOffButtonPin = 6;
const int directionButPin = 7;
const int enablePin = 9;

const int numMotors = 4;

int motorSpeed = 0;
int motorEnable = 0;
int previousMotorEnable = 0;

int motorOnOff = 0;
int motorDirection = 0;

int dir = 0;
int prevDir = 0;

void setup()
{
  Serial.begin(9600);
  /*for (int i = 0; i < numMotors; i++)
  {
    pinMode(motorPin[i], OUTPUT);
  }*/
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(onOffButtonPin, INPUT);
  pinMode(directionButPin, INPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(enablePin, LOW);
}

void loop()
{
  motorEnable = digitalRead(onOffButtonPin);
  delay(1);
  
  dir = digitalRead(directionButPin);
  delay(1);
  
  if(motorEnable != previousMotorEnable) //Switch between on and off
  {
    if(motorEnable)
      motorOnOff = !motorOnOff;
  }
  
  if(dir != prevDir)
  {
    if(dir)
      motorDirection = !motorDirection;
  }
  
  if (motorOnOff)
  {
    movement(motorDirection);
    analogWrite(enablePin, HIGH);    
    Serial.println("Going into movement");
  }
  else
  {
    analogWrite(enablePin, 0);
  }
  
  Serial.print("Motor enable:  ");
  Serial.println(motorOnOff);
  
  previousMotorEnable = motorEnable;
  prevDir = dir;
}

void movement(int motorDirection)
{
   if(motorDirection)
   {
     //Move forward
     Serial.println("Moving forward");     
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(motorPin3, HIGH);
     digitalWrite(motorPin4, LOW);
     
     //turn
     /*
     digitalWrite(motorPin[0], HIGH);
     digitalWrite(motorPin[1], LOW));
     digitalWrite(motorPin[2]), LOW);
     digitalWrite(motorPin[3], HIGH);     
     */
   }
   else
   {
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(motorPin3, LOW);
     digitalWrite(motorPin4, HIGH);
   }
}
