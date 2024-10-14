#include <Servo.h>



// # BUTTON VARIABLES
int buttonPin = 20;
int buttonState = LOW;   // Current state of the button
int lastButtonState = LOW; // Previous state of the button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Debounce time (in ms)

Servo leftServo;
Servo baseServo;
Servo rightServo;
Servo clawServo;

// # TIME VARIABLES
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;

long interval = 25; // 25 ms between movements

// POSITION VARIABLES
// {left, right, base, claw}
int position0[4] = {120, 0, 90, 80};
int position1[4] = {25, 65, 180, 145};
int position2[4] = {0, 20, 0, 120};

int currentPos = 0;
int leftServoPos = position0[0];  // position of leftServo
int rightServoPos = position0[1];   // 20 - 83
int baseServoPos = position0[2];   // position of baseServo
int clawServoPos = position0[3];  // 80 - 150


void setup() {
  leftServo.attach(10); // Left Side
  baseServo.attach(9); // Base
  rightServo.attach(5); // Right Side
  clawServo.attach(3); // Claw

  pinMode(buttonPin, INPUT); // Can change this to INPUT_PULLUP if needed

  Serial.begin(9600);

  leftServo.write(leftServoPos);
  baseServo.write(baseServoPos);
  rightServo.write(rightServoPos);
  clawServo.write(clawServoPos);
}

void cyclePosition() {
  // A method that sets the current position to the next one in the cycle
  if (currentPos == 0) {
    currentPos = 1;
  } else if (currentPos == 1) {
    currentPos = 2;
  } else if (currentPos == 2) {
    currentPos = 0;
  }
}

int getPosition(int servoNum) {
  // A helper function that returns what position a servo is supposed to be at
  if (currentPos == 0) {
    return position0[servoNum];
  } else if (currentPos == 1) {
    return position1[servoNum];
  } else if (currentPos == 2) {
    return position2[servoNum];
  }
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current duration that this loop has been running for

  // Check if button is being pressed
  buttonState = digitalRead(buttonPin);

  // A small debounce so that the button press isn't registered a few times. (basically if you're holding down the button for ~50ms, `cyclePosition` shouldn't be called twice)
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();  // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Only trigger the function when the button state is stable
    if (buttonState == HIGH && lastButtonState == LOW) {
      cyclePosition();
    }
  }

  // Update the last button state for the next loop
  lastButtonState = buttonState;

  // leftServo control
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;

    int position = getPosition(0); // Fixed semicolon
    if (leftServoPos < position) {
      leftServoPos++; // Increment position
      leftServo.write(leftServoPos);
    } else if (leftServoPos > position) {
      leftServoPos--; // Decrement position
      leftServo.write(leftServoPos);
    }
  }

  // rightServo control
  if (currentMillis - previousMillis3 >= interval) {
    previousMillis3 = currentMillis;

    int position = getPosition(1);
    if (rightServoPos < position) {
      rightServoPos++;
      rightServo.write(rightServoPos);
    } else if (rightServoPos > position) { // Fixed closing parenthesis
      rightServoPos--;
      rightServo.write(rightServoPos);
    }
  }

  // baseServo control
  if (currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;

    int position = getPosition(2); // Re-declare position
    if (baseServoPos < position) {
      baseServoPos++;
      baseServo.write(baseServoPos);
    } else if (baseServoPos > position) {
      baseServoPos--;
      baseServo.write(baseServoPos);
    }
  }

  // clawServo control
  if (currentMillis - previousMillis4 >= interval) {
    previousMillis4 = currentMillis;

    int position = getPosition(3);
    if (clawServoPos < position) {
      clawServoPos++;
      clawServo.write(clawServoPos);
    } else if (clawServoPos > position) {
      clawServoPos--;
      clawServo.write(clawServoPos);
    }
  }
}
