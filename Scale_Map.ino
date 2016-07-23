/************************************************************************
 ***	 GROOVESIZER TEMPLATE v.1 - firmware development template
 ***	 for the GROOVESIZER 8-Bit Musical Multiboard
 ***	 http://groovesizer.com
 ************************************************************************
 * Copyright (C) 2013 MoShang (Jean Marais) moshang@groovesizer.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see http://www.gnu.org/licenses/
 *
 ************************************************************************/

#include <stdint.h>
#include <avr/io.h>
#include <MIDI.h>							 // http://arduinomidilib.sourceforge.net/a00001.html
#include <Wire.h>							 // I2C library (required by the EEPROM library below)
#include <EEPROM24LC256_512.h>	// https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino

boolean noteOn = true;
static boolean active = false; //  allows us to select between the two demo modes
EEPROM256_512 mem_1;						// define the eeprom chip
byte rwBuffer[64];							// our EEPROM read/write buffer

int pot1;
int pot2;
int pot3;
int pot4;
int pot5;
int pot6;

// for the rows of LEDs
// the byte that will be shifted out
byte LEDrow1;
byte LEDrow2;
byte LEDrow3;
byte LEDrow4;
byte LEDrow5;

// setup code for controlling the LEDs via 74HC595 serial to parallel shift registers
// based on ShiftOut http://arduino.cc/en/Tutorial/ShiftOut
const int LEDlatchPin = 2;
const int LEDclockPin = 3;
const int LEDdataPin = 4;

// setup code for reading the buttons via CD4021B parallel to serial shift registers
// based on ShiftIn http://www.arduino.cc/en/Tutorial/ShiftIn
const int BUTTONlatchPin = 7;
const int BUTTONclockPin = 8;
const int BUTTONdataPin = 9;

// needed by the button debouncer
// based on http://www.adafruit.com/blog/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
#define DEBOUNCE 5
byte pressed[40], justpressed[40], justreleased[40], buttons[40];

// define the byte variables that the button states are read into
byte BUTTONvar1;
byte BUTTONvar2;
byte BUTTONvar3;
byte BUTTONvar4;
byte BUTTONvar5;

// 4051 for the potentiometers
// array of pins used to select 1 of 8 inputs on multiplexer
const int select[] = {
	17,16,15}; // pins connected to the 4051 input select lines
const int analogPin = 0;			// the analog pin connected to multiplexer output

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiA);
