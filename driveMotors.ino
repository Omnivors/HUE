
void driveMotors(byte colorCode, byte *vibrCount)
{ 
  if (colorCode != previousColorCode){ // When there is a new color, or nothing is detected, do not vibrate for one cycle.
    Serial.println(""); Serial.print("New color: "); Serial.println(colorNames[colorCode]);
    (*vibrCount) = 0;
     newVibrationPattern = true;
     vibrate(0, 0); // do not vibrate
  }
  else if (colorCode == previousColorCode && !(colorCode==0)){
    vibrate(colorCode, (*vibrCount));
    newVibrationPattern = false; // PENSO CHE QUESTE DUE RIGHE ANDREBBERO SCAMBIATE?
    (*vibrCount)++;
  }
  else if (colorCode==0){
    vibrate(0, 0); // do not vibrate    
  }

  if ( (*vibrCount) >= vibrationPatternLength){
    (*vibrCount) = 0;
  }
  previousColorCode = colorCode;
}

void vibrate(byte code, byte count){
  byte power[motorsNumber];
  byte vibratingPin = motorsNumber;

  memset(power,0,motorsNumber); // DEFAULT: Set motors to zero (DO NOT VIBRATE)

  if (code == 4){ // MAGENTA 
    if (count >= vibrationPatternLength-1){       toggleMag = !toggleMag; 
    }
    if (toggleMag){  code = 1; // assign red code
    }
    else if (!toggleMag){ code = 3; // assign blue code   
    }
  }
  
  if ((code == 1) && ((count % vibrationPatternLength ) == 0)) { // RED
      memset(power, MAX_VIBRATION_INTENSITY, motorsNumber);
  }
  else if ( (code == 2) || (code == 3) || (code==5) ){ //SINGLE PIN VIBRATING
    if (code == 2){ // GREEN
        if (newVibrationPattern){
          vibratingPin = rand() % motorsNumber; // Return a number ranging in [0, motorsNumber-1]
        }
        else if ( !newVibrationPattern && toggleGrn){
          vibratingPin = nextPin(previousVibratingPin);
          toggleGrn = !toggleGrn;
        }
        else{
          toggleGrn = !toggleGrn;
         }
      }
    
    else if (code == 3){ // BLU
      if (newVibrationPattern){
        vibratingPin = rand() % motorsNumber; // Return a number ranging in [0, motorsNumber-1]
      }
      else {
        vibratingPin = nextPin(previousVibratingPin);
      }
    }
    else if (code == 5){ // YELLOW
      if (count == 0 ){
          vibratingPin = rand() % motorsNumber; // Return a number ranging in [0, motorsNumber-1]
      }
    }

    if (vibratingPin != motorsNumber){
      power[vibratingPin] = MAX_VIBRATION_INTENSITY; // Activate the selected motor
      previousVibratingPin = vibratingPin;
    }
  }

  
    for (byte x=0; x<motorsNumber; x++){ // analog pins A0 to A5
        analogWrite(pin[x], power[x]);
        if (!(code==0 && count==0)){ Serial.print((bool)power[x]);}
    }     Serial.print(".");
  
}

byte nextPin (byte vibrPin)
{ 
  byte newVibrPin = vibrPin + 1;
  if (newVibrPin < motorsNumber){
    return newVibrPin;}
  else{
    return 0;}
}

