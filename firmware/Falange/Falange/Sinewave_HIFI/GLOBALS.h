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
#define SWITCH1 4 // delay pre/post filter
#define SWITCH2 7 // filter mode (LOWPASS, HIGHPASS)
#define KNOB1 2 // Delay
#define KNOB2 3 // LFO Rate (modulation?)
#define KNOB3 4 // Amount (distortion, feedback, modulation)
#define KNOB4 5 // Filter freq (resonance ?)
#define KNOB5 6 // LFO Wave
#define KNOB6 7 // Distotion/FX types



#endif /* GLOBALS_H_ */