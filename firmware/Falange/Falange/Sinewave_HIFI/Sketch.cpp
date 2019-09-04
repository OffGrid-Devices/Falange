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

void lightled();
//End of Auto generated function prototypes by Atmel Studio

/************************************************************************/
/* OBJECTS                                                              */
/************************************************************************/
Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> testosc(WHITENOISE8192_DATA);
//LowPassFilter lopass;
StateVariable <LOWPASS> lopass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
StateVariable <HIGHPASS> hipass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
AudioDelayFeedback <256> fbkDelay;
/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
bool filtermode = 0; // 0=LP, 1=HP
bool filterplace = 0; // 1 = post delay, 0 = pre delay 
Q16n0 freq; // cutoff
const IntMap freqMap(0,1023,20,4096); // HP freq mapping

uint16_t delaytime;

int8_t amount; 

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
	lopass.setResonance(127); // 0 to 255, 255 is the "sharp" end
	hipass.setResonance(127); // 0 to 255, 0 is the "sharp" end
	// DELAY
	fbkDelay.setFeedbackLevel(-111); // can be -128 to 127
		
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
	filterplace = bit_get(PIND, BIT(4));		// read SWITCH2
	filtermode = bit_get(PIND, BIT(7));		// read SWITCH2
	freq =  mozziAnalogRead(KNOB4);			// read knob
	hipass.setCentreFreq( freqMap(freq) );	// set HP freq
	//lopass.setCutoffFreq( freq >> 2 );		// set LP freq
	lopass.setCentreFreq( freqMap(freq) );
	
	// DELAY
	delaytime = (1023 - (mozziAnalogRead(KNOB1) ) >> 2) + 1;
	//fbkDelay.setDelayTimeCells(delaytime);
	
	// MOD AMOUNT 
	amount = (mozziAnalogRead(KNOB3) >> 2) - 128;
	fbkDelay.setFeedbackLevel(amount); // can be -128 to 127
	
	// LED
	lightled();
}
/************************************************************************/
/* UPDATE AUDIO                                                         */
/************************************************************************/
int updateAudio(){
	int outsig = testosc.next() >> 2; // divide by half to avoid svf distortion on high Q
	
	if(filterplace){ // filter post delay
		// signal to filter should be from -128 to 127 (actually 244 >> 1)
		outsig = outsig + fbkDelay.next(outsig, delaytime);
		if (filtermode) outsig = hipass.next(outsig);
		else outsig = lopass.next(outsig);
	}
	else{ // filter pre delay
		if (filtermode) outsig = hipass.next(outsig);
		else outsig = lopass.next(outsig);
		outsig = outsig + fbkDelay.next(outsig, delaytime);
	}
	
	
	// output must be from -8192 to 8191	
	return outsig << 4;
}


void loop(){
  audioHook(); // required here
}

/************************************************************************/
/* OTHER FUNCTIONS                                                      */
/************************************************************************/
void lightled(){
	// METHOD 1
	/*if (filtermode) PORTD |=(1<<3);
	else PORTD &= ~(1 << 3);*/
	
	// METHOD 2
	PORTD = (filtermode<<PD3);
}
