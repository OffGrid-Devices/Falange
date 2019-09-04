/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/* OffGrid-Devices "Falange" by Tiago Ângelo 2018 (inspired by a MaxMSP tutorial by Maurizio Giri)
 version 0.1
 using Mozzi v1.0.3rc6 and Arduino v.1.6.5
 Tiago Ângelo 2018
 Instructions to upload firmware:
 1 - open file "mozzi_config.h" and set:
 2 - AUDIO_MODE HIFI
 3 - AUDIO_RATE 16384
 4 - USE_AUDIO_INPUT true
 5 - AUDIO_INPUT_PIN 0 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <AudioDelayFeedback.h>
#include <StateVariable.h>

#include "GLOBALS.h"

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
//End of Auto generated function prototypes by Atmel Studio

#define CONTROL_RATE 1024

// Synth class objects
//AudioDelayFeedback <256> fbkDelay; //max delay time = 256/16384 = 15.625ms

Oscil<SIN256_NUM_CELLS, CONTROL_RATE> testosc(SIN256_DATA);
Oscil<TRIANGLE512_NUM_CELLS, CONTROL_RATE> lfo(TRIANGLE512_DATA); // lfo to modulate delay size
Oscil <UPHASOR256_NUM_CELLS, AUDIO_RATE> sah(UPHASOR256_DATA); // used to sample&hold


Q8n0 mknob1, mknob2, mknob3, mknob4, mknob5, mknob6; 

void setup(){
  startMozzi(CONTROL_RATE);
  setupFastAnalogRead(FASTEST_ADC);
  
  //fbkDelay.setFeedbackLevel(-111); // can be -128 to 127
 
  lfo.setFreq(.163f);
  sah.setFreq(41.5f);
  testosc.setFreq(440.f);
  
  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
  pinMode(LED, OUTPUT);
}


/*void updateControl(){
	smoothFreq = mozziAnalogRead(SMOOTH) >> 2; // 0 to 255 is 0~8192Hz
	smooth.setCutoffFreq( smoothFreq );
	//lfo.setFreq( mozziAnalogRead(LFOFREQ) ); // 0 to 1024
	float f = mozziAnalogRead(3) / 128.f;
	sah.setFreq( f ); // 0 to 1024
	dlySizeParam = mozziAnalogRead(4) / 1023.f; // 0.0 to 1.0
	mDelay.setFeedbackLevel( (mozziAnalogRead(FEEDBACK) >> 2) -128 ); // -128 to 127
	
	 
}
*/

void updateControl(){
	
}
int updateAudio(){
	
	// output range should be from -8192 to 8191
	return testosc.next() <<6;
}

/*int updateAudio(){
  // read audio input 
  int16_t inSig = getAudioInput() - 512; // range from 0-1023 to -512 to 511
  
	 // calc delay-size
	 if (sah.next() > 253){
		 if(smoothFreq > 0) delaySize = Q8n0_to_Q16n16(1) + (smooth.next( ((long)(lfo.next() + 255) << 1)) * dlySizeParam);
		 else delaySize = Q8n0_to_Q16n16(1) + (((long)(lfo.next() + 255) << 11) * dlySizeParam); // disable smooth for granular effects
	 }
  
  // output range must be between -8192 and 8191 (14-bit) 
  return (inSig + mDelay.next(inSig, delaySize)) << 4; 
}*/


void loop(){
  audioHook();
}





