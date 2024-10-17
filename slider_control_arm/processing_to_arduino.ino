#include <ServoEasing.h> 
#include <ServoEasing.hpp> 

// Servo objects
ServoEasing leftServo;
ServoEasing baseServo;
ServoEasing rightServo;
ServoEasing clawServo; 

// Servo pins
const int leftServoPin = 10; 
const int baseServoPin = 9; 
const int rightServoPin = 5; 
const int clawServoPin = 3; 

// Servo limits
const int leftServoMin = 0, leftServoMax = 65;
const int rightServoMin = 0, rightServoMax = 75;
const int baseServoMin = 0, baseServoMax = 180;
const int clawServoMin = 80, clawServoMax = 145;

const int speed = 400;  // Speed for servo movements

void setup() {
  Serial.begin(9600);

  // Attach servos to their respective pins
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  baseServo.attach(baseServoPin);
  clawServo.attach(clawServoPin);
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read the incoming string
    String inputString = Serial.readStringUntil('\n');

    // Parse and set the servo positions
    setServoPositions(inputString);
  }
  
  delay(10);  // Small delay to avoid overwhelming the serial port
}

// Function to set the servo positions based on the received input
void setServoPositions(String input) {
  // Split the string by commas and store the values in an array
  int servoValues[4];  
  int index = 0;
  
  char inputArray[input.length() + 1];
  input.toCharArray(inputArray, input.length() + 1);
  
  char* token = strtok(inputArray, ",");
  while (token != NULL && index < 4) {
    servoValues[index] = atoi(token);
    token = strtok(NULL, ",");
    index++;
  }

  // Map the input values to the servo limits and move the servos
  if (index == 4) {  // Ensure we have exactly 4 values
    moveServo(leftServo, servoValues[0], leftServoMin, leftServoMax);
    moveServo(rightServo, servoValues[1], rightServoMin, rightServoMax);
    moveServo(baseServo, servoValues[2], baseServoMin, baseServoMax);
    moveServo(clawServo, servoValues[3], clawServoMin, clawServoMax);

    // Debugging: Print the mapped positions
    Serial.print("Mapped positions: ");
    Serial.print(servoValues[0]);
    Serial.print(", ");
    Serial.print(servoValues[1]);
    Serial.print(", ");
    Serial.print(servoValues[2]);
    Serial.print(", ");
    Serial.println(servoValues[3]);
  }
}

// Function to move a servo to the mapped position with easing
void moveServo(ServoEasing &servo, int inputValue, int minVal, int maxVal) {
  int targetPosition = map(inputValue, 0, 180, minVal, maxVal);
  servo.setEasingType(EASE_CUBIC_IN_OUT);
  servo.easeTo(targetPosition, speed);
}
