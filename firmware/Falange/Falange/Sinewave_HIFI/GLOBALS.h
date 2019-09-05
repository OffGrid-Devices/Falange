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
#define KNOB1 2		// Delay
#define KNOB2 3		// LFO Rate (modulation?)
#define KNOB3 4		// Amount (distortion, feedback, modulation)
#define KNOB4 5		// Filter freq (resonance ?)
#define KNOB5 6		// LFO Wave
#define KNOB6 7		// Distotion/FX types

// SYNTH 
#define CONTROL_RATE 64
// distortion modes
#define MOFF 0		// off 
#define MAND 1		// AND &
#define MOR 2		// OR | 
#define MXOR 3		// XOR ^
#define MNOT 4		// NOT ~
#define MSHIFT 5	// bitshift left
#define MRND1 6		// updateAudio random bitshift
#define MRND2 7		// updateControl random bitshift
// filter
#define LOWESTFREQ 128 // 128 works well 
#define HIGHESTHPFREQ 3008 // higher values (ex: 4096) cause weird things :) 
#define HIGHESTLPFREQ 3072
#define LOWESTRES 255
#define HIGHESTRES 1
#endif /* GLOBALS_H_ */