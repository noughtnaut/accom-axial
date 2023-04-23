#include <Arduino.h>

//#include "Stopwatch.h"
//#include "TeensyPin.h"
//#include "Axial.h"

//Axial ax;

/**
 * This runs once when the microcontroller boots up.
**/
void setup() {
	Serial.begin(0); // Will always be at USB speed
	while (!Serial && millis() < 5000) {} // Wait up to 5s for serial connection to be established

//	ax.init();
//	OutputPin::getLED().blink(20, 10);
//	OutputPin::getLED().blink(20, 10);
//	OutputPin::getLED().blink(20, 100);
}

/**
 * After `setup()` has finished, this runs repeatedly while the microcontroller is powered on.
**/
void loop() {
//	ax.scanKeyboard();
}