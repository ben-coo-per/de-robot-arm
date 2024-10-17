import oscP5.*;
import netP5.*;
import processing.serial.*;

// Define Serial port
Serial port;

// Define OSC variables
OscP5 oscP5;
NetAddress myRemoteLocation;

// Define four sliders
Slider slider1, slider2, slider3, slider4;

void setup() {
  size(400, 400);
  
  // Initialize sliders with range 0 to 180
  slider1 = new Slider(50, 100, 300, 20, 0, 180, 90);  // Initial value 90
  slider2 = new Slider(50, 150, 300, 20, 0, 180, 90);  // Initial value 90
  slider3 = new Slider(50, 200, 300, 20, 0, 180, 90);  // Initial value 90
  slider4 = new Slider(50, 250, 300, 20, 0, 180, 90);  // Initial value 90

  // Set up serial communication
  println(Serial.list());  // Print available serial ports in the console
  port = new Serial(this, Serial.list()[4], 9600);  // Select correct port from the printed list

  // Initialize OSC communication
  oscP5 = new OscP5(this, 12000);  // Listening to port 12000 for incoming OSC messages
  myRemoteLocation = new NetAddress("127.0.0.1", 12000);  // Set the remote address for sending OSC messages (if required)
}

void draw() {
  background(255);
  
  // Update and display sliders
  slider1.update();
  slider1.display();
  slider2.update();
  slider2.display();
  slider3.update();
  slider3.display();
  slider4.update();
  slider4.display();
  
  // Display values of sliders
  fill(0);
  text("Slider 1 value: " + slider1.getValue(), 50, 350);
  text("Slider 2 value: " + slider2.getValue(), 50, 370);
  text("Slider 3 value: " + slider3.getValue(), 50, 390);
  text("Slider 4 value: " + slider4.getValue(), 50, 410);
  
  // Send slider values over serial port at regular intervals
  if (frameCount % 30 == 0) {  // Send values every ~0.5 seconds
    sendSliderValues();
  }
}

// Function to send slider values over serial port
void sendSliderValues() {
  int slider1Val = (int)slider1.getValue();
  int slider2Val = (int)slider2.getValue();
  int slider3Val = (int)slider3.getValue();
  int slider4Val = (int)slider4.getValue();
  
  // Combine values in comma-separated format
  String message = slider1Val + "," + slider2Val + "," + slider3Val + "," + slider4Val + "\n";
  
  // Send message over serial port
  port.write(message);
}

// Handle OSC messages (if necessary)
void oscEvent(OscMessage theOscMessage) {
  if (theOscMessage.checkAddrPattern("/wek/outputs") == true) {
    if (theOscMessage.checkTypetag("f")) {
      // Parse and print OSC message values for debugging
      float firstValue = theOscMessage.get(0).floatValue();
      println("Received OSC message /wek/outputs with value: " + firstValue);
    }
  }
}

// Slider class
class Slider {
  float x, y, w, h, val, minVal, maxVal;
  boolean dragging = false;
  
  Slider(float x, float y, float w, float h, float minVal, float maxVal, float startVal) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.minVal = minVal;
    this.maxVal = maxVal;
    this.val = map(startVal, minVal, maxVal, x, x + w);
  }
  
  void display() {
    // Draw line for slider
    stroke(0);
    line(x, y, x + w, y);
    
    // Draw draggable knob
    noStroke();
    fill(150);
    ellipse(val, y, h, h);
  }
  
  void update() {
    if (dragging) {
      val = constrain(mouseX, x, x + w);
    }
  }
  
  float getValue() {
    return map(val, x, x + w, minVal, maxVal);
  }
  
  void press() {
    if (dist(mouseX, mouseY, val, y) < h / 2) {
      dragging = true;
    }
  }
  
  void release() {
    dragging = false;
  }
}

void mousePressed() {
  slider1.press();
  slider2.press();
  slider3.press();
  slider4.press();
}

void mouseReleased() {
  slider1.release();
  slider2.release();
  slider3.release();
  slider4.release();
}
