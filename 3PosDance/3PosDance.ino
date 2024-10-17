#include <ServoEasing.h> 
#include <ServoEasing.hpp> // Integrate the back-end code for Servos to run on Arduino. This one allows for 

ServoEasing leftServo;
ServoEasing baseServo;
ServoEasing rightServo;
ServoEasing clawServo; // Naming all the Servos for reference in functions later e.g. attach

int speed = 400;

int leftServoPin = 10; // left servo's pin connection 
int baseServoPin = 9; // base servo's pin connection 
int rightServoPin = 5; // right servo's pin connection 
int clawServoPin = 3; // claw servo's pin connection 
int buttonPin = 20; // button's pin connection

int btnState = 0; // Inititating the default setting of the button as being off, it hasn't been pushed down
int previousBtnState = 0; // Inititating the default setting of the button as being off, it hasn't been pushed down


// Left servo limits: 0 - 65
// Right servo limits: 0 - 75
// Base servo limits: 0 - 180
// Claw servo limits: 80- 145

// storing servo angles within a 4 column array to create each robot arm pose
int pose1[4] = {60, 0, 90, 80}; 
int pose2[4] = {25, 75, 180, 145}; 
int pose3[4] = {0, 25, 0, 120};
int currentPose = 0; // useful for the if statement to have a start point, start point should be position

void setup() {
  Serial.begin(9600); // good for debugging, function that enables serial.write aka narrative output of code at work  
  pinMode(buttonPin, INPUT); // The push button's data cable is connected to D20 . 4 button types INPUT is 1, OUTPUT + PULLUP & PULLDOWN

  leftServo.attach(leftServoPin, 40); // left servo's pin connection 
  rightServo.attach(rightServoPin, 30); // base servo's pin connection 
  baseServo.attach(baseServoPin, 45); // right servo's pin connection 
  clawServo.attach(clawServoPin, 80); // claw servo's pin connection 
}

void loop() {
  int btnState = digitalRead(buttonPin);
  if (btnState == 1 && previousBtnState==0) {
    Serial.println("called button");
    cyclePosition();
    Serial.println("pos: ");
    Serial.print(currentPose);
  }
  
  // leftServo control (non-blocking)
  int leftServoTargetPosition = getPosition(0);
  leftServo.setEasingType(EASE_CUBIC_IN_OUT);
  leftServo.easeTo(leftServoTargetPosition, speed);

  int rightServoTargetPosition = getPosition(1);
  rightServo.setEasingType(EASE_CUBIC_IN_OUT);
  rightServo.easeTo(rightServoTargetPosition, speed);


  int baseServoTargetPosition = getPosition(2);
  baseServo.setEasingType(EASE_CUBIC_IN_OUT);
  baseServo.easeTo(baseServoTargetPosition, speed);


  int clawServoTargetPosition = getPosition(3);
  clawServo.setEasingType(EASE_CUBIC_IN_OUT);
  clawServo.easeTo(clawServoTargetPosition, speed);

  
  delay(10);
}

void cyclePosition() {
  previousBtnState = 1;
  if(currentPose == 0) {
    currentPose = 1;
  } else if(currentPose == 1) {
    currentPose = 2;
  } else if(currentPose == 2) {
    currentPose = 0;
  } 
  previousBtnState = 0;
}

int getPosition(int servoNum) {
  if(currentPose == 0) {
    return pose1[servoNum];
  } else if(currentPose == 1) {
    return pose2[servoNum];
  } else if(currentPose == 2) {
    return pose3[servoNum];
  } 
}
