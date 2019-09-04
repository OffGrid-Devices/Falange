/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
#include "GLOBALS.h" 
 
#include <MozziGuts.h>
#include <IntMap.h>
#include <Oscil.h> 
#include <StateVariable.h>
#include <LowPassFilter.h>

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


Oscil <WHITENOISE8192_NUM_CELLS, AUDIO_RATE> testosc(WHITENOISE8192_DATA);
LowPassFilter lopass;
//StateVariable <LOWPASS> lopass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
StateVariable <HIGHPASS> hipass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH

bool filtermode = 0; 
Q16n0 freq; 
const IntMap freqMap(0,1023,20,4096);


void setup(){
	startMozzi(); // uses the default control rate of 64, defined in mozzi_config.h  
	setupFastAnalogRead(FASTEST_ADC);
	
	// HARDWARE
	pinMode(SWITCH1, INPUT);
	pinMode(SWITCH2, INPUT);
	pinMode(LED, OUTPUT);
	// LFO
	testosc.setFreq(2); // set the frequency
	// FILTER
	lopass.setResonance(127); // 0 to 255, 0 is the "sharp" end
	hipass.setResonance(127); // 0 to 255, 0 is the "sharp" end
}

void lightled(){
	// METHOD 1
	/*if (filtermode) PORTD |=(1<<3);
	else PORTD &= ~(1 << 3);*/
	
	// METHOD 2
	PORTD = (filtermode<<PD3);
}

void updateControl(){
	
	filtermode = bit_get(PIND, BIT(7)); // read SWITCH2
	
	
	freq =  mozziAnalogRead(KNOB4);
	hipass.setCentreFreq( freqMap(freq) );
	lopass.setCutoffFreq( freq >> 2 ); 
	 
	
	//lightled();
}



int updateAudio(){
	int outsig = testosc.next() >> 1; // divide by half to avoid svf distortion on high Q
	
	if (filtermode) outsig = hipass.next(outsig);
	else outsig = lopass.next(outsig); 
	
	// output must be from -8192 to 8191	
	return outsig << 4;
}


void loop(){
  audioHook(); // required here
}
