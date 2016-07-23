// *************************************
//           HELPER FUNCTIONS
// *************************************

// POTS
// reading from 4051
// this function returns the analog value for the given channel
int getValue( int channel)
{
  // set the selector pins HIGH and LOW to match the binary value of channel
  for(int bit = 0; bit < 3; bit++)
  {
    int pin = select[bit]; // the pin wired to the multiplexer select bit
    int isBitSet =  bitRead(channel, bit); // true if given bit set in channel
    digitalWrite(pin, isBitSet);
  }
  //return map((constrain(analogRead(analogPin), 40, 1000)), 40, 1000, 0, 127); // we're only using readings between 40 and 1000, as the extreme ends are unstable
  // or return the raw value and only do the (computationally costly) map function if and when needed
  int reading = analogRead(analogPin);
  return reading; 
}

void clearJust() // clear the "just" arrays
{
  for (int i = 0; i < 40; i++)
  {
    justpressed[i] = 0;
    justreleased[i] = 0;
  }
}

// a handy DIFFERENCE FUNCTION
int difference(int i, int j)
{
  int k = i - j;
  if (k < 0)
    k = j - i;
  return k;
}

void scrollText(char myString[])
{
  byte textLength = strlen(myString);
  // read character definitions from EEPROM
  for (byte j = 0; j < 1; j++) // how many times from 0 - 9 & A - 7
  {
    for (byte m = 0; m < textLength; m++) // how many characters
    {
      int page = 508 ;
      byte c = myString[m];
      if (c < 0)       // SPACE
      {
        c = 0;
        page = 508;
      }
      else if (c < 58)  // 0 - 10
      {
        c = (c - 47)*5;
        page = 508;
      }
      else if (c < 65)  // ?
      {
        c = 55;
      }
      else if (c < 77) // A - L
      {
        c = (c - 65)*5;
        page = 509;
      }
      else if (c < 89) // M - X
      {
        c = (c - 77)*5;
        page = 510;
      }
      else
      {
        c = (c - 89)*5;
        page = 511;
      }
      mem_1.readPage(page, rwBuffer); // read the page that the character is on
      byte charRow1 =  rwBuffer[c];
      byte charRow2 =  rwBuffer[c+1];
      byte charRow3 =  rwBuffer[c+2];
      byte charRow4 =  rwBuffer[c+3];
      byte charRow5 =  rwBuffer[c+4];

      for (byte i = 0; i < 7; i++) // how many pixels wide?
        // it's only 7 because we don't need three blank pixels before the character, 2 are enough
      {
        //shift each row one pixel (bit) to the right to make space for the new collumn 
        LEDrow1 = LEDrow1 >> 1;
        LEDrow2 = LEDrow2 >> 1;
        LEDrow3 = LEDrow3 >> 1;
        LEDrow4 = LEDrow4 >> 1;
        LEDrow5 = LEDrow5 >> 1;

        if (bitRead(charRow1, 6-i)) // shorthand for "if ((bitRead(charRow1, i) == 1)"
          bitSet(LEDrow1, 7);
        if (bitRead(charRow2, 6-i)) // 
          bitSet(LEDrow2, 7);
        if (bitRead(charRow3, 6-i))
          bitSet(LEDrow3, 7);
        if (bitRead(charRow4, 6-i))
          bitSet(LEDrow4, 7);
        if (bitRead(charRow5, 6-i))
          bitSet(LEDrow5, 7);

        for (int k = 0; k < 10; k++) // how many times through the update cycle before we move to the next vertical row of pixels
        {
          updateLeds();
          delay(5);
        }
      }
    }
  }
}

