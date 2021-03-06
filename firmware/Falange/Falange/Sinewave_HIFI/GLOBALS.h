/*
 * GLOBALS.h
 *
 * Created: 04-09-2019 01:08:32
 *  Author: tiago
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

// MACROS
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define BIT(x)(0x01 << (x))
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// HARDWARE
#define LED 3
#define SWITCH1 4	// delay pre/post filter
#define SWITCH2 7	// filter mode (LOWPASS, HIGHPASS)
#define KNOB1 A2		// Delay
#define KNOB2 A3		// LFO Rate (modulation?)
#define KNOB3 A4		// Amount (distortion, feedback, modulation)
#define KNOB4 A5		// Filter freq (resonance ?)
#define KNOB5 A6		// LFO Wave
#define KNOB6 A1		// Distotion/FX types

// TABLES
#include <tables/sin256_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/waveshape_sigmoid_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/whitenoise8192_int8.h>
#include <tables/uphasor256_uint8.h> // for the s&h

// SYNTH 
#define CONTROL_RATE 64 // min=64 beacause of Q24n8
// distortion modes
#define MOFF 0		// off 
#define MAND 1		// AND &
#define MOR 2		// OR | 
#define MXOR 3		// XOR ^
#define MNOT 4		// NOT ~
#define MSHIFT 5	// bitshift left
#define MRND1 6		// audio-rate random bitshift
#define MRND2 7		// lfo distortion bitshift

#define RNDSHIFT 16 // value used in rnd shifts
// filter
#define LOWESTFREQ 128 // 128 works well 
#define HIGHESTHPFREQ 3008 // higher values (ex: 4096) cause weird things :) 
#define HIGHESTLPFREQ 3072
#define LOWESTRES 255
#define HIGHESTRES 1
// delay
#define MAXDELAY 256 // ArduinoProMini max delay = 256 samples
#define FBKMIN -64
#define FBKMAX 63

#endif /* GLOBALS_H_ */