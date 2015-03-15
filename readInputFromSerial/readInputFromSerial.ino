char incomingByte = 0;   // for incoming serial data
int counter = 1;
void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        Serial.print("Please enter hopper locations, X then Y, hopper 1 then hopper 2");
}

int x[4] = {0,0,0,0};
void loop() {
        int value = 0;
        // send data only when you receive data:
        if (Serial.available() > 0) {
          
                while(true){
                  incomingByte = Serial.read();
                  if (incomingByte == '\n')
                    break;
                  value = value * 10;
                  value += (int) (incomingByte - '0');
                  }
                  Serial.println("");
                switch((counter%2)){
                  case 0: Serial.print("I received: ");
                          Serial.print(value);
                          Serial.println(" as a Y coordinate");
                          x[counter - 1] = value;
                          ++counter;
                          break;
                  case 1: Serial.print("I received: ");
                          Serial.print(value);
                          Serial.println(" as an X coordinate");
                          x[counter - 1] = value;
                          ++counter;
                          break;
                }
                
              }
              if (counter == 5){
                for(int i = 0; i < 4; ++i){
                  Serial.println("");
                  Serial.print(x[i]);
                  Serial.print(" ");
                  
                }
              delay(50000);
              }
      
      delay(50);  
      }

