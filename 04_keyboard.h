#ifndef ACCOM_KEYBOARD
#define ACCOM_KEYBOARD

#include <Arduino.h>

#include "01_pin.h"
#include "03_keymap.h"

static const int DMX_SETTLE_TIME_MICROS = 40;
Keymap keymap;

Pin pinKbdInhibit(17, INPUT_PULLUP, LOW, false); // Grounding this pin prevents keystrokes to be sent over USB

// The layout next to the jog wheel is:
//  [jog]  [var]    [shutl]
// [play] [abslt] [swr fader]
Pin pinLedJog(28, OUTPUT, LOW, false);  // JOG LED (r4)
Pin pinLedVar(29, OUTPUT, LOW, false);  // VAR LED (r2)
Pin pinLedSht(32, OUTPUT, LOW, false);  // SHUTL LED (r33)
Pin pinLedAbs(31, OUTPUT, LOW, false);  // ABSLT LED (r31)
Pin pinLedSwr(30, OUTPUT, LOW, false);  // SWR-FADER LED (r29)

static const int NUM_DMX = 4;
Pin pinG2Afnc(23, OUTPUT, LOW, false);  // DMX G2A fcn (r17) // NOTE: Moved from damaged Teensy pin
Pin pinG2Ak2j(22, OUTPUT, LOW, false);  // DMX G2A k2j (r19) // NOTE: Moved from damaged Teensy pin
Pin pinG2Akbd(40, OUTPUT, LOW, false);  // DMX G2A kbd (r21)
Pin pinG2Alks(15, OUTPUT, LOW, false);  // DMX G2A lks (r13)
// Note: we don't scan the LKS DMX on pin 16 here; it's for controlling the bulbs
Pin pinG2APins[NUM_DMX] = { pinG2Afnc, pinG2Akbd, pinG2Ak2j, pinG2Alks };
static const int G2A_LKS_INDEX = 3; // Note: pinG2Alks must be last in this array because it is scanned differently

static const int DMX_SEL_MAX = 8;
Pin pinSelA = Pin(51, OUTPUT, HIGH, false);  // DMX Select A (r23)
Pin pinSelB = Pin(52, OUTPUT, HIGH, false);  // DMX Select B (r25)
Pin pinSelC = Pin(53, OUTPUT, HIGH, false);  // DMX Select C (r27)

static const int NUM_RETURN_LINES = 8;
Pin pinRet1 = Pin(27, INPUT_PULLUP, LOW, false);  // Return 1 (r1)
Pin pinRet2 = Pin(39, INPUT_PULLUP, LOW, false);  // Return 2 (r3)
Pin pinRet3 = Pin(38, INPUT_PULLUP, LOW, false);  // Return 3 (r5)
Pin pinRet4 = Pin(37, INPUT_PULLUP, LOW, false);  // Return 4 (r7)
Pin pinRet5 = Pin(36, INPUT_PULLUP, LOW, false);  // Return 5 (r9)
Pin pinRet6 = Pin(35, INPUT_PULLUP, LOW, false);  // Return 6 (r11)
Pin pinRet7 = Pin(34, INPUT_PULLUP, LOW, false);  // Return 7 (r13)
Pin pinRet8 = Pin(33, INPUT_PULLUP, LOW, false);  // Return 8 (r15)
Pin pinRetPins[NUM_RETURN_LINES] = { pinRet1, pinRet2, pinRet3, pinRet4, pinRet5, pinRet6, pinRet7, pinRet8 };

void scanReturnLine(int g2a, int selA, int selB, int selC) {
  // Scan each return line
  for (int r = 0; r < NUM_RETURN_LINES; r++) {
    Key& key = keymap.get(g2a, selA, selB, selC, r);
    key.setPressed(pinRetPins[r].isActive());
    if (key.changed()) {
      if (pinKbdInhibit.isActive()) {
        Serial.printf("Suppressed keystroke: g2a:abc->r = %i:%i%i%i->%i was %s\n", g2a, selA, selB, selC, r, key.isPressed()?"pressed":"released");
        return;
      }
      if (key.isStandardUsbKey()) {
        // Send key events to USB host instead of simply reporting press/release events
        if (key.isPressed()) {
          Keyboard.press(key.getUsbKeyCode());
          Serial.printf("USB keycode %i was pressed\n", key.getUsbKeyCode());
        } else { // released
          Keyboard.release(key.getUsbKeyCode());
          Serial.printf("USB keycode %i was released\n", key.getUsbKeyCode());
        }
      } else { // nonstandard key
        // Report press/release events for nonstandard keys
        Serial.printf("%s was %s\n", key.getLabel(), key.isPressed() ? "pressed" : "released");
        if (key.isPressed()) {
          Keyboard.printf("%s", key.getLabel());
          // FIXME (if possible??) "ABSLT" prints "AXONY" because console sends in DK layout
          switch(key.getUsbKeyCode()) {
            case 99989: // Send Windows-E
              Serial.printf("Special key: %s\n", key.getLabel());
              Keyboard.set_modifier(MODIFIERKEY_GUI);
              Keyboard.press(KEY_D); // 'D' on the Teensy (using QwertyDK) becomes 'E' on my laptop (using DvorakNO)
              Keyboard.release(KEY_D); // FIXME Avoid being affected by keyboard mapping
              Keyboard.release(MODIFIERKEY_GUI);
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

void scanG2ASelect(int g2a, int sel) {
  // Decode binary bits
  bool selA = sel & 4; // MSB
  bool selB = sel & 2;
  bool selC = sel & 1; // LSB
  pinSelA.setActive(selA);
  pinSelB.setActive(selB);
  pinSelC.setActive(selC);
  delayMicroseconds(DMX_SETTLE_TIME_MICROS); // Allow time for DMX to settle
  pinLedJog.setActive(selA); // MSB
  pinLedVar.setActive(selB);
  pinLedSht.setActive(selC); // LSB
//  delay(500);

  scanReturnLine(g2a, selA, selB, selC);
}

void scanG2A(int g2a) {
  Pin pinG2A = pinG2APins[g2a];
  pinG2A.setActive(true);
  // iterate over DMX SEL [a..c] as binary combinations
  for (int sel = 0; sel <= DMX_SEL_MAX-1; sel++) {
    scanG2ASelect(g2a, sel);
  }
  pinG2A.setActive(false);
}

void scanKeyboard() {

  // Debug: Keep an LED on while scanning, to identify when the loop stops running
  pinLedAbs.setActive(true);

  // Iterate over DMX G2A
  // Only one of these should be LOW at any given time
  for (int g2a = 0; g2a < NUM_DMX-1; g2a++) { // `NUM_DMX-1`: skip pinG2Alks because it uses a reduced address space
    scanG2A(g2a);
  }
  pinLedAbs.setActive(false); // Debug
//  pinLedSwr.setActive(true); // Debug
//  scanG2A(NUM_DMX);
  // Scan pinG2Alks using a reduced address space
//  pinG2Alks.setActive(true);
//  scanG2ASelect(G2A_LKS_INDEX, 0b001); // LKS 1-8
//  scanG2ASelect(G2A_LKS_INDEX, 0b010); // LKS 9-12, LKS VFD 1-4
//  scanG2ASelect(G2A_LKS_INDEX, 0b000); // LKS VFD 5- (still wastes 6/8 iterations but eh)
//  pinG2Alks.setActive(false);
//  pinLedSwr.setActive(false); // Debug: Turn this LED aff after each scan
}

void setupKeyboard() {
  Serial.print("keyboard:");
  keymap = Keymap();
  //threads.addThread(scanKeyboard); // FIXME Why does this seem to randomly just stop?
  if (pinKbdInhibit.isActive()) {
    Serial.print("(suppressed)");
  }
  Serial.println("ok");
}

#endif