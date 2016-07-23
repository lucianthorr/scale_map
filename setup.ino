#include <stdint.h>
#include <avr/io.h>
#include <MIDI.h>							 // http://arduinomidilib.sourceforge.net/a00001.html
#include <Wire.h>							 // I2C library (required by the EEPROM library below)
#include <EEPROM24LC256_512.h>	// https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino

// *************************************
//					 THE SETUP
// *************************************

void setup() {

	//define pin modes
	pinMode(LEDlatchPin, OUTPUT);
	pinMode(LEDclockPin, OUTPUT);
	pinMode(LEDdataPin, OUTPUT);

	pinMode(BUTTONlatchPin, OUTPUT);
	pinMode(BUTTONclockPin, OUTPUT);
	pinMode(BUTTONdataPin, INPUT);

	for(int bit = 0; bit < 3; bit++)
		pinMode(select[bit], OUTPUT);	// set the three EEPROM select pins to output

	// Initiate MIDI communications, listen to all channels
  midiA.turnThruOn();
  midiA.begin(MIDI_CHANNEL_OMNI);
  midiA.setHandleNoteOn(HandleNoteOn);

	//begin I2C Bus
	Wire.begin();

	//begin EEPROM with I2C Address
	mem_1.begin(0,0);//addr 0 (DEC) type 0 (defined as 24LC256)

	// check if character definitions exists in EEPROM by seeing if the first 10 bytes of the expected character array add up to 93
	mem_1.readPage(508, rwBuffer); // 0 is the first page of the EEPROM (511 is last) - a page is 64 bytes long
	byte x = 0;
	for (byte i = 0; i < 10; i++)
	{
		x = x + rwBuffer[i];
	}
	if (x != 93) // if not, write the following to EEPROM
	{
		//give us a bad sign!
		LEDrow1 =	B10101010;
		LEDrow2 =	B01010101;
		LEDrow3 =	B10101010;
		LEDrow4 =	B01010101;
		LEDrow5 =	B10101010;
		for (int i = 0; i < 100; i++)
		{
			updateLeds();
			delay(5);
		}
	}

	else
	{
		//give us a good sign!
		LEDrow1 =	B11111111;
		LEDrow2 =	B11111111;
		LEDrow3 =	B11111111;
		LEDrow4 =	B11111111;
		LEDrow5 =	B11111111;
		for (int i = 0; i < 100; i++)
		{
			updateLeds();
			delay(5);
		}
	}

	// message on boot
	scrollText(" X	"); // scrollText() is defined in HelperFunctions - see the tab above
}
