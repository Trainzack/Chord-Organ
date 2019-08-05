#ifndef ClockIn_h
#define ClockIn_h

#include <Bounce2.h>

#define CLOCK_LED 11

class ClockIn {
	public:
		/*ClockIn() : ClockCV(CLOCK_CV,40) {

		};*/
		void init();
		void led(boolean high);
		void out(boolean high);
		void update();

	private:
		Bounce ClockCV;
		elapsedMillis pulseOutTimer = 0;
		uint32_t pulseTime = 10;
		boolean pulseHigh = false;
    boolean pulseDetected = false;
		boolean resetButton = false;
		boolean resetCVRose = false;
		elapsedMillis resetLedTimer = 0;
};

#endif
