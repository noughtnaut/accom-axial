#ifndef ACCOM_KEYBOARD
#define ACCOM_KEYBOARD

#include <Arduino.h>
#include <Keyboard.h>
#include "Keyboard.h"

#include "01_pin.h"
#include "03_keymap.h"

static const uint32_t DMX_SETTLE_TIME_MICROS = 40;
Keymap keymap;

// Pin pinKbdInhibit(17, INPUT_PULLUP, LOW, false); // Grounding this pin prevents keystrokes to be sent over USB
Pin pinKbdInhibit(17, INPUT_PULLUP, HIGH, false);

// The layout next to the jog wheel is:
//  [jog]  [var]    [shutl]
// [play] [abslt] [swr fader]
Pin pinLedJog = Pin(28, OUTPUT, LOW, false); // JOG LED (r4)
Pin pinLedVar = Pin(29, OUTPUT, LOW, false); // VAR LED (r2)
Pin pinLedSht = Pin(32, OUTPUT, LOW, false); // SHUTL LED (r33)
Pin pinLedAbs = Pin(31, OUTPUT, LOW, false); // ABSLT LED (r31)
Pin pinLedSwr = Pin(30, OUTPUT, LOW, false); // SWR-FADER LED (r29)

static const int NUM_DMX = 4;
Pin pinG2Afnc = Pin(23, OUTPUT, LOW, false); // DMX G2A fcn (r17) // NOTE: Moved from damaged Teensy pin
Pin pinG2Ak2j = Pin(22, OUTPUT, LOW, false); // DMX G2A k2j (r19) // NOTE: Moved from damaged Teensy pin
Pin pinG2Akbd = Pin(21, OUTPUT, LOW, false); // DMX G2A kbd (r21) // NOTE: Moved from damaged Teensy pin
Pin pinG2Alks = Pin(15, OUTPUT, LOW, false); // DMX G2A lks (r13)
Pin pinG2Ablb = Pin(16, OUTPUT, LOW, false); // DMX G2A bulbs (r12)
// Note: we don't scan the LKS DMX on pin 16 here (that's for controlling the bulbs) but we still need to set it HIGH to disable it
static const int G2A_FNC_INDEX = 0;
static const int G2A_KBD_INDEX = 1;
static const int G2A_K2J_INDEX = 2;
static const int G2A_LKS_INDEX = 3;
Pin pinG2APins[NUM_DMX] = { pinG2Afnc, pinG2Akbd, pinG2Ak2j, pinG2Alks }; // Note: pinG2Alks must be last in this array because it is scanned differently

static const int DMX_SEL_MAX = 8;
static const int DMX_SEL_MAX_LKS = 3;
Pin pinSelA = Pin(50, OUTPUT, HIGH, false); // DMX Select A (r23) // NOTE: Moved from damaged Teensy pin 51
Pin pinSelB = Pin(52, OUTPUT, HIGH, false); // DMX Select B (r25)
Pin pinSelC = Pin(53, OUTPUT, HIGH, false); // DMX Select C (r27)

static const int NUM_RETURN_LINES = 8;
// FIXME TODO should these be INPUT (without pullup) in order to support kbd+vfd? Must test!
Pin pinRet1 = Pin(27, INPUT_PULLUP, LOW, false); // Return 1 (r1)
Pin pinRet2 = Pin(39, INPUT_PULLUP, LOW, false); // Return 2 (r3)
Pin pinRet3 = Pin(38, INPUT_PULLUP, LOW, false); // Return 3 (r5)
Pin pinRet4 = Pin(37, INPUT_PULLUP, LOW, false); // Return 4 (r7)
Pin pinRet5 = Pin(36, INPUT_PULLUP, LOW, false); // Return 5 (r9)
Pin pinRet6 = Pin(35, INPUT_PULLUP, LOW, false); // Return 6 (r11)
Pin pinRet7 = Pin(34, INPUT_PULLUP, LOW, false); // Return 7 (r13)
Pin pinRet8 = Pin(33, INPUT_PULLUP, LOW, false); // Return 8 (r15)
Pin pinRetPins[NUM_RETURN_LINES] = { pinRet1, pinRet2, pinRet3, pinRet4, pinRet5, pinRet6, pinRet7, pinRet8 };

void interpretStandardUsbKeyEvent(Key& key) {
  // Report press/release events for standard USB keyboard keys
  Serial.printf(" - Standard USB Key #%i '%s'", key.getKeyCode(), key.getLabel());
  if (pinKbdInhibit.isActive()) {
    Serial.printf(" suppressed");
    return;
  }
  if (key.isPressed()) {
    Keyboard.press(key.getKeyCode());
  } else { // released
    Keyboard.release(key.getKeyCode());
  }
}

bool macroKey(Key& key) {
  switch(key.getKeyCode()) {
    case 99989: // FILE MNGR: Send Windows-E to launch file manager
      Serial.printf("Special key: %s\n", key.getLabel());
      Keyboard.set_modifier(MODIFIERKEY_GUI);
      Keyboard.press(KEY_D);   // 'D' on the Teensy (using QwertyDK) becomes 'E' on my laptop (using DvorakNO)
      Keyboard.release(KEY_D); // FIXME Avoid being affected by keyboard mapping
      Keyboard.release(MODIFIERKEY_GUI);
    case 99968: // FREEZ/UNFRZ: Send Windows-L to lock screen
      Serial.printf("Special key: %s\n", key.getLabel());
      Keyboard.set_modifier(MODIFIERKEY_GUI);
      Keyboard.press(KEY_P);   // 'P' on the Teensy (using QwertyDK) becomes 'L' on my laptop (using DvorakNO)
      Keyboard.release(KEY_P); // FIXME Avoid being affected by keyboard mapping
      Keyboard.release(MODIFIERKEY_GUI);

    // TODO: More macro keys
    // TODO: Not hard-coded but managed via UI
    //- trigger VFD menu by [LEARN MACRO]
    //- capture keystrokes
    //- name and save
    //- map to (any) key

      break;
    default:
      return false;
  }
  return true;
}

void interpretAccomKeyEvent(Key& key) {
  // Report press/release events for nonstandard keys
  Serial.printf(" - Accom Key #%i '%s'", key.getKeyCode(), key.getLabel());
  if (key.isPressed()) {
// FIXME (if possible??) "ABSLT" prints "AXONY" because console sends in DK layout
//    if (macroKey(key)) { // Macro keys handle their own actions
//      return;
//    } else {
//      // TODO
//    }
  } else { // released
    // No output when sustaining or releasing nonstandard key
  }
}

void interpretKeyEvent(Key& key) {
  if (key.isStandardUsbKey()) {
    interpretStandardUsbKeyEvent(key);
  } else {
    interpretAccomKeyEvent(key);
  }
}

void scanReturnLine(int g2a, bool selA, bool selB, bool selC) {
  delayMicroseconds(DMX_SETTLE_TIME_MICROS); // Allow time for DMX to settle
  // Scan each return line
  for (int r = 0; r < NUM_RETURN_LINES; r++) {
    //Serial.printf("g2a:abc->r = %s:%i%i%i->%i\n", g2a==0?"fnc":g2a==1?"kbd":g2a==2?"k2j":g2a==3?"lks":"???", selA, selB, selC, r);
    Key& key = keymap.get(g2a, selA, selB, selC, r);
    key.setPressed(pinRetPins[r].isActive());
    if (key.changed()) {
      Serial.printf("Keystroke: g2a:abc->r = %s:%i%i%i->%i was %s",
        g2a==0?"fnc":g2a==1?"kbd":g2a==2?"k2j":g2a==3?"lks":"???", selA, selB, selC, r, key.isPressed()?"pressed":"released"
      );
      interpretKeyEvent(key);
      Serial.printf("\n");
    }
  }
}

void cycleSelect(int g2a, bool selA, bool selB, bool selC) {
  pinLedJog.setActive(selA); // Debug
  pinLedVar.setActive(selB); // Debug
  pinLedSht.setActive(selC); // Debug

  scanReturnLine(g2a, selA, selB, selC);
//  delay(150); // Debug: Makes scanning super slow, but you can follow the LEDs
//  if (g2a == G2A_LKS_INDEX) {
//    delay(1600);
//  }

  pinLedJog.setActive(false); // Debug
  pinLedVar.setActive(false); // Debug
  pinLedSht.setActive(false); // Debug
}

void cycleSelect(int g2a) {
  pinLedAbs.setActive((g2a & 2)); // Debug
  pinLedSwr.setActive((g2a & 1)); // Debug
//  if (g2a != G2A_LKS_INDEX) { // Iterate over DMX SEL [a..c] as binary combinations
    for (int sel = 0; sel < DMX_SEL_MAX; sel++) {
      // Decode binary bits
      bool selA = sel & 4; // MSB
      bool selB = sel & 2;
      bool selC = sel & 1; // LSB
      pinSelA.setActive(selA);
      pinSelB.setActive(selB);
      pinSelC.setActive(selC);
      cycleSelect(g2a, selA, selB, selC);
      pinSelA.setActive(false);
      pinSelB.setActive(false);
      pinSelC.setActive(false);
    }
//  } else { // Iterate over LKS DMX in a different way because only 3/8th are used
//    for (int sel = 0; sel < DMX_SEL_MAX_LKS; sel++) {
//      // Decode binary bits
//      bool selA = sel & 4; // MSB
//      bool selB = sel & 2;
//      bool selC = sel & 1; // LSB
//      cycleSelect(g2a, selA, selB, selC);
//    }
//  }
  pinLedAbs.setActive(false); // Debug
  pinLedSwr.setActive(false); // Debug
}

void cycleG2A() {
  // Iterate over DMX G2A to enable each DMX, by itself, in order
//  for (int g2a = 0; g2a < NUM_DMX; g2a++) { // Both keyboard and LKS
  for (int g2a = 0; g2a < NUM_DMX-1; g2a++) { // Keyboard only, no LKS
//    logger.logln(g2a==0?"fnc":g2a==1?"kbd":g2a==2?"k2j":g2a==3?"lks":"?????");
    pinG2APins[g2a].setActive(true);
    cycleSelect(g2a);
    pinG2APins[g2a].setActive(false);
  }
}

void scanKeyboard() {
  cycleG2A();
// Alternative implementation:
// - Run G2A+SEL cycles in a separate thread, but keep their states in global and atomic variables
// - Set interrupts on the return lines to trigger a decode whenever a signal is received
// - Might need an additional variable to flag "is settled and ready for scanning".
// - Test if it is possible to check the cycle setup at that exact moment
}

void setupKeyboard(Logger logger) {
  logger.begin("init keyboard");
  keymap = Keymap();
  if (pinKbdInhibit.isActive()) {
    logger.logln("suppressing keystrokes");
  }
  logger.end();
}

#endif