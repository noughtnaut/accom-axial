//#include "../lib/HID/src/HID.h"
//#include "../lib/Keyboard/src/Keyboard.h"
#include <Arduino.h>
//#include <Keyboard.h>

#include "AxialKeyboard.h"

void AxialKeyboard::init() {
//	Serial.println("Starting keyboard ...");
//FIXME
//	Keyboard.begin();
	keymap.init();

//	Serial.println("Instantiating output pins ...");
	pinKbdInhibit = InputPin(17, LOW); // Grounding this pin prevents keystrokes to be sent over USB

// The layout next to the jog wheel is:
//  [jog]  [var]    [shutl]
// [play] [abslt] [swr fader]
	pinLedJog = OutputPin(28, LOW); // JOG LED (r4)
	pinLedVar = OutputPin(29, LOW); // VAR LED (r2)
	pinLedSht = OutputPin(32, LOW); // SHUTL LED (r33)
	pinLedAbs = OutputPin(31, LOW); // ABSLT LED (r31)
	pinLedSwr = OutputPin(30, LOW); // SWR-FADER LED (r29)

	pinG2Afnc = OutputPin(23, LOW); // DMX G2A fcn (r17) // NOTE: Moved from damaged Teensy pin
	pinG2Ak2j = OutputPin(22, LOW); // DMX G2A k2j (r19) // NOTE: Moved from damaged Teensy pin
	pinG2Akbd = OutputPin(40, LOW); // DMX G2A kbd (r21)
	pinG2Alks = OutputPin(15, LOW); // DMX G2A lks (r13)
//  Note: ^-- Only one of these should be LOW at any given time
	pinG2APins.clear();
	pinG2APins.emplace_back(pinG2Afnc);
	pinG2APins.emplace_back(pinG2Akbd);
	pinG2APins.emplace_back(pinG2Ak2j);
	// We exclude `pinG2alks` because it is scanned differently
	pinG2APins.shrink_to_fit(); // Make `.size()` report accurately

	pinG2Abulb = OutputPin(16, LOW); // DMX G2A bulbs (r12), this is output not input, so not scanned

	pinSelA = OutputPin(51, HIGH); // DMX Select A (r23)
	pinSelB = OutputPin(52, HIGH); // DMX Select B (r25)
	pinSelC = OutputPin(53, HIGH); // DMX Select C (r27)

//	Serial.println("Instantiating input pins ...");
	// FIXME Are the return lines pull-up or something else?
	// TODO My suspicion is that the kbd lines are active-low, but the lks are active-high :-/
	pinRet1 = InputPin(27, LOW); // Return 1 (r1)
	pinRet2 = InputPin(39, LOW); // Return 2 (r3)
	pinRet3 = InputPin(38, LOW); // Return 3 (r5)
	pinRet4 = InputPin(37, LOW); // Return 4 (r7)
	pinRet5 = InputPin(36, LOW); // Return 5 (r9)
	pinRet6 = InputPin(35, LOW); // Return 6 (r11)
	pinRet7 = InputPin(34, LOW); // Return 7 (r13)
	pinRet8 = InputPin(33, LOW); // Return 8 (r15)
	pinRetPins.clear();
	pinRetPins.emplace_back(pinRet1);
	pinRetPins.emplace_back(pinRet2);
	pinRetPins.emplace_back(pinRet3);
	pinRetPins.emplace_back(pinRet4);
	pinRetPins.emplace_back(pinRet5);
	pinRetPins.emplace_back(pinRet6);
	pinRetPins.emplace_back(pinRet7);
	pinRetPins.emplace_back(pinRet8);
	pinRetPins.shrink_to_fit(); // Make `.size()` report accurately

//	Serial.println("Checking keyboard inhibit  ...");
	//threads.addThread(scanKeyboard); // FIXME Why does this seem to randomly just stop?
	if (pinKbdInhibit.isActive()) {
		Serial.println("(Keyboard input will be suppressed)");
	}

//	Serial.println("Keyboard is ready");
}

void AxialKeyboard::scanReturnLine(int g2a, int selA, int selB, int selC) {
	// Scan each return line
	for(int r = 0; r != (int)pinRetPins.size(); r++) {
		AxialKey &key = keymap.get(g2a, selA, selB, selC, r);
		key.setPressed(pinRetPins.at(r).isActive());
		if (key.changed()) {
			if (pinKbdInhibit.isActive()) {
				Serial.printf("Suppressed keystroke: g2a:abc->r = %i:%i%i%i->%i was %s\n", g2a, selA, selB, selC, r,
							  key.isPressed() ? "pressed" : "released");
				return;
			}
			if (key.isStandardUsbKey()) {
				// Send key events to USB host instead of simply reporting press/release events
				if (key.isPressed()) {
//FIXME					Keyboard.press(key.getUsbKeyCode());
					Serial.printf("USB keycode %i was pressed\n", key.getUsbKeyCode());
				} else { // released
//FIXME					Keyboard.release(key.getUsbKeyCode());
					Serial.printf("USB keycode %i was released\n", key.getUsbKeyCode());
				}
			} else { // nonstandard key
				// Report press/release events for nonstandard keys
				Serial.printf("%s was %s\n", key.getLabel(), key.isPressed() ? "pressed" : "released");
				if (key.isPressed()) {
//FIXME					Keyboard.printf("%s", key.getLabel());
					// FIXME (if possible??) "ABSLT" prints "AXONY" because console sends in DK layout
					switch (key.getUsbKeyCode()) {
						case 99989: // Send Windows-E
							Serial.printf("Special key: %s\n", key.getLabel());
//FIXME							Keyboard.set_modifier(MODIFIERKEY_GUI);
//FIXME							Keyboard.press(KEY_D); // 'D' on the Teensy (using QwertyDK) becomes 'E' on my laptop (using DvorakNO)
//FIXME							Keyboard.release(KEY_D); // FIXME Avoid being affected by keyboard mapping
//FIXME							Keyboard.release(MODIFIERKEY_GUI);
							break;
						default:
							Serial.printf("(Unmapped special key: %s)\n", key.getLabel());
					}
				} else { // released
					// No output when sustaining or releasing nonstandard key
				}
			}
		}
	}
}

void AxialKeyboard::scanG2ASelect(int g2a, int sel) {
	// Decode binary bits
	bool selA = sel & 4; // MSB
	bool selB = sel & 2;
	bool selC = sel & 1; // LSB
	pinSelA.setActive(selA);
	pinSelB.setActive(selB);
	pinSelC.setActive(selC);
	delayMicroseconds(DMX_SETTLE_TIME_MICROS); // Allow time for DMX to settle
	pinLedJog.setActive(selA); // Debug, MSB
	pinLedVar.setActive(selB); // Debug
	pinLedSht.setActive(selC); // Debug, LSB
//  delay(500);

	scanReturnLine(g2a, selA, selB, selC);
}

void AxialKeyboard::scanG2A(int g2a) {
	OutputPin pinG2A = pinG2APins[g2a];
	pinG2A.on();
	// iterate over DMX SEL [a..c] as binary combinations
	for (int sel = 0; sel <= DMX_SEL_MAX; sel++) {

		scanG2ASelect(g2a, sel);
	}
	pinG2A.off();
}

void AxialKeyboard::scanKeyboard() {
	// Debug: Use keyboard LEDs to indicate scan state

	// Iterate over DMX G2A
	pinLedAbs.on(); // Debug
	// Only one of these should be LOW at any given time
	for (int g2a = 0;
		 g2a < AxialKeymap::NUM_DMX - 1; // `-1`: We exclude `pinG2alks` because it is scanned differently
		 g2a++) {
		scanG2A(g2a);
	}
	pinLedAbs.off(); // Debug

	// Scan pinG2Alks using a reduced address space
	pinLedSwr.on(); // Debug
	pinG2Alks.on();
//  scanG2ASelect(DMX_LKS_INDEX, 0b100); // LKS 1-8
//  scanG2ASelect(DMX_LKS_INDEX, 0b010); // LKS 9-12, LKS VFD 1-4
//  scanG2ASelect(DMX_LKS_INDEX, 0b000); // LKS VFD 5- (still wastes 6/8 iterations but eh)
	pinG2Alks.off();
	pinLedSwr.off(); // Debug
}