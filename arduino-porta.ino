/*
 Door Lock - Automatized
 An Arduino control to open and close doors using step-motor and bluetooth.
 More in: https://github.com/raphaelbs/Porta
 PlayStore app: https://play.google.com/store/apps/details?id=com.createlier.porta
 Created 15 Feb. 2016
 by Raphael Brandão
 */

#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);
bool isOpen = false;
int stepsRequired = 2048;

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(12);
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("Welcome to Porta's Arduino Serial Communication");
  Serial.println("The firmware is listening for three actions commands:");
  Serial.println(" - toggle : open and close door");
  Serial.println(" - open : open door");
  Serial.println(" - close : close door");
  Serial.println("You can also set the step/action using:");
  Serial.println(" - sr[number] : where number is the step required. I.e.: sr2048 ");
}

void loop() {
  if(Serial.available()){
    char serialBuffer[8];
    // IMPORTART: setting the Arduino to read until a break-line is detected
	char linebreak = '\n'; \\ Here is the line-break I talked about
	int finalSize = Serial.readBytesUntil(linebreak , serialBuffer, 8); 
    serialBuffer[finalSize] = '\0';
    handler(serialBuffer);
  }
}

void handler(String in){
  if(in == "toggle"){
    // IMPORTART: always send message using println() or '\n' to break-line
    Serial.println("toggle");
    if(isOpen){
      myStepper.step(stepsRequired);
    }else{
      myStepper.step(-stepsRequired);
    }
    isOpen = !isOpen;
    delay(500);
  }
  if(in == "open"){
    Serial.println("open");
    myStepper.step(-stepsRequired);
    isOpen = true;
    delay(500);
  }
  if(in == "close"){
    Serial.println("close");
    myStepper.step(stepsRequired);
    isOpen = false;
    delay(500);
  }
  if(in.charAt(0)=='s' && in.charAt(1)=='r'){
    Serial.print("Changing steps/action from ");
    Serial.print(stepsRequired);
    Serial.print(" to ");
    in = in.substring(2);
    int newSteps = in.toInt();
    Serial.println(newSteps);
    stepsRequired = newSteps;
  }
}
