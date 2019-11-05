/*
Interface an Android smartphone with an Arduino via Bluetooth 
to control an StepperMotor from your phone
modified on 22 Oct 2019
by Alireza Sheikhjafari @ Rabbit Channel

install :Adfruit motor shield library

Serial port protocol description
https://www.aparat.com/v/sQzoT
*/
#include <AFMotor.h>
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String commandString = "";

int EndOfCommend;
int EndOfdata;


int currentAngle=0;
int lastAngle=0;
int delta=0;


// two stepper motors one on each port
AF_Stepper myStepper(200, 1);

void setup() {
 Serial.begin(9600); 

  myStepper.setSpeed(100);

}
void loop() {
   if(Serial.available()) serialEvent();
  if(stringComplete)
  {
      stringComplete = false;
      getCommand();
      
      if(commandString.equals("angle"))
      {
        String text = getData();
        int angle= text.toInt();
        Rotateto_angle(angle);
      }
    
    inputString = "";
  }
  delay(100);
  
}

void Rotateto_angle(int angle)
{
  currentAngle = map(angle,0,359,0,2048); // Makes 200 pulses for making one full cycle rotation
 
  if (currentAngle != lastAngle){
    if(currentAngle > lastAngle){  
      delta = currentAngle - lastAngle;
      Serial.println("BACKWARD");   
       myStepper.step(delta, BACKWARD, SINGLE);
       myStepper.release();   
      }
   
    if(currentAngle < lastAngle){  
      delta = lastAngle - currentAngle;
      Serial.println("FORWARD"); 
      myStepper.step(delta, FORWARD, SINGLE);
       myStepper.release();   
      }
    }
    lastAngle = currentAngle;

}




void getCommand()
{
  if(inputString.length()>0)
  {  
     EndOfCommend=inputString.indexOf(',');
     commandString = inputString.substring(1,EndOfCommend);
     
     Serial.print("Command:");
     Serial.println(commandString);
  }
}




String getData()
{
  String value = inputString.substring(EndOfCommend+1,inputString.length()-2);
  Serial.print("Data:");
  Serial.println(value);
  Serial.println("---------------------------");
  return value;
}



void serialEvent() {
  while (Serial.available() && stringComplete == false) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    Serial.print(inChar);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

