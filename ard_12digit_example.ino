#include "LedController.hpp"
// #include "Arduino.h"

// https://github.com/noah1510/LedController/blob/master/examples/english/7-Segment-counting/7-Segment-counting.ino
// Set Display Pins
#define DIN 35
#define CLK 36
#define CS 34

LedController<2, 1> lc;
int delaytime = 2000;
char testMessage[] = "this is a test";
String countUp;
/*
Section 1 is the first 8 digits. Section 0 is the last 4
 section 1    section 0
 88888888     8888
*/

byte numberList[]{ // 
  0b1111110,  // 0
  0b0110000,  // 1
  0b1101101,  // 2
  0b1111001,  // 3
  0b0110011,  // 4
  0b1011011,  // 5
  0b1011111,  // 6
  0b1110000,  // 7
  0b1111111,  // 8
  0b1111011   // 9
};

void setup() {
  Serial.begin(9600);

  //Here a new LedController object is created without hardware SPI.
  lc = LedController<2, 1>(DIN, CLK, CS);

  // Set the brightness to a medium values
  lc.setIntensity(8);
  // and clear the display 
  lc.clearMatrix();
}

void outputString(String message) {
  int position = 0; // Keeps track of digit location
  int section = 1; // First section is the first 9 digits. Section 0 has 4 digits
  bool dotList[] = { false, false, false, false, false, 
  false, false, false, false, false, false, false }; // Decimal location. 1 indicates a decimal in that location


  lc.clearMatrix();
  Serial.println(message);

  // Updates dotList where decimals are located with true (marking the location)
  // Decimal points are removed from the string
  int indexOfDot = message.indexOf('.', 1);
  while (indexOfDot != -1) { 
    dotList[indexOfDot - 1] = true;
    message.remove(indexOfDot, 1);
    indexOfDot = message.indexOf('.', indexOfDot);
  };

  position = 0;

  for (int i = 0; i < message.length(); i++) {
    if (position > 7) { // if position is greater than 7. Move to the next section (section 0)
      section = 0; // Section 1 is the first 8 digits. Section 0 is the last 4
      position = position - 8;
    }
    
    // sets the section, position, char to display, and if decimal is lit
    lc.setChar(section, position, message[i], dotList[i]);
    position++;
  }
}

void loop() {
  // DEMO
  lc.clearMatrix();

  outputString("test.test.");
  delay(delaytime);

  lc.setIntensity(1);
  outputString("123456789012");
  delay(delaytime);

  outputString("192.168.1.1");
  delay(delaytime);

  lc.setIntensity(8);
  outputString("12 30 p");
  delay(delaytime);

  lc.setIntensity(15);
  outputString("1-01-2025");
  delay(delaytime);

  lc.clearMatrix();

  for (int x=0; x<10000 ; x++) {
  countUp = String(x);
  outputString(countUp);
  delay(10);
  }
}
