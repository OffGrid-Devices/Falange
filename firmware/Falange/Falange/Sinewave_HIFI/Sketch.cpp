/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/************************************************************************/
/* INCLUDES                                                                     */
/************************************************************************/
#include "GLOBALS.h" 

#include <MozziGuts.h>
#include <IntMap.h>
#include <mozzi_rand.h>
#include <Oscil.h> 
#include <StateVariable.h>
#include <LowPassFilter.h>
#include <AudioDelayFeedback.h>

//Beginning of Auto generated function prototypes by Atmel Studio
void updateControl();
int updateAudio();

void lightled(int8_t sig);
int distortions(int sig, uint8_t dist);
//End of Auto generated function prototypes by Atmel Studio

/************************************************************************/
/* OBJECTS                                                              */
/************************************************************************/
Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> testosc(WHITENOISE8192_DATA);
Oscil <SIN256_NUM_CELLS, AUDIO_RATE> lfo1(SIN256_DATA);
Oscil <TRIANGLE512_NUM_CELLS, AUDIO_RATE> lfo2(TRIANGLE512_DATA);
Oscil <SAW256_NUM_CELLS, AUDIO_RATE> lfo3(SAW256_DATA);
Oscil <WAVESHAPE_SIGMOID_NUM_CELLS, AUDIO_RATE> lfo4(WAVESHAPE_SIGMOID_DATA);
Oscil <SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> lfo5(SQUARE_NO_ALIAS512_DATA);
Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> lfo6(WHITENOISE8192_DATA);
Oscil <UPHASOR256_NUM_CELLS, AUDIO_RATE> sah(UPHASOR256_DATA); // used for sample&hold

//LowPassFilter lp;
StateVariable <LOWPASS> lp; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
StateVariable <HIGHPASS> hp; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
AudioDelayFeedback <MAXDELAY> fbkDelay;

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
// FILTER
bool switch2 = 0; // 0=LP, 1=HP
Q16n0 freqknob; // cutoff
Q0n8 res;
const IntMap lpFreqMap(0, 255, LOWESTFREQ, HIGHESTLPFREQ);
const IntMap hpFreqMap(0, 255, LOWESTFREQ, HIGHESTHPFREQ); // filter freq mapping
const IntMap hpResMap(255, 0, LOWESTRES, HIGHESTRES);

// DELAY
uint16_t delaysize;
uint16_t delaymod; 
int8_t delayfbk; 

// DISTORTION
uint8_t distortion_mode; // 0 to 7 (see SYNTH distortion modes in GLOBALS.h)
uint8_t rnd; 

// LFO
int8_t lfosig;
uint8_t lfowave = 0; 
uint8_t lforate; 

// MOD
bool switch1 = false;
int8_t modknob; 

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/
void setup(){
	startMozzi(CONTROL_RATE); // uses the default control rate of 64, defined in mozzi_config.h  
	setupFastAnalogRead(FASTEST_ADC);
	
	// HARDWARE
	pinMode(SWITCH1, INPUT);
	pinMode(SWITCH2, INPUT);
	pinMode(LED, OUTPUT);
	// LFO
	testosc.setFreq(2); // set the frequency
	// FILTER
	lp.setResonance(127); // 0 to 255, 255 is the "sharp" end
	hp.setResonance(127); // 0 to 255, 0 is the "sharp" end
	// DELAY
	fbkDelay.setFeedbackLevel(-111); // can be -128 to 127
	// LFO	
}

/************************************************************************/
/* UPDATE CONTROL                                                       */
/************************************************************************/
void updateControl(){
	// DISTORTION 
	distortion_mode = mozziAnalogRead(KNOB6) >> 7; // read knob (0..7)
	// MOD
	modknob = mozziAnalogRead(KNOB3) >> 2;
	// DELAY
	delaysize = (1023 - (mozziAnalogRead(KNOB1) ) >> 2);
	delayfbk = modknob - 128;
	(delayfbk < FBKMIN) ? (delayfbk = FBKMIN) : 
	(delayfbk > FBKMAX) ? (delayfbk = FBKMAX) : delayfbk;
	delayfbk = delayfbk << 1;
	fbkDelay.setFeedbackLevel(delayfbk); // can be -128 to 127
	
	// FILTER
	switch2 = bit_get(PIND, BIT(SWITCH2));		// read SWITCH2
	freqknob =  mozziAnalogRead(KNOB4) >> 2;	// read knob
	lp.setCentreFreq( lpFreqMap(freqknob) );	// set LP freq
	hp.setCentreFreq( hpFreqMap(freqknob) );	// set HP freq
	
	res = modknob;
	(res > 128) ? (res = 127 - res) : res;  
	res = res << 1;
	lp.setResonance( res );			// set LP res
	hp.setResonance( hpResMap(res) );		// set HP res */
		
	// LFO
	//float rate = ipow( mozziAnalogRead(KNOB2), 2) / 8176.0078125;
	lforate = mozziAnalogRead(KNOB2) >> 4;
	lfowave = mozziAnalogRead(KNOB5) >> 7;  
	lfo1.setFreq(lforate);
	lfo2.setFreq(lforate);
	lfo3.setFreq(lforate);
	lfo4.setFreq(lforate);
	lfo5.setFreq(lforate);
	lfo6.setFreq(1);
	lfo6.setPhase(rand(128));
	sah.setFreq(lforate);
	
	// MOD
	switch1 = bit_get(PIND, BIT(SWITCH1));		// read SWITCH2	
}
/************************************************************************/
/* UPDATE AUDIO                                                         */
/************************************************************************/
int updateAudio(){
	// test signal (put audio in here...)
	//int outsig = testosc.next()>>1; // divide by half to avoid svf distortion on high Q
	int outsig = (getAudioInput() -512) >> 3;
	
	// LFO 
	lfosig = lfo1.next(); 
	lightled( lfosig ); // LED
	(lfowave < 1) ? ( lfosig = lfosig ) :
	(lfowave < 2) ? ( lfosig = lfo2.next() ) :
	(lfowave < 3) ? ( lfosig = lfo3.next() ) :
	(lfowave < 4) ? ( lfosig = lfo4.next() ) :
	(lfowave < 5) ? ( lfosig = lfo5.next() ) :
	(lfowave < 6) ? ( 
		(sah.next() < 1) ? (lfosig = lfo5.next()) : (lfosig)
		) :
	(lfowave < 7) ? (
		(sah.next() < 1) ? (lfosig = (outsig + 128) >> 1) : (lfosig)
	) :
	(lfowave < 7) ? (
		(sah.next() < 1) ? (lfosig = (255 - (outsig + 128)) >> 1) : (lfosig)
	) : lfosig;
		
	// SIGNAL CHAIN: Filter > Delay > FX 
	// apply LP or HP filter
	(switch2 < 1) ? (outsig = lp.next(outsig)) : (outsig = hp.next(outsig));
	// delay modulation
	(switch1 > 0) ? (delaymod = (lfosig * delaysize) >> 7) : (delaymod = delaysize);
	// turn delay on/off
	(delaysize > 0) ? (outsig = (outsig + fbkDelay.next(outsig, delaymod)) >> 1) : outsig;
	

	// faster "switch statement" (cascading if then else)
	(distortion_mode < MAND) ? (outsig = outsig) : // OFF
	(distortion_mode < MOR) ? (outsig = (outsig & -lfosig)) : // AND 
	(distortion_mode < MXOR) ? (outsig = (outsig | lfosig)) : // OR
	(distortion_mode < MNOT) ? (outsig = (outsig ^ lfosig)) : // XOR
	(distortion_mode < MSHIFT) ? (outsig = (outsig + ~lfosig)) : // NOT
	(distortion_mode < MRND1) ? (outsig = (outsig << lfosig)) : // BITSHIFT
	(distortion_mode < MRND2) ? (outsig = ( (outsig << rand(RNDSHIFT))>>7 )) : (outsig = (distortions(outsig, (lfosig + 128)>>5 ))); // CTRL RATE else AUDIO RATE
	
	
	/*switch(distortion_mode){
		case MAND:
		outsig = outsig & -lfosig;
		break;
		case MOR:
		outsig = outsig | lfosig;
		break;
		case MXOR:
		outsig = outsig ^ lfosig;
		break;
		case MNOT:
		outsig = outsig + ~lfosig;
		break;
		case MSHIFT:
		outsig = outsig << lfosig;
		break;
		case MRND2:
		outsig = outsig << rand(250);
		break;
		case MRND1:
		outsig = outsig << rnd;
		break;
		
		default:
		break;
	}*/
		
	return outsig << 4; // <<4
	// output must be from -8192 to 8191
}

void loop(){
  audioHook(); // required here
}

/************************************************************************/
/* OTHER FUNCTIONS                                                      */
/************************************************************************/
void lightled(int8_t sig){
	// METHOD 1
	(sig > 0) ? PORTD |=(1<<3) : PORTD &= ~(1 << 3);
	
	// METHOD 2
	/*if (sig > 0) PORTD |=(1<<3);
	else PORTD &= ~(1 << 3);*/
	
	// METHOD 3
	//PORTD = (filtermode<<PD3);
}

int distortions(int sig, uint8_t dist){
	// faster "switch statement" (cascading if then else)
	(dist < 1) ? (sig = (sig << rand(RNDSHIFT)) >> 7) : // rnd distortion
	(dist < 2) ? (sig = sig) : // OFF
	(dist < 3) ? sig = (sig & -lfosig) : // AND
	(dist < 4) ? sig = (sig | lfosig) : // OR
	(dist < 5) ? sig = (sig ^ lfosig) : // XOR
	(dist < 6) ? sig = (sig + ~lfosig) : // NOT
	(dist < 7) ? sig = (sig << lfosig) : sig; // BITSHIFT
	
	return sig; 
}