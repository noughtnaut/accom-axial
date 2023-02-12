#ifndef AXIAL_H
#define AXIAL_H

#include <Arduino.h>

#include "AxialKeyboard.h"
#include "AxialDisplay.h"

/**
 * This represents the Accom Axial console including its components.
**/
class Axial {
private:
	AxialDisplay d;
	AxialKeyboard k;
	void init_display();
	void init_keyboard();
public:
	/**
	 * Initialises the instance (passive constructor).
	**/
	void init();
	void showoff();
};

#endif