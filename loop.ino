#include <stdint.h>
#include <avr/io.h>
#include <MIDI.h>							 // http://arduinomidilib.sourceforge.net/a00001.html
#include <Wire.h>							 // I2C library (required by the EEPROM library below)
#include <EEPROM24LC256_512.h>	// https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino
// *************************************
//					 THE LOOP
// *************************************

// welcome to the loop
void loop() {
  midiA.read();
	// create an array of 5 bytes to keep track of which LEDs are on
	// we mark them as static so they only get created once and retain their values - like a global variable, except that their scope is limited to this function (ie. the loop)
	static byte buttonLED[5] = {
		0,0,0,0,0										};


	// reset the LEDs to all off at the start of the loop
	// I found it easier to start with a clean slate at the top of each loop like this
	// makes it easier to keep track of why a particular LED is lit
	// remember that the bytes below are a mirror image of what you will see on the groovesizer
	// to on turn on the top left LED, you would need "LEDrow1 = B00000001;"
	LEDrow1 =	B00000000;
	LEDrow2 =	B00000000;
	LEDrow3 =	B00000000;
	LEDrow4 =	B00000000;
	LEDrow5 =	B00000000;

	// we check buttons at the top of the loop, the "just" flags get reset here every time
	// it's best to reset the "just" flags manually whenever we check for "just" and find it true - use "clearJust()"
	// it prevents another just function from being true further down in the same loop
	check_switches();

	// get the pot values (defined in HelperFunctions - see the tabs at the top)
	pot1 = getValue(0);
	pot2 = getValue(1);
	pot3 = getValue(2);
	pot4 = getValue(3);
	pot5 = getValue(4);
	pot6 = getValue(5);
	if(justpressed[32]){
			active = not active;
			justpressed[32] = false;
	}

  if(active){


			// we already updated the switches at the top of the loop, so now we can just check their states with a for-loop
			// the 40 buttons are numbered 0 to 39
			for (byte i = 0; i < 40; i++)
			{
				if (justpressed[i]) {
						buttonLED[i/8] = bitSet(buttonLED[i/8], (i % 8));

						// i is a byte so i/8 will return the row of LEDs (0 - 4) that i is in
						// we're using bitSet, since we only want to switch on, not toggle the bit
						// sendNoteOn (byte NoteNumber, byte Velocity, byte Channel)
						// send out a midi note on channel 1
					//	MIDI.sendNoteOn (i + 45, 127, 1); // we're adding 45 to offset the button number (MIDI notes are 0 to 127)
				} else if (justreleased[i]) {
						buttonLED[i/8] = bitClear(buttonLED[i/8], (i % 8));

						// we're using bitClear, since we want to switch off, not toggle the bit
						// sendNoteOff (byte NoteNumber, byte Velocity, byte Channel)
						// turn off the midinote
					//	MIDI.sendNoteOff (i + 45, 0, 1); // sending a MIDI note off is one way to turn off a note
						// the other way is to send a note on with velocity 0
				}
		}
		buttonLED[4] = buttonLED[4] | B00000001;
		clearJust(); // it's helpful to clear the just-arrays after you've checked for it, so a just can't be true later in the loop again
		// end of EXAMPLE CODE FOR TRIGGERING MIDI NOTES
	}else{
		buttonLED[4] = buttonLED[4] ^ B00000001;
	}

	// update the actual bytes we're going to shift out on this loop
	LEDrow1 =	buttonLED[0];
	LEDrow2 =	buttonLED[1];
	LEDrow3 =	buttonLED[2];
	LEDrow4 =	buttonLED[3];
	LEDrow5 =	buttonLED[4];

	// this function shifts out the the 5 bytes corresponding to the led rows
	// declared in ShiftOut (see above)
	updateLeds();
}
//END OF THE LOOP
