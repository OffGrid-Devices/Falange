#include <MozziGuts.h>
#include <Oscil.h>

#include <AudioDelayFeedback.h>
#include <tables/sin256_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/waveshape_chebyshev_3rd_256_int8.h>
#include <tables/brownnoise8192_int8.h>
#include <tables/whitenoise8192_int8.h>


#define CONTROL_RATE 128 // powers of 2 please

Oscil<TRIANGLE512_NUM_CELLS, CONTROL_RATE> lfo(TRIANGLE512_DATA); // for modulating delay time, measured in audio samples

AudioDelayFeedback <256> aDel;

// the delay time, measured in samples, updated in updateControl, and used in updateAudio 
byte del_samps;
Q16n16 del_samps_fractional;
float lfofreq; 
void setup(){
  startMozzi(CONTROL_RATE);
  lfo.setFreq(.163f); // set the delay time modulation frequency (ie. the sweep frequency)
  aDel.setFeedbackLevel(-111); // can be -128 to 127
  setupFastAnalogRead(FASTEST_ADC);
}


void updateControl(){
  
  lfofreq = mozziAnalogRead(3) / 128.f;
  lfo.setFreq(lfofreq);
  aDel.setFeedbackLevel( (mozziAnalogRead(5) >> 2) -128 ); // -128 to 127
  del_samps_fractional = Q8n0_to_Q16n16(mozziAnalogRead(2)>>3) + ( (long)lfo.next()<<(mozziAnalogRead(4)>>6) ); 

}


int updateAudio(){
  char asig = (getAudioInput() - 512) >>1; 
  return (asig<<1) + (aDel.next(asig, del_samps_fractional)<<5); // mix some straight signal with the delayed signal
}


void loop(){
  audioHook();
}



