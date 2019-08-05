#ifndef Trig_h
#define Trig_h

#include <Bounce2.h>

#define TRIG_LED 11
#define TRIG_CV 9

class Trig {
	public:
		Trig() : trigCV(TRIG_CV,40) {

		};
		void init(boolean pulse_output);
		void led(boolean high);
		void out(boolean high);
		void update();

	private:
		Bounce trigCV;
		elapsedMillis pulseOutTimer = 0;
		uint32_t pulseTime = 10;
		boolean pulseHigh = false;
    boolean output_pulses = true;
		boolean resetButton = false;
		boolean resetCVRose = false;
		elapsedMillis resetLedTimer = 0;
};

#endif
