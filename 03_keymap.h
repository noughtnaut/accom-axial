#include "keylayouts.h"
#ifndef ACCOM_MAP
#define ACCOM_MAP

#include <Arduino.h>

class Key {
private:
  int keyCode = 0;  // @see https://www.pjrc.com/teensy/td_keyboard.html
  String label = String("☹");
  bool pressed = false;
  bool changedSinceLastQuery = false;

public:
  Key() {}

  Key(int stdKeyCode) {
    keyCode = stdKeyCode;
  }

  Key(int customKeyCode, String str) {
    keyCode = customKeyCode;  // Must NOT be a standard USB keycode
    label = str;
  }

  int getKeyCode() {
    return keyCode;
  }

  bool isStandardUsbKey() {
    return keyCode < 99000;
  }

  const char* getLabel() {
    return label.c_str();
  }

  bool isPressed() {
    return pressed;
  }

  void setPressed(bool newState) {
    changedSinceLastQuery = (pressed != newState);
    pressed = newState;
  }

  // Reports TRUE only on the first call after last state change
  bool changed() {
    bool output = changedSinceLastQuery; // C has no `finally{}` feature
    changedSinceLastQuery = false;
    return output;
  }
};

const Key KEY_UNDEFINED = Key();

class Keymap {

public:

  Keymap() {
    setupKeymap();
    // TODO: Load (modified) keymap from storage
  }

  Key& get(int g2a, bool selA, bool selB, bool selC, int rx) {
    //    Serial.printf("keymap[%i][%i][%i][%i][%i]\n", g2a, selA, selB, selC, rx);
    return keymap[g2a][selA][selB][selC][rx];
  }

  void setupKeymap() {
    // Define all keys as "undefined"
    for (int g2a = 0; g2a < NUM_DMX; g2a++) {
      for (int selA = 0; selA < NUM_DMX_STATES; selA++) {
        for (int selB = 0; selB < NUM_DMX_STATES; selB++) {
          for (int selC = 0; selC < NUM_DMX_STATES; selC++) {
            for (int rl = 0; rl < NUM_RETURN_LINES; rl++) {
              keymap[g2a][selA][selB][selC][rl] = KEY_UNDEFINED;
            }
          }
        }
      }
    }
    // Overwrite slots for known keys
    setupDefaultKeymap();
    loadKeymap();
  }

  void loadKeymap() {
    // TODO: read from onboard SD card
    // ask file name (browse via VFD), or load default/last used
  }

  void saveKeymap() {
    // TODO: read from onboard SD card
    // ask file name (browse via VFD, allow overwrite existing)
  }

  void setupDefaultKeymap() {
    // Overwrite slots for known keys
//         ,--------------- g2a for fn, k1, k2j, lks (in that order) enabled/low
//         |  ,------------ selA active/high
//         |  |  ,--------- selB active/high
//         |  |  |  ,------ selC active/high
//         |  |  |  |  ,--- return line active/low
//         v  v  v  v  v
    // G2A #0: function keys (fn)
    keymap[0][0][0][1][0] = Key(MODIFIERKEY_GUI, String("[SYSTM]"));// SYSTM
    keymap[0][0][0][1][1] = Key(99999, String("[HELP]"));          // HELP
    keymap[0][0][0][1][2] = Key(99998, String("[f1c2]"));          // Fn block 1 col 3 row 2
    keymap[0][0][0][1][3] = Key(99997, String("[AUDIO]"));         // AUDIO
    keymap[0][0][0][1][4] = Key(99996, String("[CLEAN ON/OFF]"));  // CLEAN ON/OFF
    keymap[0][0][0][1][5] = Key(99995, String("[FILTR]"));         // FILTR
    keymap[0][0][0][1][6] = Key(99994, String("[RIPPLE MODE]"));   // RIPPLE MODE
    keymap[0][0][0][1][7] = Key(99993, String("[LIST TRACK]"));    // LIST TRACK
    keymap[0][0][1][0][0] = Key(99992, String("[SESSN MNGR]"));    // SESSN MNGR
    keymap[0][0][1][0][1] = Key(99991, String("[ASIGN MNGR]"));    // ASIGN MNGR
    keymap[0][0][1][0][2] = Key(99980, String("[INIT MNGR]"));     // INIT MNGR
    keymap[0][0][1][0][3] = Key(99989, String("[FILE MNGR]"));     // FILE MNGR
    keymap[0][0][1][0][4] = Key(99988, String("[LIST I/O]"));      // LIST I/O
    keymap[0][0][1][0][5] = Key(99987, String("[f2b1]")),          // Fn block 2 col 2 row 1
    keymap[0][0][1][0][6] = Key(99986, String("[f2c1]")),
    keymap[0][0][1][0][7] = Key(99985, String("[f2d1]")),
    keymap[0][0][1][1][0] = Key(99984, String("[LEARN MACRO]"));   // LEARN MACRO
    keymap[0][0][1][1][1] = Key(99983, String("[MACRO 1/4]"));     // MACRO 1/4
    keymap[0][0][1][1][2] = Key(99982, String("[MACRO 2/5]"));     // MACRO 2/5
    keymap[0][0][1][1][3] = Key(99981, String("[MACRO 3/6]"));     // MACRO 3/6
    keymap[0][0][1][1][4] = Key(99980, String("[LIST]"));          // LIST
    keymap[0][0][1][1][5] = Key(99979, String("[SORT]"));          // SORT
    keymap[0][0][1][1][6] = Key(99978, String("[EDIT NMBR]"));     // EDIT NMBR
    keymap[0][0][1][1][7] = Key(99977, String("[TITLE NOTES]"));   // TITLE NOTES
    keymap[0][1][0][1][0] = Key(99976, String("[f3a2]")),
    keymap[0][1][0][1][1] = Key(99975, String("[f3b2]")),
    keymap[0][1][0][1][2] = Key(99974, String("[f3c2]")),
    keymap[0][1][0][1][3] = Key(99973, String("[NEW VERSN]"));     // NEW VERSN
    keymap[0][1][0][1][4] = Key(99972, String("[PRE READ]"));      // PRE READ
    keymap[0][1][0][1][5] = Key(99971, String("[f4b2]")),
    keymap[0][1][0][1][6] = Key(99970, String("[f4c2]")),
    keymap[0][1][0][1][7] = Key(99969, String("[f4d2]")),
    keymap[0][1][1][0][0] = Key(99968, String("[UN/FREEZE]"));     // UNFRZ / FREEZ
    keymap[0][1][1][0][1] = Key(99967, String("[LOOP]"));          // LOOP
    keymap[0][1][1][0][2] = Key(99966, String("[EE ON/OFF]"));     // EE ON/OFF
    keymap[0][1][1][0][3] = Key(KEY_MEDIA_EJECT, String("[EJECT]"));// EJECT
    keymap[0][1][1][0][4] = Key(99965, String("[SWR ON/OFF]"));    // SWR ON/OFF
    keymap[0][1][1][0][5] = Key(99964, String("[MULTI REC]"));     // MULTI REC
    keymap[0][1][1][0][6] = Key(99963, String("[COLOR FRAME]"));   // COLOR FRAME
    keymap[0][1][1][0][7] = Key(99962, String("[SRC ADV]"));       // SRC ADV
    keymap[0][1][1][1][0] = Key(99961, String("[CLEAR MARKS]"));   // CLEAR MARKS
    keymap[0][1][1][1][1] = Key(99960, String("[OPEN END]"));      // OPEN END
    keymap[0][1][1][1][2] = Key(99959, String("[f3c3]")),
    keymap[0][1][1][1][3] = Key(99958, String("[f3d3]")),
    keymap[0][1][1][1][4] = Key(99957, String("[HIST UNDO]"));     // HIST UNDO
    keymap[0][1][1][1][5] = Key(99956, String("[TO]"));            // TO
    keymap[0][1][1][1][6] = Key(99955, String("[DF / NDF]"));      // DF / NDF
    keymap[0][1][1][1][7] = Key(99954, String("[F1 / F2]"));       // F1 / F2
    // G2A #1: 2/3 of the qwerty keyboard (k1)
    keymap[1][0][0][0][0] = Key(KEY_F11, String("[XCHNG -> F11]"));       // XCHNG
    keymap[1][0][0][0][1] = Key(KEY_8, String("[8]"));             // 8
    keymap[1][0][0][0][2] = Key(KEY_U, String("[U]"));             // U
    keymap[1][0][0][0][3] = Key(KEY_J, String("[J]"));             // J
    keymap[1][0][0][0][4] = Key(KEY_M, String("[M]"));             // M
    keymap[1][0][0][0][5] = Key(KEY_SPACE, String("[SPACE]"));     // SPACE
    keymap[1][0][0][0][6] = Key(99952, String("[SET/TRIM OUT]"));  // SET/TRIM OUT
    keymap[1][0][0][0][7] = Key(99951, String("[SET/TRIM DUR]"));  // SET/TRIM DUR
    keymap[1][0][0][1][0] = Key(KEY_F3, String("[CLIP LIBRY -> F3]"));   // CLIP LIBRY
    keymap[1][0][0][1][1] = Key(KEY_4, String("[4]"));             // 4
    keymap[1][0][0][1][2] = Key(KEY_E, String("[E]"));             // E
    keymap[1][0][0][1][3] = Key(KEY_D, String("[D]"));             // D
    keymap[1][0][0][1][4] = Key(KEY_C, String("[C]"));             // C
    keymap[1][0][0][1][5] = Key(KEYPAD_PERIOD, String("[KP .]"));  // KP FR/TC
    keymap[1][0][0][1][6] = Key(KEYPAD_6, String("[KP 6]"));       // KP 6
    keymap[1][0][0][1][7] = Key(KEYPAD_ENTER, String("[KP ENTER]"));// KP ENTER
    keymap[1][0][1][0][0] = Key(KEY_F5, String("[TRANS EDITR -> F5]"));  // TRANS EDITR
    keymap[1][0][1][0][1] = Key(KEY_6, String("[6]"));             // 6
    keymap[1][0][1][0][2] = Key(KEY_T, String("[T]"));             // T
    keymap[1][0][1][0][3] = Key(KEY_G, String("[G]"));             // G
    keymap[1][0][1][0][4] = Key(KEY_B, String("[B]"));             // B
    keymap[1][0][1][0][5] = Key(99948, String("00"));              // KP 00
    keymap[1][0][1][0][6] = Key(KEYPAD_5, String("[KP 5]"));       // KP 5
    keymap[1][0][1][0][7] = Key(KEYPAD_2, String("[KP 2]"));       // KP 2
    keymap[1][0][1][1][0] = Key(KEY_F1, String("[R MARK -> F1]"));       // R MARK
    keymap[1][0][1][1][1] = Key(KEY_2, String("[2]"));             // 2
    keymap[1][0][1][1][2] = Key(KEY_Q, String("[Q]"));             // Q
    keymap[1][0][1][1][3] = Key(KEY_A, String("[A]"));             // A
    keymap[1][0][1][1][4] = Key(KEY_Z, String("[Z]"));             // Z
    keymap[1][0][1][1][5] = Key(MODIFIERKEY_ALT, String("[FWD -> ALT L]")); // FWD
    keymap[1][1][0][0][0] = Key(KEY_F6, String("[RAVE -> F6]"));         // RAVE
    keymap[1][1][0][0][1] = Key(KEY_7, String("[7]"));             // 7
    keymap[1][1][0][0][2] = Key(KEY_Y, String("[Y]"));             // Y
    keymap[1][1][0][0][3] = Key(KEY_H, String("[H]"));             // H
    keymap[1][1][0][0][4] = Key(KEY_N, String("[N]"));             // N
    keymap[1][1][0][0][5] = Key(99945, String("[DFINE CONST]"));   // DFINE CONST
    keymap[1][1][0][0][6] = Key(99944, String("[FIND XFER]"));     // FIND XFER
    keymap[1][1][0][0][7] = Key(KEYPAD_1, String("[KP 1]"));       // KP 1
    keymap[1][1][0][1][0] = Key(KEY_F2, String("[CACHE -> F2]"));        // CACHE
    keymap[1][1][0][1][1] = Key(KEY_3, String("[3]"));             // 3
    keymap[1][1][0][1][2] = Key(KEY_W, String("[W]"));             // W
    keymap[1][1][0][1][3] = Key(KEY_S, String("[S]"));             // S
    keymap[1][1][0][1][4] = Key(KEY_X, String("[X]"));             // X
    keymap[1][1][0][1][6] = Key(KEYPAD_PLUS, String("[KP +]"));    // KP + / YES
    keymap[1][1][0][1][7] = Key(KEYPAD_MINUS, String("[KP -]"));   // KP - / NO
    keymap[1][1][1][0][0] = Key(KEY_F4, String("[GT TEXT -> F4]"));      // GT TEXT
    keymap[1][1][1][0][1] = Key(KEY_5, String("[5]"));             // 5
    keymap[1][1][1][0][2] = Key(KEY_R, String("[R]"));             // R
    keymap[1][1][1][0][3] = Key(KEY_F, String("[F]"));             // F
    keymap[1][1][1][0][4] = Key(KEY_V, String("[V]"));             // V
    keymap[1][1][1][0][5] = Key(KEYPAD_8, String("[KP 8]"));       // KP 8
    keymap[1][1][1][0][6] = Key(KEYPAD_9, String("[KP 9]"));       // KP 9
    keymap[1][1][1][0][7] = Key(KEYPAD_3, String("[KP 3]"));       // KP 3
    keymap[1][1][1][1][0] = Key(KEY_ESC, String("[VIEW -> ESC]"));         // VIEW
    keymap[1][1][1][1][1] = Key(KEY_1, String("[1]"));             // 1
    keymap[1][1][1][1][2] = Key(99939, String("[WKPS/EDL]"));      // WKPS/EDL
    keymap[1][1][1][1][3] = Key(KEY_CAPS_LOCK, String("[CAPS]"));  // CONTROL
    keymap[1][1][1][1][4] = Key(MODIFIERKEY_SHIFT, String("[SHIFT L]"));// SHIFT L
    keymap[1][1][1][1][5] = Key(MODIFIERKEY_CTRL, String("[REV -> CTRL L]"));// REV
    // G2A #2: 1/3 of the qwerty keyboard, and the jog wheel board (k2j)
    keymap[2][0][0][0][0] = Key(KEY_LEFT_ARROW, String("[PREV]")); // PREV
    keymap[2][0][0][0][1] = Key(KEY_HOME, String("[HOME]"));       // START
    keymap[2][0][0][0][2] = Key(KEY_RIGHT_ARROW, String("[NEXT]"));// NEXT
    keymap[2][0][0][0][3] = Key(KEY_PAGE_UP, String("[PG UP]"));   // PREV PAGE
    keymap[2][0][0][0][4] = Key(99936, String("[VFD 2 COMMIT]"));  // VFD 2 COMMIT
    keymap[2][0][0][0][5] = Key(99935, String("[VFD 4 COMMIT]"));  // VFD 4 COMMIT
    keymap[2][0][0][0][6] = Key(99934, String("[VFD 6 COMMIT]"));  // VFD 6 COMMIT
    keymap[2][0][0][0][7] = Key(99933, String("[USER]"));          // USER
    keymap[2][0][0][1][0] = Key(KEY_F9, String("[MENU DATA -> F9]"));    // MENU DATA
    keymap[2][0][0][1][1] = Key(KEY_EQUAL, String("[=]"));         // = / CHNG
    keymap[2][0][0][1][2] = Key(KEY_LEFT_BRACE, String("["));      // [
    keymap[2][0][0][1][3] = Key(KEY_QUOTE, String("[']"));         // SINGLE QUOTE
    keymap[2][0][0][1][4] = Key(MODIFIERKEY_RIGHT_SHIFT, String("[SHIFT R]"));// SHIFT R
    keymap[2][0][0][1][5] = Key(MODIFIERKEY_RIGHT_CTRL, String("[MARK OUT -> CTRL R]"));// MARK OUT
    keymap[2][0][0][1][6] = Key(KEY_ENTER, String("[A/V SWR -> ENTER]"));       // A/V SWR
    keymap[2][0][0][1][7] = Key(KEY_ENTER, String("[ENTER]"));     // ENTER
    keymap[2][0][1][0][0] = Key(99930, String("[REC/SRC]"));       // REC/SRC
    keymap[2][0][1][0][1] = Key(99929, String("[MARK OUT JOG]"));  // MARK OUT JOG
    keymap[2][0][1][0][2] = Key(KEY_MEDIA_PLAY_PAUSE, String("[PLAY]"));          // PLAY
    keymap[2][0][1][0][3] = Key(99927, String("[JOG]"));           // JOG
    keymap[2][0][1][0][4] = Key(99926, String("[VAR]"));           // VAR
    keymap[2][0][1][0][5] = Key(99925, String("[SHUTL]"));         // SHUTL
    keymap[2][0][1][0][6] = Key(99924, String("[ABSLT]"));         // ABSLT
    keymap[2][0][1][0][7] = Key(99993, String("[SWR FADER]"));     // SWR FADER
    keymap[2][0][1][1][0] = Key(KEY_F8, String("[D MEM] -> F8"));        // D MEM
    keymap[2][0][1][1][1] = Key(KEY_0, String("[0]"));             // 0
    keymap[2][0][1][1][2] = Key(KEY_O, String("[O]"));             // O
    keymap[2][0][1][1][3] = Key(KEY_L, String("[L]"));             // L
    keymap[2][0][1][1][4] = Key(KEY_PERIOD, String("[.]"));        // .
    keymap[2][0][1][1][5] = Key(99921, String("[SET/TRIM IN]"));   // SET/TRIM IN
    keymap[2][0][1][1][6] = Key(99920, String("[SET/TRIM RATE]")); // SET/TRIM RATE
    keymap[2][0][1][1][7] = Key(KEY_F12, String("[SLIDE -> F12]"));       // SLIDE
    keymap[2][1][0][0][0] = Key(99907, String("[MARK IN]"));       // MARK IN JOG
    keymap[2][1][0][0][1] = Key(KEY_END, String("[END]"));         // END
    keymap[2][1][0][0][2] = Key(99918, String("[SELECT]"));        // SELECT
    keymap[2][1][0][0][3] = Key(KEY_PAGE_DOWN, String("[PG DN]")); // NEXT PAGE
    keymap[2][1][0][0][4] = Key(99917, String("[VFD 1 COMMIT]"));  // VFD 1 COMMIT
    keymap[2][1][0][0][5] = Key(99916, String("[VFD 3 COMMIT]"));  // VFD 3 COMMIT
    keymap[2][1][0][0][6] = Key(99915, String("[VFD 5 COMMIT]"));  // VFD 5 COMMIT
    keymap[2][1][0][0][7] = Key(99914, String("[STOP]"));          // STOP
    keymap[2][1][0][1][0] = Key(KEY_F9, String("[GPI -> F9]"));          // GPI
    keymap[2][1][0][1][1] = Key(KEY_MINUS, String("[-]"));         // - / COPY
    keymap[2][1][0][1][2] = Key(KEY_P, String("[P]"));             // P
    keymap[2][1][0][1][3] = Key(KEY_SEMICOLON, String("[;]"));     // ;
    keymap[2][1][0][1][4] = Key(KEY_SLASH, String("[?]"));         // ? / CANCL
    keymap[2][1][0][1][5] = Key(99912, String("[MATCH]"));         // MATCH
    keymap[2][1][0][1][6] = Key(KEY_BACKSPACE, String("[BKSP]"));  // BKSP
    keymap[2][1][0][1][7] = Key(99911, String("[ALL]"));           // ALL
    keymap[2][1][1][0][0] = Key(KEY_F10, String("[D CMD -> F10]"));       // D CMD
    keymap[2][1][1][0][1] = Key(KEY_BACKSLASH, String("[\\]"));    // \\ / LAYER
    keymap[2][1][1][0][2] = Key(KEY_RIGHT_BRACE, String("]"));     // ]
    keymap[2][1][1][0][3] = Key(99909, String("[AUX 1-9]"));       // AUX 1-9
    keymap[2][1][1][0][4] = Key(99908, String("[TRIM CLIP]"));     // TRIM CLIP
    keymap[2][1][1][0][5] = Key(MODIFIERKEY_RIGHT_ALT, String("[MARK IN -> ALT GR]"));// MARK IN
    keymap[2][1][1][0][6] = Key(99906, String("[BLACK]"));         // BLACK
    keymap[2][1][1][0][7] = Key(KEY_SLASH, String("[/]"));         // ROLL /
    keymap[2][1][1][1][0] = Key(KEY_F7, String("[DEV XCHNG -> F7]"));    // DEV XCHNG
    keymap[2][1][1][1][1] = Key(KEY_9, String("[9]"));             // 9
    keymap[2][1][1][1][2] = Key(KEY_I, String("[I]"));             // I
    keymap[2][1][1][1][3] = Key(KEY_K, String("[K]"));             // K
    keymap[2][1][1][1][4] = Key(KEY_COMMA, String("[""]"));        // DOUBLE QUOTE
    keymap[2][1][1][1][5] = Key(KEYPAD_0, String("[KP 0]"));       // KP 0
    keymap[2][1][1][1][6] = Key(KEYPAD_4, String("[KP 4]"));       // KP 4
    keymap[2][1][1][1][7] = Key(KEYPAD_7, String("[KP 7]"));       // KP 7
    // G2A #3: lighted key switches (lks)
    // KNOWN GOOD
    keymap[3][1][0][1][0] = Key(99904, String("[VID SWR]"));       // VID SWR
    keymap[3][1][0][1][1] = Key(99903, String("[RTD]"));           // RTD
    keymap[3][1][0][1][2] = Key(99902, String("[EFX]"));           // EFX
    keymap[3][1][0][1][3] = Key(99901, String("[TBC]"));           // TBC
    keymap[3][1][0][1][4] = Key(99900, String("[AUD SWR]"));       // AUD SWR
    keymap[3][1][0][1][5] = Key(99899, String("[CCR]"));           // CCR
    keymap[3][1][0][1][6] = Key(99898, String("[STIL]"));          // STIL
    keymap[3][1][0][1][7] = Key(99897, String("[RTR]"));           // RTR
    // UNKNOWN
    keymap[3][0][1][1][0] = Key(99896, String("[f5a3]"));
    keymap[3][0][1][1][1] = Key(99895, String("[f5b3]"));
    keymap[3][0][1][1][2] = Key(99894, String("[f5c3]"));
    keymap[3][0][1][1][3] = Key(99893, String("[f5d3]"));
    keymap[3][0][1][1][4] = Key(99892, String("[VFD Menu 1]"));     // VFD Menu 1
    keymap[3][0][1][1][5] = Key(99891, String("[VFD Menu 2]"));     // VFD Menu 2
    keymap[3][0][1][1][6] = Key(99890, String("[VFD Menu 3]"));     // VFD Menu 3
    keymap[3][0][1][1][7] = Key(99889, String("[VFD Menu 4]"));     // VFD Menu 4
    keymap[3][1][1][1][0] = Key(99888, String("[VFD Menu 5]"));     // VFD Menu 5
    keymap[3][1][1][1][1] = Key(99887, String("[VFD Menu 6]"));     // VFD Menu 6
  }

  static const int NUM_DMX = 4;
  static const int NUM_DMX_STATES = 2; // It's binary, on/off
  static const int NUM_RETURN_LINES = 8;

private:

  Key keymap
    [NUM_DMX]  // g2a for fn, k1, k2j, lks
    [NUM_DMX_STATES]  // selA
    [NUM_DMX_STATES]  // selB
    [NUM_DMX_STATES]  // selC
    [NUM_RETURN_LINES]  // return line
  ;
};

#endif