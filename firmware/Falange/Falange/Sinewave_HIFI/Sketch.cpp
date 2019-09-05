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

#include <tables/sin256_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/brownnoise8192_int8.h>
#include <tables/uphasor256_uint8.h>
#include <tables/whitenoise8192_int8.h>

//Beginning of Auto generated function prototypes by Atmel Studio
void updateControl();
int updateAudio();

void lightled(int8_t sig);
//End of Auto generated function prototypes by Atmel Studio

/************************************************************************/
/* OBJECTS                                                              */
/************************************************************************/
Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> testosc(WHITENOISE8192_DATA);
Oscil <SIN256_NUM_CELLS, AUDIO_RATE> lfo(SIN256_DATA);
//LowPassFilter lp;
StateVariable <LOWPASS> lp; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
StateVariable <HIGHPASS> hp; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
AudioDelayFeedback <256> fbkDelay;
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
// FILTER
bool filtermode = 0; // 0=LP, 1=HP
Q16n0 freq; // cutoff
Q0n8 res;
const IntMap lpFreqMap(0, 255, LOWESTFREQ, HIGHESTLPFREQ);
const IntMap hpFreqMap(0, 255, LOWESTFREQ, HIGHESTHPFREQ); // filter freq mapping
const IntMap hpResMap(255, 0, LOWESTRES, HIGHESTRES);

// DELAY
bool delayplace = 0; // 0 = post fx+filter, 1 = pre fx+filter
uint16_t delaytime;
int8_t amount; 
int8_t lfosig; 

// DISTORTION
uint8_t distortion_mode; // 0 to 7 (see SYNTH distortion modes in GLOBALS.h)
uint8_t rnd; 

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
		 
	// FILTER
	filtermode = bit_get(PIND, BIT(7));		// read SWITCH2
	freq =  mozziAnalogRead(KNOB4) >> 2;	// read knob
	
	lp.setCentreFreq( lpFreqMap(freq) );	// set LP freq
	hp.setCentreFreq( hpFreqMap(freq) );	// set HP freq
	/*res = 255 - (mozziAnalogRead(KNOB3) >> 2);// read pot
	lp.setResonance( res );			// set LP res
	hp.setResonance( hpResMap(res) );		// set HP res */
	
	
	// DELAY
	delayplace = bit_get(PIND, BIT(4));		// read SWITCH1
	delaytime = (1023 - (mozziAnalogRead(KNOB1) ) >> 2) + 1;
	fbkDelay.setDelayTimeCells(delaytime);
	
	// MOD AMOUNT 
	amount = (mozziAnalogRead(KNOB3) >> 2) - 128;
	fbkDelay.setFeedbackLevel(amount); // can be -128 to 127
	// LFO
	//float rate = ipow( mozziAnalogRead(KNOB2), 2) / 8176.0078125;
	//lfo.setFreq( rate );
	lfo.setFreq( mozziAnalogRead(KNOB2) >> 4);
	// DISTORTION 
	if(distortion_mode == MRND2) rnd = rand(16); 
	
}
/************************************************************************/
/* UPDATE AUDIO                                                         */
/************************************************************************/
int updateAudio(){
	// LED
	lfosig = lfo.next();
	lightled( lfosig );
	
	int outsig = testosc.next()>>1; // divide by half to avoid svf distortion on high Q
	
	outsig = lp.next(outsig);

	// faster "switch statement" (cascading if then else)
	(distortion_mode < MAND) ? (outsig = outsig) : // OFF
	(distortion_mode < MOR) ? outsig = (outsig & -lfosig) : // AND 
	(distortion_mode < MXOR) ? outsig = (outsig | lfosig) : // OR
	(distortion_mode < MNOT) ? outsig = (outsig ^ lfosig) : // XOR
	(distortion_mode < MSHIFT) ? outsig = (outsig + ~lfosig) : // NOT
	(distortion_mode < MRND1) ? outsig = (outsig << lfosig) : // BITSHIFT
	(distortion_mode < MRND2) ? outsig = (outsig << rnd) : outsig = ( outsig << rand(16) ); // CTRL RATE else AUDIO RATE
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
	
	outsig = ( outsig + fbkDelay.next( outsig ) ) >> 1;
	return outsig << 6; // <<4
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
