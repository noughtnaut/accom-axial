#ifndef AXIAL_H
#define AXIAL_H

#include <Arduino.h>

#include "AxialKeyboard.h"

/**
 * This represents the Accom Axial console including its components.
**/
class Axial {
private:
	AxialKeyboard k;
	void init_keyboard();
public:
	/**
	 * Initialises the instance (passive constructor).
	**/
	void init();
	void scanKeyboard();
};

#endif