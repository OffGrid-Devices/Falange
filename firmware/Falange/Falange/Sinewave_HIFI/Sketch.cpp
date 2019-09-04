/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/************************************************************************/
/* INCLUDES                                                                     */
/************************************************************************/
#include "GLOBALS.h" 

#include <MozziGuts.h>
#include <IntMap.h>
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
const IntMap freqMap(0, 255, LOWESTFREQ, HIGHESTFREQ); // filter freq mapping

// DELAY
bool delayplace = 0; // 0 = post fx+filter, 1 = pre fx+filter
uint16_t delaytime;
int8_t amount; 
int8_t lfosig; 

// DISTORTION
uint8_t distortion_mode; // 0 to 7 (see SYNTH distortion modes in GLOBALS.h)
uint8_t pot_samples[4];
uint8_t counter = 0; 

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
	// DISTORTION (pot is noisy....should be replaced or filtered)
	pot_samples[counter] = mozziAnalogRead(KNOB6) >> 7; // read knob (0..7)
	counter++; 
	if (counter > 3){ // after 4 samples calculate pot value 
		counter = 0;
		distortion_mode = 0; 
		for (int i = 0; i < 4; i++)
		{
			distortion_mode += pot_samples[i];
		}
		distortion_mode = distortion_mode >> 2; 
	} 
		 
	// FILTER
	delayplace = bit_get(PIND, BIT(4));		// read SWITCH2
	filtermode = bit_get(PIND, BIT(7));		// read SWITCH2
	freq =  mozziAnalogRead(KNOB4) >> 2;			// read knob
	hp.setCentreFreq( freqMap(freq) );	// set HP freq
	//lopass.setCutoffFreq( freq >> 2 );		// set LP freq
	lp.setCentreFreq( freqMap(freq) );
	res = 255 - (mozziAnalogRead(KNOB3) >> 2);
	lp.setResonance( res );
	
	// DELAY
	delaytime = (1023 - (mozziAnalogRead(KNOB1) ) >> 2) + 1;
	//fbkDelay.setDelayTimeCells(delaytime);
	
	// MOD AMOUNT 
	amount = (mozziAnalogRead(KNOB3) >> 2) - 128;
	fbkDelay.setFeedbackLevel(amount); // can be -128 to 127
	// LFO
	float rate = ipow( mozziAnalogRead(KNOB2), 2) / 8176.0078125;
	lfo.setFreq(rate);
	
}
/************************************************************************/
/* UPDATE AUDIO                                                         */
/************************************************************************/
int updateAudio(){
	// LED
	/*lfosig = lfo.next();
	lightled( lfosig );*/
	
	int outsig = testosc.next()>>1; // divide by half to avoid svf distortion on high Q
	
	/*if(delayplace){ // FX > Filter > Delay
		if (filtermode) outsig = hipass.next(outsig>>1);
		else outsig = lopass.next(outsig>>1);
		outsig = outsig + fbkDelay.next(outsig, delaytime);
	}
	else{ // Delay > FX > Filter
		// signal to filter should be from -128 to 127 (actually 244 >> 1)
		outsig = outsig + fbkDelay.next(outsig, delaytime);
		if (filtermode) outsig = hipass.next(outsig>>2);
		else outsig = lopass.next(outsig>>2);
	}
	// output must be from -8192 to 8191
	return outsig << 6;	
	*/
	
	// output must be from -8192 to 8191
	return lp.next(outsig) << 4;
	
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
