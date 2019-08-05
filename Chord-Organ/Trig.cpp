#include "Arduino.h"
#include "Trig.h"

void Trig::init(boolean pulse_output) {
	pinMode(TRIG_LED, OUTPUT);
    output_pulses = pulse_output;
    if (output_pulses) {
      pinMode(TRIG_CV, OUTPUT);
    }
}

void Trig::led(boolean high) {
    digitalWrite(TRIG_LED, high ? HIGH : LOW);
}

void Trig::update() {
	// Only if TRIG is being used as an input.
    if (!pulseHigh){
        trigCV.update();
        resetCVRose = trigCV.rose();
        if (resetCVRose) {
        	resetLedTimer = 0;
        }
        led(resetLedTimer < 20);
    } else if (pulseOutTimer > pulseTime) {
    	out(false);
    }
}

void Trig::out(boolean isHigh) {
	if(isHigh) {
//		Serial.println("TRIG OUT");
	}
	pulseHigh = isHigh;
 
  if (output_pulses) {
	  digitalWrite(TRIG_CV, isHigh ? HIGH : LOW);
  }
	led(isHigh);
	if(isHigh) {
		pulseOutTimer = 0;
	}
}
