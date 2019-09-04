// OffGrid-Devices "Falange" by Tiago Ângelo 2018 (inspired by a MaxMSP tutorial by Maurizio Giri)
// version 0.1
// using Mozzi v1.0.3rc6 and Arduino v.1.6.5
// Tiago Ângelo 2018
// Instructions to upload firmware: 
// 1 - open file "mozzi_config.h" and set: 
// 2 - AUDIO_MODE HIFI
// 3 - AUDIO_RATE 16384
// 4 - USE_AUDIO_INPUT true
// 5 - AUDIO_INPUT_PIN 0

#include <MozziGuts.h>
#define CONTROL_RATE 64

// Hardware Pin definition 
#define LED 3
#define LFORANGE 4
#define SIZERANGE 7
#define SMOOTH 2
#define LFOFREQ 3
#define SIZE 4
#define FEEDBACK 5
#define LFOWAVE 6
#define FILTER 7

void setup(){
  startMozzi();
  //setupFastAnalogRead(FASTEST_ADC);
}


void updateControl(){
  // read hardware interface
  
}


int updateAudio(){
  int asig = getAudioInput(); // range 0-1023
  asig = asig - 512; // output range must be between -8192 and 8191 (14-bit)
  
  return asig;
}


void loop(){
  audioHook();
}





