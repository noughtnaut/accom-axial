#include <Arduino.h>
#include <map>

#include "Axial.h"

void Axial::init() {
	init_keyboard();

	Serial.println("Accom Axial is ready");
	Serial.println("        0123 4567");
}

void Axial::init_keyboard() {
	k.init();
}

void Axial::scanKeyboard() {
	k.scanKeyboard();
}