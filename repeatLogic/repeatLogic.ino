/*
  Get to hopper
  Get into hopper
  Get out of hopper
  Get to front wall
  Get to game board
  Play ball
  Repeat
*/

const byte rightSwitchPin = 32;
const byte leftSwitchPin = 33;

byte rightSwitchVal, leftSwitchVal;
byte prevRightVal = 0;

byte numLeft[] = {4,4,4,4};
byte currentHopper = 0;

byte xlocation[] = {40, -40, 0, 0};
byte ylocation[] = {0, 0, 80, 90);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(rightSwitchPin, INPUT);
  pinMode(leftSwitchPin, INPUT);

}

void loop() {

  if (currentHopper < 4)
  {
    do
    {
      rightSwitchVal = digitalRead(rightSwitchPin);
      delay(1);
      
      if(rightSwitchVal != prevRightVal)
      {
        if(rightSwitchVal)
        {
          numLeft[currentHopper]--;
          Serial.print("Num Left: ");
          Serial.println(numLeft[currentHopper]);
        }
      }
      
      prevRightVal = rightSwitchVal;
    }while(numLeft[currentHopper] != 0);
    
    currentHopper++;
    Serial.print("Current hopper: ");
    Serial.println(currentHopper);
  }
}
