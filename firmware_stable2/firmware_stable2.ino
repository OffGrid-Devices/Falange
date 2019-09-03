#include <MozziGuts.h>
#include <Oscil.h>

#include <AudioDelayFeedback.h>
#include <StateVariable.h>

#include <tables/sin256_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/waveshape_chebyshev_3rd_256_int8.h>
#include <tables/waveshape_sigmoid_int8.h>
#include <tables/waveshape_tanh_int8.h>


#define CONTROL_RATE 128 // powers of 2 please

Oscil<SIN256_NUM_CELLS, CONTROL_RATE> lfo(SIN256_DATA); // for modulating delay time, measured in audio samples
AudioDelayFeedback <256> aDel;
StateVariable <LOWPASS> lpf; // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH

// the delay time, measured in samples, updated in updateControl, and used in updateAudio 
Q16n16 del_samps_fractional;
float lfofreq; 
uint8_t table, _table = 0; 
//uint8_t fx; 

void setup(){
  startMozzi(CONTROL_RATE);
  pinMode(4, INPUT); //switch 1
  pinMode(7, INPUT); //switch 2
  lfo.setFreq(.163f); // set the delay time modulation frequency (ie. the sweep frequency)
  aDel.setFeedbackLevel(-111); // can be -128 to 127 
  lpf.setResonance(128); // 0 to 255, 0 is the "sharp" end
  
  setupFastAnalogRead(FASTEST_ADC);
}


void updateControl(){

  if(digitalRead(4)){
    lfofreq = mozziAnalogRead(3) / 32.f;
    lfo.setFreq(lfofreq);
  }
  else{
    lfofreq = mozziAnalogRead(3) / 512.f;
    lfo.setFreq(lfofreq);
  }
  
  
  aDel.setFeedbackLevel( (mozziAnalogRead(5) >> 2) -128 ); // -128 to 127
  
  if(digitalRead(7)) del_samps_fractional = Q8n0_to_Q16n16(mozziAnalogRead(7)>>2) + ( (long)lfo.next()<<(mozziAnalogRead(4)>>6) ); 
  else del_samps_fractional = Q8n0_to_Q16n16(mozziAnalogRead(7)>>5) + ( (long)lfo.next()<<(mozziAnalogRead(4)>>6) ); 
  
  lpf.setCentreFreq( (mozziAnalogRead(2)<<2) + 120 );
  //fx = 1 + ( mozziAnalogRead(7) >> 6);
  
  table = mozziAnalogRead(6)>>7; 
  if(table != _table){
    _table = table;
    switch(table){
      case 0: lfo.setTable(SIN256_DATA); break;
      case 1: lfo.setTable(TRIANGLE512_DATA); break;
      case 2: lfo.setTable(SAW256_DATA); break;
      case 3: lfo.setTable(PHASOR256_DATA); break;
      case 4: lfo.setTable(SQUARE_NO_ALIAS512_DATA); break;
      case 5: lfo.setTable(CHEBYSHEV_3RD_256_DATA); break;
      case 6: lfo.setTable(WAVESHAPE_SIGMOID_DATA); break;
      case 7: lfo.setTable(WAVESHAPE_TANH_DATA); break;
      default: break; 
    }
  }
}


int updateAudio(){
  char asig = lpf.next( (getAudioInput() - 512) >>1 );
  //char asig = (getAudioInput() - 512) >>1; 
  return (asig<<1) + (aDel.next(asig, del_samps_fractional)<<5); // mix some straight signal with the delayed signal
}


void loop(){
  audioHook();
}



