/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */


#define echoPin 22 // Echo Pin
#define trigPin 23 // Trigger Pin
#define LEDPin1 52 // red LED for out of range
#define LEDPin2 53 // green LED for in range
const float distanceConstant = 58.2;

int maximumRange = 15; // Maximum range needed
int minimumRange = 5; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin1, OUTPUT); // Use LED indicator (if required)
  pinMode(LEDPin2, OUTPUT); // Use LED indicator (if required)
}

void loop() {
  /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/distanceConstant;

  Serial.print("The distance is: ");
  if (distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON 
     to indicate "out of range" */
    Serial.println(distance);
    digitalWrite(LEDPin1, HIGH);
    digitalWrite(LEDPin2, LOW); 
  }
  else if (distance >= maximumRange){
    /* Send the distance to the computer using Serial protocol, and
     turn LED OFF to indicate successful reading. */
    Serial.println(distance);
    digitalWrite(LEDPin1, LOW); 
    digitalWrite(LEDPin2, HIGH);    
  }
  else
  {
    digitalWrite(LEDPin1, HIGH);
    digitalWrite(LEDPin2, HIGH);
    Serial.println(distance);
  }

  //Delay 50ms before next reading.
  delay(50);
}

