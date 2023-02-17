#include <Arduino.h>

//#include "Stopwatch.h"
#include "TeensyPin.h"
#include "Axial.h"

Axial ax;

OutputPin pinLED{};

/**
 * This runs once when the microcontroller boots up.
**/
void setup() {
	pinLED = OutputPin::getLED();
//	Stopwatch myTimer;
//	myTimer.start("Startup");
	pinLED.blink(50, 50);

	Serial.begin(0); // Will always be at USB speed
	while (!Serial && millis() < 5000) {} // Wait up to 5s for serial connection to be established
//	myTimer.lap("Startup", true);

	ax.init();

//	myTimer.stop("Startup", true);
	pinLED.blink(50, 50);
	pinLED.blink(100, 500);
}

/**
 * After `setup()` has finished, this runs repeatedly while the microcontroller is powered on.
**/
void loop() {
	pinLED.blink(1, 10); // Long enough to be visible, short enough to be fast

	ax.scanKeyboard();
}