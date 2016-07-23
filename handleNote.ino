#include <stdint.h>
#include <avr/io.h>
#include <MIDI.h>							 // http://arduinomidilib.sourceforge.net/a00001.html
#include <Wire.h>							 // I2C library (required by the EEPROM library below)
#include <EEPROM24LC256_512.h>	// https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino

void HandleNoteOn(byte channel, byte pitch, byte velocity){
  if(active){
    midiA.sendNoteOn(42, velocity, channel);
  }
  // midi::MidiType type = MIDI.getType();
	// midi::Channel channel = MIDI.getChannel();
	// byte data1 = MIDI.getData1();
	// byte data2 = MIDI.getData2();
	// MIDI.send(type, data1, data2, channel);
  pressed[1] = 1;
  justpressed[1] = 1;
}
