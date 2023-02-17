#ifndef AXIAL_KEYBOARD_H
#define AXIAL_KEYBOARD_H

#include <Arduino.h>
#include <vector>

#include "TeensyPin.h"
#include "AxialKeymap.h"

class AxialKeyboard { // NOLINT(cppcoreguidelines-pro-type-member-init)

private:

	AxialKeymap keymap;

	static const int DMX_SETTLE_TIME_MICROS = 40;
	InputPin pinKbdInhibit; // Grounding this pin prevents keystrokes to be sent over USB

	OutputPin pinG2Afnc; // DMX G2A fcn (r17) // NOTE: Moved from damaged Teensy pin
	OutputPin pinG2Ak2j; // DMX G2A k2j (r19) // NOTE: Moved from damaged Teensy pin
	OutputPin pinG2Akbd; // DMX G2A kbd (r21)
	OutputPin pinG2Alks; // DMX G2A lks (r13)
//  ^-- Note: Only one of these should be LOW at any given time
	std::vector<OutputPin> pinG2APins;
	OutputPin pinG2Abulb;  // DMX G2A bulbs (r12), this is output not input, so not scanned

	static const int DMX_SEL_MAX = 8;
	OutputPin pinSelA;  // DMX Select A (r23)
	OutputPin pinSelB;  // DMX Select B (r25)
	OutputPin pinSelC;  // DMX Select C (r27)

	InputPin pinRet1;  // Return 1 (r1)
	InputPin pinRet2;  // Return 2 (r3)
	InputPin pinRet3;  // Return 3 (r5)
	InputPin pinRet4;  // Return 4 (r7)
	InputPin pinRet5;  // Return 5 (r9)
	InputPin pinRet6;  // Return 6 (r11)
	InputPin pinRet7;  // Return 7 (r13)
	InputPin pinRet8;  // Return 8 (r15)
	std::vector<InputPin> pinRetPins;

// The layout next to the jog wheel is:
//  [jog]  [var]    [shutl]
// [play] [abslt] [swr fader]
	OutputPin pinLedJog; // JOG LED (r4)
	OutputPin pinLedVar; // VAR LED (r2)
	OutputPin pinLedSht; // SHUTL LED (r33)
	OutputPin pinLedAbs; // ABSLT LED (r31)
	OutputPin pinLedSwr; // SWR-FADER LED (r29)

	void scanReturnLine(int g2a, int selA, int selB, int selC);
	void scanG2ASelect(int g2a, int sel);
	void scanG2A(int g2a);

public:
	AxialKeyboard()= default;
	/**
	 * Initialises the instance (passive constructor).
	**/
	void init();
	void scanKeyboard();
};

#endif