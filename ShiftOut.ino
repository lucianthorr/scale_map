/**********************************************************************************************
 ***
 *** ShiftOut to address 42 LEDs via 4 74HC595s serial to parallel shifting registers 
 *** Based on http://arduino.cc/en/Tutorial/ShiftOut
 ***
 ***********************************************************************************************/
//***TOP*OF*THE*SKETCH**************************************************************************
// remember to add the following to the start of your sketch before (before setup)
/*
int LEDlatchPin = 2;
 int LEDclockPin = 3;
 int LEDdataPin = 4;
 */
//***SETUP**************************************************************************************
// and add the following pin modes to setup()
/*
pinMode(LEDlatchPin, OUTPUT);
 pinMode(LEDclockPin, OUTPUT); 
 pinMode(LEDdataPin, OUTPUT);
 */

void shiftOut(int myLEDdataPin, int myLEDclockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  // on the rising edge of the clock,
  // clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myLEDclockPin, OUTPUT);
  pinMode(myLEDdataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myLEDdataPin, 0);
  digitalWrite(myLEDclockPin, 0);

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myLEDclockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {	
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myLEDdataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myLEDclockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myLEDdataPin, 0);
  }

  //stop shifting
  digitalWrite(myLEDclockPin, 0);
}

// this function shifts out the the 4 bytes corresponding to the led rows
void updateLeds(void)
{
  static boolean lastSentTop = false; // we want to alternate sending the top 2 and bottom 3 rows to prevent an edge case where 4 rows of LEDs lit at the same time sourcing too much current
  //ground LEDlatchPin and hold low for as long as you are transmitting
  digitalWrite(LEDlatchPin, 0);
  if (!lastSentTop) // send the top to rows
  {
  shiftOut(LEDdataPin, LEDclockPin, B00000000);
  shiftOut(LEDdataPin, LEDclockPin, B00000000); 
  shiftOut(LEDdataPin, LEDclockPin, B00000000);
  shiftOut(LEDdataPin, LEDclockPin, LEDrow2); 
  shiftOut(LEDdataPin, LEDclockPin, LEDrow1);
  lastSentTop = true;
  }
  else // ie. lastSentTop is true, then send the bottom 3 rows
  {
  shiftOut(LEDdataPin, LEDclockPin, LEDrow5);
  shiftOut(LEDdataPin, LEDclockPin, LEDrow4); 
  shiftOut(LEDdataPin, LEDclockPin, LEDrow3);
  shiftOut(LEDdataPin, LEDclockPin, B00000000); 
  shiftOut(LEDdataPin, LEDclockPin, B00000000);
  lastSentTop = false;
  }
  //return the latch pin high to signal chip that it 
  //no longer needs to listen for information
  digitalWrite(LEDlatchPin, 1);
}


