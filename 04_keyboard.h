#ifndef ACCOM_KEYBOARD
#define ACCOM_KEYBOARD

#include <Arduino.h>
// Other includes must be listed in the main INO file

#include "03_keymap.h"

Keymap keymap;
const int DMX_SETTLE_TIME_MICROS = 40;

// The layout next to the jog wheel is:
//  [jog]  [var]    [shutl]
// [play] [abslt] [swr fader]
Pin pinLedJog(28, OUTPUT, LOW, HIGH);  // JOG LED (r4)
Pin pinLedVar(29, OUTPUT, LOW, HIGH);  // VAR LED (r2)
Pin pinLedSht(32, OUTPUT, LOW, HIGH);  // SHUTL LED (r33)
Pin pinLedAbs(31, OUTPUT, LOW, HIGH);  // ABSLT LED (r31)
Pin pinLedSwr(30, OUTPUT, LOW, HIGH);  // SWR-FADER LED (r29)

Pin pinG2Afnc(40, OUTPUT, LOW, HIGH);  // DMX G2A fcn (r17)
Pin pinG2Akbd(41, OUTPUT, LOW, HIGH);  // DMX G2A kbd (r21)
Pin pinG2Ak2j(42, OUTPUT, LOW, HIGH);  // DMX G2A k2j (r19)

Pin pinSelA(53, OUTPUT, HIGH, LOW);  // DMX Select A (r23)
Pin pinSelB(52, OUTPUT, HIGH, LOW);  // DMX Select B (r25)
Pin pinSelC(51, OUTPUT, HIGH, LOW);  // DMX Select C (r27)

Pin pinRet1(33, INPUT_PULLUP, LOW, HIGH);  // Return 1 (r1)
Pin pinRet2(34, INPUT_PULLUP, LOW, HIGH);  // Return 2 (r3)
Pin pinRet3(35, INPUT_PULLUP, LOW, HIGH);  // Return 3 (r5)
Pin pinRet4(36, INPUT_PULLUP, LOW, HIGH);  // Return 4 (r7)
Pin pinRet5(37, INPUT_PULLUP, LOW, HIGH);  // Return 5 (r9)
Pin pinRet6(38, INPUT_PULLUP, LOW, HIGH);  // Return 6 (r11)
Pin pinRet7(39, INPUT_PULLUP, LOW, HIGH);  // Return 7 (r13)
Pin pinRet8(27, INPUT_PULLUP, LOW, HIGH);  // Return 8 (r15)

Pin pinG2APins[] = { pinG2Afnc, pinG2Ak2j, pinG2Akbd };
Pin pinRetPins[] = { pinRet1, pinRet2, pinRet3, pinRet4, pinRet5, pinRet6, pinRet7, pinRet8 };

void scanKeyboard() {
  pinLedJog.setActive(true); // Debug: Keep this LED on while scanning
  // iterate over DMX G2A
  // Only one of these should be LOW at any given time
  for (int g = 0; g <= 2; g++) {
    Pin pinG2A = pinG2APins[g];
    pinG2A.setActive(true);
    // iterate over DMX SEL [a..c] as binary combinations
    for (int s = 0; s <= 7; s++) {
      // Decode binary bits
      bool sA = s & 4;
      bool sB = s & 2;
      bool sC = s & 1;
      pinSelA.setActive(sA);
      pinSelB.setActive(sB);
      pinSelC.setActive(sC);
      delayMicroseconds(DMX_SETTLE_TIME_MICROS); // Allow time for DMX to settle
      // Scan each return line
      for (int r = 0; r <= 7; r++) {
        Key& key = keymap.get(g, sA, sB, sC, r);
        key.setPressed(pinRetPins[r].isActive());
        if (key.changed()) {
          if (key.isStandardUsbKey()) {
            // Send key events to USB host instead of simply reporting press/release events
            if (key.isPressed()) {
              Keyboard.press(key.getUsbKeyCode());
              Serial.printf("%i was pressed\n", key.getUsbKeyCode());
            } else { // released
              Keyboard.release(key.getUsbKeyCode());
              Serial.printf("%i was released\n", key.getUsbKeyCode());
            }
          } else { // nonstandard key
            // Report press/release events for nonstandard keys
            Serial.printf("%s was %s\n", key.getLabel(), key.isPressed() ? "pressed" : "released");
            if (key.isPressed()) {
              Keyboard.printf("%s", key.getLabel());
              // FIXME (if possible??) "ABSLT" prints "AXONY" because console sends in DK layout
            } else { // released
              // No output when releasing nonstandard key
            }
          }
        }
      }
    }
    pinG2A.setActive(false);
  }
  pinLedJog.setActive(false); // Debug: Turn this LED aff after scanning
}

int setupKeyboard() {
  Serial.print("keyboard:");
  keymap = Keymap();
  //threads.addThread(scanKeyboard); // FIXME Why does this seem to randomly just stop?
  Serial.println("ok");
  return 0;
}

#endif