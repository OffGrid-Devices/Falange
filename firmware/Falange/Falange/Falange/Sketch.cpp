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

#include <tables/sin256_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/brownnoise8192_int8.h>

#include <tables/uphasor256_uint8.h>
#include <tables/whitenoise8192_int8.h>

#include "FalangeHardware.h"

//Beginning of Auto generated function prototypes by Atmel Studio
void updateControl();
int updateAudio();
//End of Auto generated function prototypes by Atmel Studio

#define CONTROL_RATE 1024


AudioDelayFeedback <256> mDelay; //max delay time = 256/16384 = 15.625ms
Q8n24 delaySize = 100; // in samples (or cells as is called in Mozzi)

void setup(){
  startMozzi(CONTROL_RATE);
  mDelay.setFeedbackLevel(-111); // can be -128 to 127
}


void updateControl(){
}


int updateAudio(){
  int16_t inSig = getAudioInput() - 512; // range from 0-1023 to -512 to 511
  
  // output range must be between -8192 and 8191 (14-bit) 
  return (inSig + mDelay.next(inSig, delaySize)) << 4; 
}


void loop(){
  audioHook();
}





