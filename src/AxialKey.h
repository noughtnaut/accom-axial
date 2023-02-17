#ifndef AXIAL_KEY_H
#define AXIAL_KEY_H

#include <Arduino.h>

#include "TeensyPin.h"

class AxialKey {

private:

	int usbKeyCode; // @see https://www.pjrc.com/teensy/td_keyboard.html
	String label;
	bool pressed;
	bool changedSinceLastQuery;

public:

	static AxialKey UNDEFINED();
	AxialKey();

	// FIXME Why does moving this incur the "Redefinition of 'AxialKey'" error?
	explicit AxialKey(int keyCode) : AxialKey() {
		usbKeyCode = keyCode;
	}

	// FIXME Why does moving this incur the "Redefinition of 'AxialKey'" error?
	AxialKey(int keyCode, const String &str) : AxialKey() {
		// TODO: Check if this keycode would be a duplicate
		usbKeyCode = keyCode; // Must NOT be a standard USB keycode
		label = str;
	}

	int getUsbKeyCode() const;
	bool isStandardUsbKey() const;
	const char *getLabel();
	bool isPressed() const;
	bool setPressed(bool newState);
	/**
	 * Reports TRUE only on the first call after last state change
	**/
	bool changed();
};

const AxialKey KEY_UNDEFINED = AxialKey();


#endif