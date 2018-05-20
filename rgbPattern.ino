#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs for the vibration motors
const byte pin[] = {5,6,9};
const byte motorsNumber = sizeof(pin)/sizeof(pin[0]); // number of vibration motors

// WARNING: Pin 8 is broken.
// WARNING: To fix it, pin 9 and pin 8 are (physically) connected by a metal wire!

// Max strength of the motor vibration. REDUCE if vibrations are too intense/annoying. Range 1-255. 
const byte MAX_VIBRATION_INTENSITY = 150; 

// Buffer size. The longer the buffer size, the higher the accuracy in the color detection, the slower the change from one color to another. Default: 10 samples.
const byte MemorySize= 9;

// Vibration interval. Set the speed of the motors. Default: 250ms.
const unsigned long vibrationInterval = 250;

// Vibration pattern length: duration of the "Step sequencer". For example, if longer, red will vibrate at a slower rhythm.
const byte vibrationPatternLength = 3*motorsNumber; // Each color has a vibration pattern which lasts 6 steps.

byte oldCodes[MemorySize];
unsigned long previousVibrTime = 0;

byte previousColorCode = 0;byte previousVibratingPin = 0;
byte vibrationIndex = 0;bool newVibrationPattern = true;
bool toggleGrn = true; bool toggleYlw = true; bool toggleMag = true;
bool toggleLed = false;

const byte colorNum = 6 + 1;
String colorNames[colorNum] = { "No color","RED","GREEN","BLUE","MAGENTA","YELLOW","ORANGE"};
  
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

//const unsigned long ledInterval = 20;
//unsigned long previousLedTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Colors to vibrations");

  if (tcs.begin()) {    Serial.println("Found sensor");  } 
  else {    Serial.println("No TCS34725 found ... check your connections");    while (1); }// halt! 
  
  // use these pins to drive VIBRATION MOTORS
  for (byte x=0; x<motorsNumber; x++){ // analog pins A0 to A5
      pinMode(pin[x], OUTPUT);
  } 

  // Initialize the array "oldCodes" to all zeros
  memset(oldCodes,0,MemorySize);

  vibrate(0, 0); // do not vibrate
}

void loop() {
  float colors[] = {0,0,0};
  byte temporaryColorCode = 0;
  byte colorCode = 0;
  unsigned long currentTime = millis();
  
  //Serial.print(" New   code:\t ");   Serial.print(temporaryColorCode);  //Serial.print(" Color code:\t ");   Serial.println(colorCode);

  if (currentTime - previousVibrTime >= vibrationInterval) { // Every "vibrationInterval" milliseconds, read a new color
    readRGB(colors);   

    Serial.print((int)colors[0]);Serial.print("\t");   Serial.print((int)colors[1]);Serial.print("\t");    Serial.println((int)colors[2]);
    
    // The newly read COLOR.
    temporaryColorCode = readColor(colors);     
  
    // The color which drives the vibration motors
    colorCode = decideColor(temporaryColorCode, oldCodes, MemorySize);      

    //  store the last time you activated the motors in  a variable
    previousVibrTime = currentTime;

    // activate vibrations
    driveMotors(colorCode, &vibrationIndex);   
  }
}

//void checkLed()
//{
//  unsigned long  currTime = millis();
//  if ( ( toggleLed && (currTime - previousLedTime) >= ledInterval) || ( !toggleLed && (currTime - previousLedTime) >= ledInterval/5) ) {
//    tcs.setInterrupt(toggleLed);      // turn on LED
//    toggleLed = !toggleLed;
//    previousLedTime = currTime;
//  }
//}


