//#include "../lib/HID/src/HID.h"
//#include "../lib/Keyboard/src/Keyboard.h"

#include "AxialKeymap.h"

void AxialKeymap::init() {
//	Serial.println("Generating keymap ...");
	// Define all keys as "undefined"
	for (int g2a = 0; g2a < NUM_DMX; g2a++) {
		for (int selA = 0; selA < NUM_DMX_STATES; selA++) {
			for (int selB = 0; selB < NUM_DMX_STATES; selB++) {
				for (int selC = 0; selC < NUM_DMX_STATES; selC++) {
					for (int rl = 0; rl < NUM_RETURN_LINES; rl++) {
						keymap[g2a][selA][selB][selC][rl] = AxialKey::UNDEFINED();
					}
				}
			}
		}
	}
	// Then overwrite slots for known keys
	//     ,--------------- g2a for fn, k1, k2j, lks
	//     |  ,------------ selA
	//     |  |  ,--------- selB
	//     |  |  |  ,------ selC
	//     |  |  |  |  ,--- return line
	//     v  v  v  v  v
	// G2A #0: function keys (fn)
	keymap[0][0][0][1][0] = AxialKey(MODIFIERKEY_GUI); // SYSTM
	keymap[0][0][0][1][1] = AxialKey(99999, String("[HELP]")); // HELP
	keymap[0][0][0][1][2] = AxialKey(99998, String("[f1c2]")); // Fn block 1 col 3 row 2
	keymap[0][0][0][1][3] = AxialKey(99997, String("[AUDIO]")); // AUDIO
	keymap[0][0][0][1][4] = AxialKey(99996, String("[CLEAN ON/OFF]")); // CLEAN ON/OFF
	keymap[0][0][0][1][5] = AxialKey(99995, String("[FILTR]")); // FILTR
	keymap[0][0][0][1][6] = AxialKey(99994, String("[RIPPLE MODE]")); // RIPPLE MODE
	keymap[0][0][0][1][7] = AxialKey(99993, String("[LIST TRACK]")); // LIST TRACK
	keymap[0][0][1][0][0] = AxialKey(99992, String("[SESSN MNGR]")); // SESSN MNGR
	keymap[0][0][1][0][1] = AxialKey(99991, String("[ASIGN MNGR]")); // ASIGN MNGR
	keymap[0][0][1][0][2] = AxialKey(99980, String("[INIT MNGR]")); // INIT MNGR
	keymap[0][0][1][0][3] = AxialKey(99989, String("[FILE MNGR]")); // FILE MNGR
	keymap[0][0][1][0][4] = AxialKey(99988, String("[LIST I/O]")); // LIST I/O
	keymap[0][0][1][0][5] = AxialKey(99987, String("[f2b1]")),
	keymap[0][0][1][0][6] = AxialKey(99986, String("[f2c1]")),
	keymap[0][0][1][0][7] = AxialKey(99985, String("[f2d1]")),
	keymap[0][0][1][1][0] = AxialKey(99984, String("[LEARN MACRO]")); // LEARN MACRO
	keymap[0][0][1][1][1] = AxialKey(99983, String("[MACRO 1/4]")); // MACRO 1/4
	keymap[0][0][1][1][2] = AxialKey(99982, String("[MACRO 2/5]")); // MACRO 2/5
	keymap[0][0][1][1][3] = AxialKey(99981, String("[MACRO 3/6]")); // MACRO 3/6
	keymap[0][0][1][1][4] = AxialKey(99980, String("[LIST]")); // LIST
	keymap[0][0][1][1][5] = AxialKey(99979, String("[SORT]")); // SORT
	keymap[0][0][1][1][6] = AxialKey(99978, String("[EDIT NMBR]")); // EDIT NMBR
	keymap[0][0][1][1][7] = AxialKey(99977, String("[TITLE NOTES]")); // TITLE NOTES
	keymap[0][1][0][1][0] = AxialKey(99976, String("[f3a2]")),
	keymap[0][1][0][1][1] = AxialKey(99975, String("[f3b2]")),
	keymap[0][1][0][1][2] = AxialKey(99974, String("[f3c2]")),
	keymap[0][1][0][1][3] = AxialKey(99973, String("[NEW VERSN]")); // NEW VERSN
	keymap[0][1][0][1][4] = AxialKey(99972, String("[PRE READ]")); // PRE READ
	keymap[0][1][0][1][5] = AxialKey(99971, String("[f4b2]")),
	keymap[0][1][0][1][6] = AxialKey(99970, String("[f4c2]")),
	keymap[0][1][0][1][7] = AxialKey(99969, String("[f4d2]")),
	keymap[0][1][1][0][0] = AxialKey(99968, String("[UN/FREEZE]")); // UNFRZ / FREEZ
	keymap[0][1][1][0][1] = AxialKey(99967, String("[LOOP]")); // LOOP
	keymap[0][1][1][0][2] = AxialKey(99966, String("[EE ON/OFF]")); // EE ON/OFF
	keymap[0][1][1][0][3] = AxialKey(KEY_MEDIA_EJECT); // EJECT
	keymap[0][1][1][0][4] = AxialKey(99965, String("[SWR ON/OFF]")); // SWR ON/OFF
	keymap[0][1][1][0][5] = AxialKey(99964, String("[MULTI REC]")); // MULTI REC
	keymap[0][1][1][0][6] = AxialKey(99963, String("[COLOR FRAME]")); // COLOR FRAME
	keymap[0][1][1][0][7] = AxialKey(99962, String("[SRC ADV]")); // SRC ADV
	keymap[0][1][1][1][0] = AxialKey(99961, String("[CLEAR MARKS]")); // CLEAR MARKS
	keymap[0][1][1][1][1] = AxialKey(99960, String("[OPEN END]")); // OPEN END
	keymap[0][1][1][1][2] = AxialKey(99959, String("[f3c3]")),
	keymap[0][1][1][1][3] = AxialKey(99958, String("[f3d3]")),
	keymap[0][1][1][1][4] = AxialKey(99957, String("[HIST UNDO]")); // HIST UNDO
	keymap[0][1][1][1][5] = AxialKey(99956, String("[TO]")); // TO
	keymap[0][1][1][1][6] = AxialKey(99955, String("[DF / NDF]")); // DF / NDF
	keymap[0][1][1][1][7] = AxialKey(99954, String("[F1 / F2]")); // F1 / F2
	// G2A #1: 2/3 of the qwerty keyboard (k1)
	keymap[1][0][0][0][0] = AxialKey(99953, String("[XCHNG]")); // XCHNG
	keymap[1][0][0][0][1] = AxialKey(KEY_8); // 8
	keymap[1][0][0][0][2] = AxialKey(KEY_U); // U
	keymap[1][0][0][0][3] = AxialKey(KEY_J); // J
	keymap[1][0][0][0][4] = AxialKey(KEY_M); // M
	keymap[1][0][0][0][5] = AxialKey(KEY_SPACE); // SPACE
	keymap[1][0][0][0][6] = AxialKey(99952, String("[SET/TRIM OUT]")); // SET/TRIM OUT
	keymap[1][0][0][0][7] = AxialKey(99951, String("[SET/TRIM DUR]")); // SET/TRIM DUR
	keymap[1][0][0][1][0] = AxialKey(99950, String("[CLIP LIBRY]")); // CLIP LIBRY
	keymap[1][0][0][1][1] = AxialKey(KEY_4); // 4
	keymap[1][0][0][1][2] = AxialKey(KEY_E); // E
	keymap[1][0][0][1][3] = AxialKey(KEY_D); // D
	keymap[1][0][0][1][4] = AxialKey(KEY_C); // C
	keymap[1][0][0][1][5] = AxialKey(KEYPAD_PERIOD); // NUM FR/TC
	keymap[1][0][0][1][6] = AxialKey(KEYPAD_6); // NUM 6
	keymap[1][0][0][1][7] = AxialKey(KEYPAD_ENTER); // NUM ENTER
	keymap[1][0][1][0][0] = AxialKey(99949, String("[TRANS EDITR]")); // TRANS EDITR
	keymap[1][0][1][0][1] = AxialKey(KEY_6); // 6
	keymap[1][0][1][0][2] = AxialKey(KEY_T); // T
	keymap[1][0][1][0][3] = AxialKey(KEY_G); // G
	keymap[1][0][1][0][4] = AxialKey(KEY_B); // B
	keymap[1][0][1][0][5] = AxialKey(99948, String("00")); // NUM 00
	keymap[1][0][1][0][6] = AxialKey(KEYPAD_5); // NUM 5
	keymap[1][0][1][0][7] = AxialKey(KEYPAD_2); // NUM 2
	keymap[1][0][1][1][0] = AxialKey(99947, String("[R MARK]")); // R MARK
	keymap[1][0][1][1][1] = AxialKey(KEY_2); // 2
	keymap[1][0][1][1][2] = AxialKey(KEY_Q); // Q
	keymap[1][0][1][1][3] = AxialKey(KEY_A); // A
	keymap[1][0][1][1][4] = AxialKey(KEY_Z); // Z
	keymap[1][0][1][1][5] = AxialKey(MODIFIERKEY_ALT); // FWD
	keymap[1][1][0][0][0] = AxialKey(99946, String("[RAVE]")); // RAVE
	keymap[1][1][0][0][1] = AxialKey(KEY_7); // 7
	keymap[1][1][0][0][2] = AxialKey(KEY_Y); // Y
	keymap[1][1][0][0][3] = AxialKey(KEY_H); // H
	keymap[1][1][0][0][4] = AxialKey(KEY_N); // N
	keymap[1][1][0][0][5] = AxialKey(99945, String("[DFINE CONST]")); // DFINE CONST
	keymap[1][1][0][0][6] = AxialKey(99944, String("[FIND XFER]")); // FIND XFER
	keymap[1][1][0][0][7] = AxialKey(KEYPAD_1); // NUM 1
	keymap[1][1][0][1][0] = AxialKey(99943, String("[CACHE]")); // CACHE
	keymap[1][1][0][1][1] = AxialKey(KEY_3); // 3
	keymap[1][1][0][1][2] = AxialKey(KEY_W); // W
	keymap[1][1][0][1][3] = AxialKey(KEY_S); // S
	keymap[1][1][0][1][4] = AxialKey(KEY_X); // X
	keymap[1][1][0][1][6] = AxialKey(99942, String("[YES]")); // NUM + / YES
	keymap[1][1][0][1][7] = AxialKey(99941, String("[NO]")); // NUM - / NO
	keymap[1][1][1][0][0] = AxialKey(99940, String("[GT TEXT]")); // GT TEXT
	keymap[1][1][1][0][1] = AxialKey(KEY_5); // 5
	keymap[1][1][1][0][2] = AxialKey(KEY_R); // R
	keymap[1][1][1][0][3] = AxialKey(KEY_F); // F
	keymap[1][1][1][0][4] = AxialKey(KEY_V); // V
	keymap[1][1][1][0][5] = AxialKey(KEYPAD_8); // NUM 8
	keymap[1][1][1][0][6] = AxialKey(KEYPAD_9); // NUM 9
	keymap[1][1][1][0][7] = AxialKey(KEYPAD_3); // NUM 3
	keymap[1][1][1][1][0] = AxialKey(KEY_ESC); // VIEW
	keymap[1][1][1][1][1] = AxialKey(KEY_1); // 1
	keymap[1][1][1][1][2] = AxialKey(99939, String("[WKPS/EDL]")); // WKPS/EDL
	keymap[1][1][1][1][3] = AxialKey(KEY_CAPS_LOCK); // CONTROL
	keymap[1][1][1][1][4] = AxialKey(MODIFIERKEY_SHIFT); // SHIFT L
	keymap[1][1][1][1][5] = AxialKey(MODIFIERKEY_CTRL); // REV
	// G2A #2: 1/3 of the qwerty keyboard, and the jog wheel board (k2j)
	keymap[2][0][0][0][0] = AxialKey(99938, String("[PREV]")); // PREV
	keymap[2][0][0][0][1] = AxialKey(KEY_HOME); // START
	keymap[2][0][0][0][2] = AxialKey(99937, String("[NEXT]")); // NEXT
	keymap[2][0][0][0][3] = AxialKey(KEY_PAGE_UP); // PREV PAGE
	keymap[2][0][0][0][4] = AxialKey(99936, String("[VFD 2 COMMIT]")); // VFD 2 COMMIT
	keymap[2][0][0][0][5] = AxialKey(99935, String("[VFD 4 COMMIT]")); // VFD 4 COMMIT
	keymap[2][0][0][0][6] = AxialKey(99934, String("[VFD 6 COMMIT]")); // VFD 6 COMMIT
	keymap[2][0][0][0][7] = AxialKey(99933, String("[USER]")); // USER
	keymap[2][0][0][1][0] = AxialKey(99932, String("[MENU DATA]")); // MENU DATA
	keymap[2][0][0][1][1] = AxialKey(KEY_EQUAL); // = / CHNG
	keymap[2][0][0][1][2] = AxialKey(KEY_LEFT_BRACE); // [
	keymap[2][0][0][1][3] = AxialKey(KEY_QUOTE); // SINGLE QUOTE
	keymap[2][0][0][1][4] = AxialKey(MODIFIERKEY_RIGHT_SHIFT); // SHIFT R
	keymap[2][0][0][1][5] = AxialKey(MODIFIERKEY_RIGHT_CTRL); // MARK OUT
	keymap[2][0][0][1][6] = AxialKey(99931, String("[A/V SWR]")); // A/V SWR
	keymap[2][0][0][1][7] = AxialKey(KEY_ENTER); // ENTER
	keymap[2][0][1][0][0] = AxialKey(99930, String("[REC/SRC]")); // REC/SRC
	keymap[2][0][1][0][1] = AxialKey(99929, String("[MARK OUT JOG]")); // MARK OUT JOG
	keymap[2][0][1][0][2] = AxialKey(99928, String("[PLAY]")); // PLAY
	keymap[2][0][1][0][3] = AxialKey(99927, String("[JOG]")); // JOG
	keymap[2][0][1][0][4] = AxialKey(99926, String("[VAR]")); // VAR
	keymap[2][0][1][0][5] = AxialKey(99925, String("[SHUTL]")); // SHUTL
	keymap[2][0][1][0][6] = AxialKey(99924, String("[ABSLT]")); // ABSLT
	keymap[2][0][1][0][7] = AxialKey(99993, String("[SWR FADER]")); // SWR FADER
	keymap[2][0][1][1][0] = AxialKey(99992, String("[D MEM]")); // D MEM
	keymap[2][0][1][1][1] = AxialKey(KEY_0); // 0
	keymap[2][0][1][1][2] = AxialKey(KEY_O); // O
	keymap[2][0][1][1][3] = AxialKey(KEY_L); // L
	keymap[2][0][1][1][4] = AxialKey(KEY_PERIOD); // .
	keymap[2][0][1][1][5] = AxialKey(99921, String("[SET/TRIM IN]")); // SET/TRIM IN
	keymap[2][0][1][1][6] = AxialKey(99920, String("[SET/TRIM RATE]")); // SET/TRIM RATE
	keymap[2][0][1][1][7] = AxialKey(99919, String("[SLIDE]")); // SLIDE
	keymap[2][1][0][0][0] = AxialKey(99907, String("[MARK IN]")); // MARK IN JOG
	keymap[2][1][0][0][1] = AxialKey(KEY_END); // END
	keymap[2][1][0][0][2] = AxialKey(99918, String("[SELECT]")); // SELECT
	keymap[2][1][0][0][3] = AxialKey(KEY_PAGE_DOWN); // NEXT PAGE
	keymap[2][1][0][0][4] = AxialKey(99917, String("[VFD 1 COMMIT]")); // VFD 1 COMMIT
	keymap[2][1][0][0][5] = AxialKey(99916, String("[VFD 3 COMMIT]")); // VFD 3 COMMIT
	keymap[2][1][0][0][6] = AxialKey(99915, String("[VFD 5 COMMIT]")); // VFD 5 COMMIT
	keymap[2][1][0][0][7] = AxialKey(99914, String("[STOP]")); // STOP
	keymap[2][1][0][1][0] = AxialKey(99913, String("[GPI]")); // GPI
	keymap[2][1][0][1][1] = AxialKey(KEY_MINUS); // - / COPY
	keymap[2][1][0][1][2] = AxialKey(KEY_P); // P
	keymap[2][1][0][1][3] = AxialKey(KEY_SEMICOLON); // ;
	keymap[2][1][0][1][4] = AxialKey(KEY_SLASH); // ? / CANCL
	keymap[2][1][0][1][5] = AxialKey(99912, String("[MATCH]")); // MATCH
	keymap[2][1][0][1][6] = AxialKey(KEY_BACKSPACE); // BKSP
	keymap[2][1][0][1][7] = AxialKey(99911, String("[ALL]")); // ALL
	keymap[2][1][1][0][0] = AxialKey(99910, String("[D CMD]")); // D CMD
	keymap[2][1][1][0][1] = AxialKey(KEY_BACKSLASH); // \\ / LAYER
	keymap[2][1][1][0][2] = AxialKey(KEY_RIGHT_BRACE); // ]
	keymap[2][1][1][0][3] = AxialKey(99909, String("[AUX 1-9]")); // AUX 1-9
	keymap[2][1][1][0][4] = AxialKey(99908, String("[TRIM CLIP]")); // TRIM CLIP
	keymap[2][1][1][0][5] = AxialKey(MODIFIERKEY_RIGHT_ALT); // MARK IN
	keymap[2][1][1][0][6] = AxialKey(99906, String("[BLACK]")); // BLACK
	keymap[2][1][1][0][7] = AxialKey(KEY_SLASH); // ROLL /
	keymap[2][1][1][1][0] = AxialKey(99905, String("[DEV XCHNG]")); // DEV XCHNG
	keymap[2][1][1][1][1] = AxialKey(KEY_9); // 9
	keymap[2][1][1][1][2] = AxialKey(KEY_I); // I
	keymap[2][1][1][1][3] = AxialKey(KEY_K); // K
	keymap[2][1][1][1][4] = AxialKey(KEY_COMMA); // DOUBLE QUOTE
	keymap[2][1][1][1][5] = AxialKey(KEYPAD_0); // NUM 0
	keymap[2][1][1][1][6] = AxialKey(KEYPAD_4); // NUM 4
	keymap[2][1][1][1][7] = AxialKey(KEYPAD_7); // NUM 7
	// G2A #3: lighted key switches (lks)
	keymap[3][0][1][0][0] = AxialKey(99904, String("[VID SWR]")); // VID SWR
	keymap[3][0][1][0][1] = AxialKey(99903, String("[RTD]")); // RTD
	keymap[3][0][1][0][2] = AxialKey(99902, String("[EFX]")); // EFX
	keymap[3][0][1][0][3] = AxialKey(99901, String("[TBC]")); // TBC
	keymap[3][0][1][0][4] = AxialKey(99900, String("[AUD SWR]")); // AUD SWR
	keymap[3][0][1][0][5] = AxialKey(99899, String("[CCR]")); // CCR
	keymap[3][0][1][0][6] = AxialKey(99898, String("[STIL]")); // STIL
	keymap[3][0][1][0][7] = AxialKey(99897, String("[RTR]")); // RTR
	keymap[3][1][0][0][0] = AxialKey(99896, String("[f5a3]"));
	keymap[3][1][0][0][1] = AxialKey(99895, String("[f5b3]"));
	keymap[3][1][0][0][2] = AxialKey(99894, String("[f5c3]"));
	keymap[3][1][0][0][3] = AxialKey(99893, String("[f5d3]"));
	keymap[3][1][0][0][4] = AxialKey(99892, String("VFD Menu 1]")); //VFD Menu 1
	keymap[3][1][0][0][5] = AxialKey(99891, String("VFD Menu 2]")); //VFD Menu 2
	keymap[3][1][0][0][6] = AxialKey(99890, String("VFD Menu 3]")); //VFD Menu 3
	keymap[3][1][0][0][7] = AxialKey(99889, String("VFD Menu 4]")); //VFD Menu 4
	keymap[3][0][0][0][0] = AxialKey(99888, String("VFD Menu 5]")); //VFD Menu 5
	keymap[3][0][0][0][1] = AxialKey(99887, String("VFD Menu 6]")); //VFD Menu 6
//	Serial.println("Keymap is ready");
}