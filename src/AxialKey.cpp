#include <Arduino.h>

#include "AxialKey.h"

AxialKey AxialKey::UNDEFINED() {
	return AxialKey(); // NOLINT(modernize-return-braced-init-list)
}

AxialKey::AxialKey() {
	usbKeyCode = 0; // @see https://www.pjrc.com/teensy/td_keyboard.html
	label = String("☹");
	pressed = false;
	changedSinceLastQuery = false;
}

int AxialKey::getUsbKeyCode() const {
	return usbKeyCode;
}

bool AxialKey::isStandardUsbKey() const {
	return usbKeyCode < 99000;
}

const char *AxialKey::getLabel() {
	return label.c_str();
}

bool AxialKey::isPressed() const {
	return pressed;
}

bool AxialKey::setPressed(bool newState) {
	if (pressed != newState) {
		changedSinceLastQuery = true;
		pressed = newState;
	} else {
		changedSinceLastQuery = false;
	}
	return changedSinceLastQuery;
}

bool AxialKey::changed() {
	bool output = changedSinceLastQuery;
	changedSinceLastQuery = false;
	return output;
}