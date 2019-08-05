#include "Arduino.h"
#include "ClockIn.h"

void ClockIn::init() {
	pinMode(CLOCK_LED, OUTPUT);
//    pinMode(CLOCK_CV, INPUT);
}

void ClockIn::led(boolean high) {
    digitalWrite(CLOCK_LED, high ? HIGH : LOW);
}

void ClockIn::update() {
    if (!pulseHigh){
        ClockCV.update();
        resetCVRose = ClockCV.rose();
        if (resetCVRose) {
        	resetLedTimer = 0;
        }
        led(resetLedTimer < 20);
    } else if (pulseOutTimer > pulseTime) {
    	out(false);
    }
    
}


void ClockIn::out(boolean isHigh) {
	if(isHigh) {
//		Serial.println("TRIG OUT");
	}
	pulseHigh = isHigh;
	led(isHigh);
	if(isHigh) {
		pulseOutTimer = 0;
	}
}
