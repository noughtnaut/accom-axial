#ifndef ACCOM_MAP
#define ACCOM_MAP

#include <Arduino.h>

class Key {
private:
  int usbKeyCode = 0;  // @see https://www.pjrc.com/teensy/td_keyboard.html
  String label = String("☹");
  bool pressed = false;
  bool changedSinceLastQuery = false;

public:
  Key() {}

  Key(int keyCode) {
    usbKeyCode = keyCode;
  }

  Key(String str) {
    usbKeyCode = -1;  // Not a standard USB key
    label = str;
  }

  int getUsbKeyCode() {
    return usbKeyCode;
  }

  bool isStandardUsbKey() {
    return -1 != usbKeyCode;
  }

  const char* getLabel() {
    return label.c_str();
  }

  bool isPressed() {
    return pressed;
  }

  bool setPressed(bool newState) {
    if (pressed != newState) {
      changedSinceLastQuery = true;
      pressed = newState;
    } else {
      changedSinceLastQuery = false;
    }
    return changedSinceLastQuery;
  }

  // Reports TRUE only on the first call after last state change
  bool changed() {
    bool output = changedSinceLastQuery;
    changedSinceLastQuery = false;
    return output;
  }
};

const Key KEY_UNDEFINED = Key();

class Keymap {
private:
  Key keymap
    [3]  // g2a
    [2]  // selA
    [2]  // selB
    [2]  // selC
    [8]  // return line
    ;

public:
  Key& get(int g2a, bool selA, bool selB, bool selC, int rx) {
    //    Serial.printf("keymap[%i][%i][%i][%i][%i]\n", g2a, selA, selB, selC, rx);
    return keymap[g2a][selA][selB][selC][rx];
  }

  Keymap() {
    Serial.print("keymap:");
    // Define all keys as "undefined"
    for (int g2a = 0; g2a <= 2; g2a++) {
      for (int selA = 0; selA <= 1; selA++) {
        for (int selB = 0; selB <= 1; selB++) {
          for (int selC = 0; selC <= 1; selC++) {
            for (int rl = 0; rl <= 7; rl++) {
              keymap[g2a][selA][selB][selC][rl] = KEY_UNDEFINED;
            }
          }
        }
      }
    }
    // Then overwrite slots for known keys
    keymap[0][0][0][1][0] = Key(MODIFIERKEY_GUI);           // SYSTM
    keymap[0][0][0][1][1] = Key(String("[HELP]"));          // HELP
    keymap[0][0][0][1][2] = Key(String("[f1c2]"));          // Fn block 1 col 3 row 2
    keymap[0][0][0][1][3] = Key(String("[AUDIO]"));         // AUDIO
    keymap[0][0][0][1][4] = Key(String("[CLEAN ON/OFF]"));  // CLEAN ON/OFF
    keymap[0][0][0][1][5] = Key(String("[FILTR]"));         // FILTR
    keymap[0][0][0][1][6] = Key(String("[RIPPLE MODE]"));   // RIPPLE MODE
    keymap[0][0][0][1][7] = Key(String("[LIST TRACK]"));    // LIST TRACK
    keymap[0][0][1][0][0] = Key(String("[SESSN MNGR]"));    // SESSN MNGR
    keymap[0][0][1][0][1] = Key(String("[ASIGN MNGR]"));    // ASIGN MNGR
    keymap[0][0][1][0][2] = Key(String("[INIT MNGR]"));     // INIT MNGR
    keymap[0][0][1][0][3] = Key(String("[FILE MNGR]"));     // FILE MNGR
    keymap[0][0][1][0][4] = Key(String("[LIST I/O]"));      // LIST I/O
    keymap[0][0][1][0][5] = Key(String("[f2b1]")),
    keymap[0][0][1][0][6] = Key(String("[f2c1]")),
    keymap[0][0][1][0][7] = Key(String("[f2d1]")),
    keymap[0][0][1][1][0] = Key(String("[LEARN MACRO]"));  // LEARN MACRO
    keymap[0][0][1][1][1] = Key(String("[MACRO 1/4]"));    // MACRO 1/4
    keymap[0][0][1][1][2] = Key(String("[MACRO 2/5]"));    // MACRO 2/5
    keymap[0][0][1][1][3] = Key(String("[MACRO 3/6]"));    // MACRO 3/6
    keymap[0][0][1][1][4] = Key(String("[LIST]"));         // LIST
    keymap[0][0][1][1][5] = Key(String("[SORT]"));         // SORT
    keymap[0][0][1][1][6] = Key(String("[EDIT NMBR]"));    // EDIT NMBR
    keymap[0][0][1][1][7] = Key(String("[TITLE NOTES]"));  // TITLE NOTES
    keymap[0][1][0][1][0] = Key(String("[f3a2]")),
    keymap[0][1][0][1][1] = Key(String("[f3b2]")),
    keymap[0][1][0][1][2] = Key(String("[f3c2]")),
    keymap[0][1][0][1][3] = Key(String("[NEW VERSN]"));  // NEW VERSN
    keymap[0][1][0][1][4] = Key(String("[PRE READ]"));   // PRE READ
    keymap[0][1][0][1][5] = Key(String("[f4b2]")),
    keymap[0][1][0][1][6] = Key(String("[f4c2]")),
    keymap[0][1][0][1][7] = Key(String("[f4d2]")),
    keymap[0][1][1][0][0] = Key(String("[UN/FREEZE]"));    // UNFRZ / FREEZ
    keymap[0][1][1][0][1] = Key(String("[LOOP]"));         // LOOP
    keymap[0][1][1][0][2] = Key(String("[EE ON/OFF]"));    // EE ON/OFF
    keymap[0][1][1][0][3] = Key(KEY_MEDIA_EJECT);          // EJECT
    keymap[0][1][1][0][4] = Key(String("[SWR ON/OFF]"));   // SWR ON/OFF
    keymap[0][1][1][0][5] = Key(String("[MULTI REC]"));    // MULTI REC
    keymap[0][1][1][0][6] = Key(String("[COLOR FRAME]"));  // COLOR FRAME
    keymap[0][1][1][0][7] = Key(String("[SRC ADV]"));      // SRC ADV
    keymap[0][1][1][1][0] = Key(String("[CLEAR MARKS]"));  // CLEAR MARKS
    keymap[0][1][1][1][1] = Key(String("[OPEN END]"));     // OPEN END
    keymap[0][1][1][1][2] = Key(String("[f3c3]")),
    keymap[0][1][1][1][3] = Key(String("[f3d3]")),
    keymap[0][1][1][1][4] = Key(String("[HIST UNDO]"));      // HIST UNDO
    keymap[0][1][1][1][5] = Key(String("[TO]"));             // TO
    keymap[0][1][1][1][6] = Key(String("[DF / NDF]"));       // DF / NDF
    keymap[0][1][1][1][7] = Key(String("[F1 / F2]"));        // F1 / F2
    keymap[1][0][0][0][0] = Key(String("[XCHNG]"));          // XCHNG
    keymap[1][0][0][0][1] = Key(KEY_8);                      // 8
    keymap[1][0][0][0][2] = Key(KEY_U);                      // U
    keymap[1][0][0][0][3] = Key(KEY_J);                      // J
    keymap[1][0][0][0][4] = Key(KEY_M);                      // M
    keymap[1][0][0][0][5] = Key(KEY_SPACE);                  // SPACE
    keymap[1][0][0][0][6] = Key(String("[SET/TRIM OUT]"));   // SET/TRIM OUT
    keymap[1][0][0][0][7] = Key(String("[SET/TRIM DUR]"));   // SET/TRIM DUR
    keymap[1][0][0][1][0] = Key(String("[CLIP LIBRY]"));     // CLIP LIBRY
    keymap[1][0][0][1][1] = Key(KEY_4);                      // 4
    keymap[1][0][0][1][2] = Key(KEY_E);                      // E
    keymap[1][0][0][1][3] = Key(KEY_D);                      // D
    keymap[1][0][0][1][4] = Key(KEY_C);                      // C
    keymap[1][0][0][1][5] = Key(KEYPAD_PERIOD);              // NUM FR/TC
    keymap[1][0][0][1][6] = Key(KEYPAD_6);                   // NUM 6
    keymap[1][0][0][1][7] = Key(KEYPAD_ENTER);               // NUM ENTER
    keymap[1][0][1][0][0] = Key(String("[TRANS EDITR]"));    // TRANS EDITR
    keymap[1][0][1][0][1] = Key(KEY_6);                      // 6
    keymap[1][0][1][0][2] = Key(KEY_T);                      // T
    keymap[1][0][1][0][3] = Key(KEY_G);                      // G
    keymap[1][0][1][0][4] = Key(KEY_B);                      // B
    keymap[1][0][1][0][5] = Key(String("00"));               // NUM 00
    keymap[1][0][1][0][6] = Key(KEYPAD_5);                   // NUM 5
    keymap[1][0][1][0][7] = Key(KEYPAD_2);                   // NUM 2
    keymap[1][0][1][1][0] = Key(String("[R MARK]"));         // R MARK
    keymap[1][0][1][1][1] = Key(KEY_2);                      // 2
    keymap[1][0][1][1][2] = Key(KEY_Q);                      // Q
    keymap[1][0][1][1][3] = Key(KEY_A);                      // A
    keymap[1][0][1][1][4] = Key(KEY_Z);                      // Z
    keymap[1][0][1][1][5] = Key(MODIFIERKEY_ALT);            // FWD
    keymap[1][1][0][0][0] = Key(String("[RAVE]"));           // RAVE
    keymap[1][1][0][0][1] = Key(KEY_7);                      // 7
    keymap[1][1][0][0][2] = Key(KEY_Y);                      // Y
    keymap[1][1][0][0][3] = Key(KEY_H);                      // H
    keymap[1][1][0][0][4] = Key(KEY_N);                      // N
    keymap[1][1][0][0][5] = Key(String("[DFINE CONST]"));    // DFINE CONST
    keymap[1][1][0][0][6] = Key(String("[FIND XFER]"));      // FIND XFER
    keymap[1][1][0][0][7] = Key(KEYPAD_1);                   // NUM 1
    keymap[1][1][0][1][0] = Key(String("[CACHE]"));          // CACHE
    keymap[1][1][0][1][1] = Key(KEY_3);                      // 3
    keymap[1][1][0][1][2] = Key(KEY_W);                      // W
    keymap[1][1][0][1][3] = Key(KEY_S);                      // S
    keymap[1][1][0][1][4] = Key(KEY_X);                      // X
    keymap[1][1][0][1][6] = Key(String("[YES]"));            // NUM + / YES
    keymap[1][1][0][1][7] = Key(String("[NO]"));             // NUM - / NO
    keymap[1][1][1][0][0] = Key(String("[GT TEXT]"));        // GT TEXT
    keymap[1][1][1][0][1] = Key(KEY_5);                      // 5
    keymap[1][1][1][0][2] = Key(KEY_R);                      // R
    keymap[1][1][1][0][3] = Key(KEY_F);                      // F
    keymap[1][1][1][0][4] = Key(KEY_V);                      // V
    keymap[1][1][1][0][5] = Key(KEYPAD_8);                   // NUM 8
    keymap[1][1][1][0][6] = Key(KEYPAD_9);                   // NUM 9
    keymap[1][1][1][0][7] = Key(KEYPAD_3);                   // NUM 3
    keymap[1][1][1][1][0] = Key(KEY_ESC);                    // VIEW
    keymap[1][1][1][1][1] = Key(KEY_1);                      // 1
    keymap[1][1][1][1][2] = Key(String("[WKPS/EDL]"));       // WKPS/EDL
    keymap[1][1][1][1][3] = Key(KEY_CAPS_LOCK);              // CONTROL
    keymap[1][1][1][1][4] = Key(MODIFIERKEY_SHIFT);          // SHIFT L
    keymap[1][1][1][1][5] = Key(MODIFIERKEY_CTRL);           // REV
    keymap[2][0][0][0][0] = Key(String("[PREV]"));           // PREV
    keymap[2][0][0][0][1] = Key(KEY_HOME);                   // START
    keymap[2][0][0][0][2] = Key(String("[NEXT]"));           // NEXT
    keymap[2][0][0][0][3] = Key(KEY_PAGE_UP);                // PREV PAGE
    keymap[2][0][0][0][4] = Key(String("[VFD 2 COMMIT]"));   // VFD 2 COMMIT
    keymap[2][0][0][0][5] = Key(String("[VFD 4 COMMIT]"));   // VFD 4 COMMIT
    keymap[2][0][0][0][6] = Key(String("[VFD 6 COMMIT]"));   // VFD 6 COMMIT
    keymap[2][0][0][0][7] = Key(String("[USER]"));           // USER
    keymap[2][0][0][1][0] = Key(String("[MENU DATA]"));      // MENU DATA
    keymap[2][0][0][1][1] = Key(KEY_EQUAL);                  // = / CHNG
    keymap[2][0][0][1][2] = Key(KEY_LEFT_BRACE);             // [
    keymap[2][0][0][1][3] = Key(KEY_QUOTE);                  // SINGLE QUOTE
    keymap[2][0][0][1][4] = Key(MODIFIERKEY_RIGHT_SHIFT);    // SHIFT R
    keymap[2][0][0][1][5] = Key(MODIFIERKEY_RIGHT_CTRL);     // MARK OUT
    keymap[2][0][0][1][6] = Key(String("[A/V SWR]"));        // A/V SWR
    keymap[2][0][0][1][7] = Key(KEY_ENTER);                  // ENTER
    keymap[2][0][1][0][0] = Key(String("[REC/SRC]"));        // REC/SRC
    keymap[2][0][1][0][1] = Key(String("[MARK OUT JOG]"));   // MARK OUT JOG
    keymap[2][0][1][0][2] = Key(String("[PLAY]"));           // PLAY
    keymap[2][0][1][0][3] = Key(String("[JOG]"));            // JOG
    keymap[2][0][1][0][4] = Key(String("[VAR]"));            // VAR
    keymap[2][0][1][0][5] = Key(String("[SHUTL]"));          // SHUTL
    keymap[2][0][1][0][6] = Key(String("[ABSLT]"));          // ABSLT
    keymap[2][0][1][0][7] = Key(String("[SWR FADER]"));      // SWR FADER
    keymap[2][0][1][1][0] = Key(String("[D MEM]"));          // D MEM
    keymap[2][0][1][1][1] = Key(KEY_0);                      // 0
    keymap[2][0][1][1][2] = Key(KEY_O);                      // O
    keymap[2][0][1][1][3] = Key(KEY_L);                      // L
    keymap[2][0][1][1][4] = Key(KEY_PERIOD);                 // .
    keymap[2][0][1][1][5] = Key(String("[SET/TRIM IN]"));    // SET/TRIM IN
    keymap[2][0][1][1][6] = Key(String("[SET/TRIM RATE]"));  // SET/TRIM RATE
    keymap[2][0][1][1][7] = Key(String("[SLIDE]"));          // SLIDE
    keymap[2][1][0][0][0] = Key(MODIFIERKEY_RIGHT_ALT);      // MARK IN JOG
    keymap[2][1][0][0][1] = Key(KEY_END);                    // END
    keymap[2][1][0][0][2] = Key(String("[SELECT]"));         // SELECT
    keymap[2][1][0][0][3] = Key(KEY_PAGE_DOWN);              // NEXT PAGE
    keymap[2][1][0][0][4] = Key(String("[VFD 1 COMMIT]"));   // VFD 1 COMMIT
    keymap[2][1][0][0][5] = Key(String("[VFD 3 COMMIT]"));   // VFD 3 COMMIT
    keymap[2][1][0][0][6] = Key(String("[VFD 5 COMMIT]"));   // VFD 5 COMMIT
    keymap[2][1][0][0][7] = Key(String("[STOP]"));           // STOP
    keymap[2][1][0][1][0] = Key(String("[GPI]"));            // GPI
    // Note: The console key for "-/COPY" should report a "-", but in my own keyboard layout that becomes a "+"
    keymap[2][1][0][1][1] = Key(KEY_MINUS);                  // - / COPY
    keymap[2][1][0][1][2] = Key(KEY_P);                      // P
    keymap[2][1][0][1][3] = Key(KEY_SEMICOLON);              // ;
    keymap[2][1][0][1][4] = Key(KEY_SLASH);                  // ? / CANCL
    keymap[2][1][0][1][5] = Key(String("[MATCH]"));          // MATCH
    keymap[2][1][0][1][6] = Key(KEY_BACKSPACE);              // BKSP
    keymap[2][1][0][1][7] = Key(String("[ALL]"));            // ALL
    keymap[2][1][1][0][0] = Key(String("[D CMD]"));          // D CMD
    keymap[2][1][1][0][1] = Key(KEY_BACKSLASH);              // \\ / LAYER
    keymap[2][1][1][0][2] = Key(KEY_RIGHT_BRACE);            // ]
    keymap[2][1][1][0][3] = Key(String("[AUX 1-9]"));        // AUX 1-9
    keymap[2][1][1][0][4] = Key(String("[TRIM CLIP]"));      // TRIM CLIP
    keymap[2][1][1][0][5] = Key(String("[MARK IN]"));        // MARK IN
    keymap[2][1][1][0][6] = Key(String("[BLACK]"));          // BLACK
    keymap[2][1][1][0][7] = Key(KEY_SLASH);                  // ROLL /
    keymap[2][1][1][1][0] = Key(String("[DEV XCHNG]"));      // DEV XCHNG
    keymap[2][1][1][1][1] = Key(KEY_9);                      // 9
    keymap[2][1][1][1][2] = Key(KEY_I);                      // I
    keymap[2][1][1][1][3] = Key(KEY_K);                      // K
    keymap[2][1][1][1][4] = Key(KEY_COMMA);                  // DOUBLE QUOTE
    keymap[2][1][1][1][5] = Key(KEYPAD_0);                   // NUM 0
    keymap[2][1][1][1][6] = Key(KEYPAD_4);                   // NUM 4
    keymap[2][1][1][1][7] = Key(KEYPAD_7);                   // NUM 7
    Serial.print("ok ");
  }
};

#endif