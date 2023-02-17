#ifndef AXIAL_KEYMAP_H
#define AXIAL_KEYMAP_H

#include "AxialKey.h"

class AxialKeymap {

public:

	static const int NUM_DMX = 4;
	static const int NUM_DMX_STATES = 2; // It's binary
	static const int NUM_RETURN_LINES = 8;
	AxialKeymap() = default;

	AxialKey &get(int g2a, bool selA, bool selB, bool selC, int rx) {
		//    Serial.printf("keymap[%i][%i][%i][%i][%i]\n", g2a, selA, selB, selC, rx);
		return keymap[g2a][selA][selB][selC][rx];
	}

	/**
	 * Initialises the instance (passive constructor).
	**/
	void init();

private:

	AxialKey keymap
	[NUM_DMX]  // g2a for fn, k1, k2j, lks
	[NUM_DMX_STATES]  // selA
	[NUM_DMX_STATES]  // selB
	[NUM_DMX_STATES]  // selC
	[NUM_RETURN_LINES]  // return line
	;
};

#endif