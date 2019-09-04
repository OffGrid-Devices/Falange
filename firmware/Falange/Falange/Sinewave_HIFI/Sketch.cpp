/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
#include "GLOBALS.h" 
 
#include <MozziGuts.h>
#include <Oscil.h> 
#include <StateVariable.h>

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
StateVariable <LOWPASS> lopass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH
StateVariable <HIGHPASS> hipass; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH

bool filtermode = 0; 

void setup(){
	startMozzi(); // uses the default control rate of 64, defined in mozzi_config.h  
	// HARDWARE
	pinMode(SWITCH1, INPUT);
	pinMode(SWITCH2, INPUT);
	pinMode(LED, OUTPUT);
	// LFO
	testosc.setFreq(440); // set the frequency
	// FILTER
	lopass.setResonance(255); // 0 to 255, 0 is the "sharp" end
	lopass.setCentreFreq(400);
	hipass.setResonance(255); // 0 to 255, 0 is the "sharp" end
	hipass.setCentreFreq(400);
}

void lightled(){
	// METHOD 1
	/*if (filtermode) PORTD |=(1<<3);
	else PORTD &= ~(1 << 3);*/
	
	// METHOD 2
	PORTD = (filtermode<<PD3);
}

void updateControl(){
	filtermode = bit_get(PIND, BIT(7));
	
	//lightled();
}



int updateAudio(){
	// output must be from -8192 to 8191
	int outsig;
	 	
	if (filtermode) outsig = hipass.next(testosc.next());
	else outsig = lopass.next(testosc.next()); 
	
	return outsig <<6;
}


void loop(){
  audioHook(); // required here
}
