#include <Arduino.h>

#include "Axial.h"
#include "Stopwatch.h"

Stopwatch myTimer;
Axial ax;

/**
 * This runs once when the microcontroller boots up.
**/
void setup() {
	myTimer.start("Startup");
	const OutputPin &pinLED = OutputPin::getLED();
	pinLED.blink(50, 50);

	Serial.begin(0); // Will always be at USB speed
	while (!Serial && millis() < 5000) {} // Wait up to 5s for serial connection to be established
	myTimer.lap("Startup", true);

	ax.init();

	myTimer.stop("Startup", true);
	pinLED.blink(50, 50);
	pinLED.blink(100, 500);
}

/**
 * After `setup()` has finished, this runs repeatedly while the microcontroller is powered on.
**/
void loop() {
	OutputPin::getLED().occlude(20,1500);
//	OutputPin::getLED().off();delay(5);OutputPin::getLED().on();
}