const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int directionSwitchPin = 6;
const int onOffSwitchStateSwitchPin = 7;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(directionSwitchPin, INPUT);
  pinMode(onOffSwitchStateSwitchPin, INPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(enablePin, LOW);
}

void loop()
{
  Serial.print("On/Off:  ");
  Serial.print(motorEnabled);
  Serial.print("\tDirection:  ");
  Serial.println(motorDirection);
  onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
  delay(1);
  directionSwitchState = digitalRead(directionSwitchPin);
  delay(1);
  motorSpeed = analogRead(potPin/4);
  
  if(onOffSwitchState != previousOnOffSwitchState)
  {
    if(onOffSwitchState)
      motorEnabled = !motorEnabled;
  }
      
  if(directionSwitchState != previousDirectionSwitchState)
  {
    if(directionSwitchState)
      motorDirection = !motorDirection;
  }

  if(motorDirection)
  {
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  }
  else
  {
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }
  
  if(motorEnabled)
    analogWrite(enablePin, motorSpeed);
  else
    analogWrite(enablePin, 0);
    
    previousDirectionSwitchState = directionSwitchState;
    previousOnOffSwitchState = onOffSwitchState;
}
