#ifndef SQUARE_NO_ALIAS512_INT8_H_
#define SQUARE_NO_ALIAS512_INT8_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <avr/pgmspace.h>

/* square wave with rounded corners from Audacity
*/

#define SQUARE_NO_ALIAS512_NUM_CELLS 512
#define SQUARE_NO_ALIAS512_SAMPLERATE 512

const int8_t __attribute__((section(".progmem.data"))) SQUARE_NO_ALIAS512_DATA []  =
        {
                -1, 93, 117, 114, 115,
                114, 115, 115, 114, 115, 114, 115, 115, 114, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115,
                115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 114, 115,
                115, 114, 115, 114, 115, 115, 114, 115, 114, 117, 93, 0, -94, -118, -115, -116,
                -115, -116, -116, -115, -116, -115, -116, -116, -115, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116, -116,
                -116, -116, -116, -116, -115, -116, -116, -116, -116, -115, -116, -116, -115,
                -116, -115, -118, -94,
        };

#endif /* SQUARE_NO_ALIAS512_INT8_H_ */