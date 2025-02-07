#ifndef ACCOM_VFD
#define ACCOM_VFD

#include <Arduino.h>
// Other includes must be listed in the main INO file
#include <vector>

#include "01_pin.h"

// see: https://stackoverflow.com/a/18067292/14577190
#define ROUNDED_INT_DIV(n,d) ((((n)<0)^((d)<0))?(((n)-(d)/2)/(d)):(((n)+(d)/2)/(d)))

/**
 * Display driver for NEC FC40X2EA-AB
 * (based on datasheets for FC20X1SA and FC20X2JA)
**/
class Vfd {

private:

  int currentCursorPosition = 1; // 1-based; a 2x40 display has 80 positions
  //Pin pinLed = Pin::getLed(); // For debugging, to indicate various operations

  // Use Teensy UART, see: https://www.pjrc.com/teensy/td_uart.html
  static const int VFD_BAUD = 9600; // Match hardware setting on VFD daughter board
  static const int PIN_SERIAL = 1; // For Teensy 3.5, this is one of (1, 5, 26)
  static const int PIN_BUSY = 25; // The Teensy pin connected to the VFD daughter board's BUSY signal

  static const int VFD_CMD_RESET = 0xFF; // clears display and memory, sets power on condition
  static const int VFD_BS = 0x08; // BS "cursor position shifts one character to the left (unless at edge)"
  static const int VFD_HT = 0x09; // HT "cursor position shifts one character to the right (unless at edge)"
  static const int VFD_LF = 0x0a; // LF "all characters cleared, position remains"
  static const int VFD_CLEAR = 0x0c; // clears display and memory, sets power on condition
  static const int VFD_CR = 0x0d; // CR "cursor position shifts to the left end"
  static const int VFD_WRAP = 0x11; // DC1 "cursor shifts right after printing (wrap at edge)"
  static const int VFD_SCROLL = 0x12; // DC2 "cursor shifts right after printing (marquee when at edge)"
  static const int VFD_CURSOR_SHOW = 0x13; // DC3 "cursor turns on"
  static const int VFD_CURSOR_HIDE = 0x14; // DC4 "cursor turns off"
  static const int VFD_CURSOR_BLINK = 0x15; // DC5 "cursor turns on and blinks"
  static const int VFD_CURSOR_LINE = 0x16; // CM1 Ccurrent position has bottom row lit
  static const int VFD_CURSOR_BLOCK = 0x17; // CM2 "cursor position lit"
  static const int VFD_CURSOR_INVERT = 0x18; // CM3 "cursor position lit in reverse" TODO verify this means "inverted"
  // SB registers a user character. Seems to require parallel (mentions use of "D0..D4" and saying "D5..D7 are invalid")
  static const int VFD_ESC = 0x1b; // ESC "the cursor position may be defined by one byte after the ESC data"

  Pin pinLed = Pin(LED_BUILTIN, OUTPUT, HIGH, true); // Debug
  // VFD serial comms
  Pin pinBusy = Pin(PIN_BUSY, INPUT, HIGH, false); // BUSY (r31)
  Pin pinA0 = Pin(12, OUTPUT, LOW, false); // A0 (r30)
  Pin pinBL = Pin(14, OUTPUT, LOW, false); // BL (r32)
  // pinCS is always LOW (hardwired to GND on the back of the VFD board; only needed when multiplexing several VFD's)
  // VFD parallel comms
// FIXME Weirdly, just having this defined affects the display
//  Pin pinWR = Pin(26, OUTPUT, HIGH, false); // WR (r29)
  // Parallel data lines
  Pin pinD0 = Pin(0, OUTPUT, HIGH, false); // r14
  Pin pinD1 = Pin(1, OUTPUT, HIGH, false); // r16
  Pin pinD2 = Pin(2, OUTPUT, HIGH, false); // r18
  Pin pinD3 = Pin(3, OUTPUT, HIGH, false); // r20
  Pin pinD4 = Pin(4, OUTPUT, HIGH, false); // r22
  Pin pinD5 = Pin(5, OUTPUT, HIGH, false); // r24
  Pin pinD6 = Pin(6, OUTPUT, HIGH, false); // r26
  Pin pinD7 = Pin(7, OUTPUT, HIGH, false); // r28

  int numCharsPerRow = 0;
  int numRows = 0; // 1-based, from top
  int numSlots = 0; // 0-based, from left
  int numCharsPerSlot = numCharsPerRow;
  int numCharsTotal = 0;
  std::vector<char> buffer;
  std::vector<int> slotCentre;

  void updateCursorPosition(int byte) {
    //logger.begin("updateCursorPosition(byte)");
    // Update cursor position unless we're sending non-printing characters
    if (byte>=0x20) { // We're sending printing characters
      currentCursorPosition++;
    } else { // Low-numbered characters can do all sorts of funky stuff
      switch (byte) {
        case VFD_BS:
          currentCursorPosition--;
          break;
        case VFD_HT:
          currentCursorPosition++;
          break;
        case VFD_LF:
          currentCursorPosition += numCharsPerRow;
          break;
        case VFD_CLEAR:
          currentCursorPosition = 1;
          break;
        case VFD_CR: { // This bracket limits the scope of `currentRow` so that it does not leak into the `default` scope
          // For a 2x20 display: 1->1, 21-> 21, 17->1, 37->21
          int currentRow = 1 + currentCursorPosition/numCharsPerRow;
          currentCursorPosition = 1 + currentRow*numCharsPerRow;
        }
        default:
          // Do not update position
          break;
      }
    }
    // Normalise position within display
    while (currentCursorPosition<1) currentCursorPosition += numCharsTotal;
    while (currentCursorPosition>numCharsTotal) currentCursorPosition -= numCharsTotal;
    //logger.end();
  }

  void sendDataP(int byte) {
    //logger.begin("sendDataP()");
    pinLed.on();
    // Set byte bits
    //logger.log("Setting byte bits: ");
    pinD0.setActive(byte & 1);
    pinD1.setActive(byte & 2);
    pinD2.setActive(byte & 4);
    pinD3.setActive(byte & 8);
    pinD4.setActive(byte & 16);
    pinD5.setActive(byte & 32);
    pinD6.setActive(byte & 64);
    pinD7.setActive(byte & 128);
    //logger.appendln("ok");
    delay(5);
    //logger.log("Flipping WR ON: ");
//    pinWR.setActive(true); // Commence writing
    //logger.appendln("ok");
    delay(5);
    //logger.log("Flipping WR OFF: ");
//    pinWR.setActive(false); // Signal writing complete
    //logger.appendln("ok");
    delay(5);
    pinLed.off();
    //logger.end();
  }

  void awaitCTS() {
    if (pinBusy.isActive()) {
      logger.log("Waiting for VFD to become ready ... ");
      while (pinBusy.isActive()) // wait for VFD to become ready
        ; // FIXME We risk hanging here indefitinely, freezing the whole system
      logger.logln("ok");
    }
  }

  void sendData(int byte) {
    //logger.begin("sendData(byte)");
    //pinLed.on();
    awaitCTS();
    Serial1.write(byte);
    updateCursorPosition(byte); // Update cursor position unless we're sending non-printing characters
    //logger.log("Current pos: "); logger.appendln(currentCursorPosition);
    //pinLed.off();
    //logger.end();
  }

  void sendData(String text) {
    //logger.begin("sendData(string)");
    //pinLed.on();
    awaitCTS();
    Serial1.print(text.c_str());
    currentCursorPosition += text.length(); // TODO This assumes no non-printing characters
    //logger.log("Current pos: "); logger.appendln(currentCursorPosition);
    //pinLed.off();
    //logger.end();
  }

  void sendCommand(int byte) {
    //logger.begin("sendCommand(byte)");
    pinA0.on();
    sendData(byte);
    pinA0.off();
    //logger.end();
  }

  void initSerial() {
    // Setup Teensy UART, see: https://www.pjrc.com/teensy/td_uart.html
    Serial1.setTX(PIN_SERIAL);
    Serial1.begin(VFD_BAUD, SERIAL_8N1);
    //Serial1.attachCts(PIN_BUSY); // Experimental, don't know if we need it since we're also doing it ourselves
    //logger.log("Serial1.availableForWrite()="); logger.logln(Serial1.availableForWrite());
  }

  void initDimensions(int newNumRows, int newNumCharsPerRow, int newNumSlots) {
    logger.begin("dimensions");
    numRows = newNumRows;
    numCharsPerRow = newNumCharsPerRow;
    numCharsTotal = (numRows*numCharsPerRow);
    buffer.resize(numCharsTotal+1); // Only resize this once
    numSlots = newNumSlots;
    logger.log("slots: "); logger.appendln(numSlots);
    if (numSlots) {
      numCharsPerSlot = ROUNDED_INT_DIV(numCharsPerRow, numSlots);
      logger.log("chars per slot: "); logger.appendln(numCharsPerSlot);
      logger.log("over-allocated: "); logger.appendln(numSlots * numCharsPerSlot - numCharsPerRow);
      for (int slot=0; slot<numSlots; slot++) {
        logger.log("slot "); logger.append(slot);
        int centre = slot*numCharsPerSlot + numCharsPerSlot/2;
        logger.append(" centre: "); logger.appendln(centre);
        slotCentre.push_back(centre);
      }
    } else { // Just store one full-width slot
      numCharsPerSlot = numCharsPerRow;
      slotCentre.push_back(numCharsPerRow/2);
    }
    logger.end();
  }

public:

  static const byte ROW_TOP = 1;
  static const byte ROW_BOTTOM = 2;
  static const byte CHAR_BLOCK_FULL = 0x7f;
  static const byte CHAR_BLOCK_HATCHED = 0xfd;
  static const byte CHAR_BLOCK_LOWER_1 = 0x5f;
  static const byte CHAR_BLOCK_LOWER_2 = 0xe0;
  static const byte CHAR_BLOCK_LOWER_3 = 0xe1;
  static const byte CHAR_BLOCK_LOWER_4 = 0xe2;
  static const byte CHAR_BLOCK_LOWER_5 = 0xe3;
  static const byte CHAR_BLOCK_LOWER_6 = 0xe4;
  static const byte CHAR_BLOCK_RIGHT_1 = 0xe5;
  static const byte CHAR_BLOCK_RIGHT_2 = 0xe6;
  static const byte CHAR_BLOCK_RIGHT_3 = 0xe7;
  static const byte CHAR_BLOCK_RIGHT_4 = 0xe8;

  Vfd() {
    Vfd(1, 1, 0);
  }

  Vfd(int newNumRows, int newNumCharsPerRow, int newNumSlots) {
    logger.begin("init vfd");
    //pinLed.on();
    reset();
    initSerial();
    initDimensions(newNumRows, newNumCharsPerRow, newNumSlots);
    //pinLed.off();
    logger.end();
  }

  int getNumRows() {
    return numRows;
  }

  int getNumCharsPerRow() {
    return numCharsPerRow;
  }

  int getNumSlots() {
    return numSlots;
  }

  int getNumCharsPerSlot() {
    return numCharsPerSlot;
  }

  void on() {
    pinBL.off();
    //logger.logln("vfd on");
  }

  void off() {
    pinBL.on();
    delay(5);
    //logger.logln("vfd off");
  }

  void cursorShow() {
    sendData(VFD_CURSOR_SHOW);
  }

  void cursorHide() {
    sendData(VFD_CURSOR_HIDE);
  }

  void cursorBlink() {
    sendData(VFD_CURSOR_BLINK);
  }

  void cursorLine() { // does not imply making it visible
    sendData(VFD_CURSOR_LINE);
  }

  void cursorBlock() { // does not imply making it visible
    sendData(VFD_CURSOR_BLOCK);
  }

  void cursorInvertedBlock() { // does not imply making it visible
    sendData(VFD_CURSOR_INVERT);
  }

  /**
   * Brightness control
   * 4=most bright (100%)
   * 1=least bright (25%)
   * 0=off
  **/
  void brightness(int level) {
    //logger.log("brightness: ");
    switch(level) {
      default: // Full brightness
        sendData(1);
        on();
        //logger.logln("100%");
        break;
      case 4: // DIM1 = 100%
      case 3: // DIM2 =  75%
      case 2: // DIM3 =  50%
      case 1: // DIM4 =  25%
        sendData(5-level);
        //logger.log("level "); logger.logln(5-level);
        on();
        break;
      case 0: // Turn display off
        off();
        //logger.logln("0%");
    }
  }

  /**
   * Assumes starting at zero brightness
  **/
  void fadeIn(int duration) {
    //logger.begin("fadeIn");
    brightness(1);
    for (int level=2; level <= 4; level++) {
      delay(duration/4);
      brightness(level);
    }
    //logger.end();
  }

  /**
   * Assumes starting at full brightness
  **/
  void fadeOut(int duration) {
    //logger.begin("fadeOut");
    for (int level=3; level >= 1; level--) {
      brightness(level);
      delay(duration/4);
    }
    off();
    //logger.end();
  }

  void moveBy(int adjustment) {
    //logger.begin("moveBy");
    // Normalise position within display
    while (adjustment<0) adjustment += numCharsTotal;
    while (adjustment>numCharsTotal) adjustment -= numCharsTotal;
    if (!adjustment) {
      //logger.logln("Normalising obviates adjustment");
      //logger.end();
      return;
    }
    //logger.log("Need to adjust by "); logger.append(adjustment); logger.appendln(" positions");

    // Move shortest direction (possibly wrapping)
    bool goForward = true;
    if (adjustment>numCharsTotal/2) {
      //logger.log("easier going the other way, ");
      adjustment = numCharsTotal-adjustment;
      goForward = false;
      //logger.append("adjust by "); logger.append(adjustment); logger.appendln(" positions backward");
    }

    // Move the cursor by the required adjustment: use HT to go right, or BS to go left)
    int charToSend = goForward ? VFD_HT : VFD_BS;
    for (int m = 0; m<adjustment; m++) { // TODO Take wrapping mode into account
      sendData(charToSend);
    }
    //logger.end();
  }

  void moveTo(int row, int col) {
    //logger.begin("moveTo");
    //pinLed.on();
    if(row >= 1 && row <= getNumRows()
       && col >= 1 && col <= getNumCharsPerRow()
        ) {
      // TODO Optimise to use direct position instead of relative
      // TODO Optimise to use VFD_CR to jump by quarter rows
      int desiredPos = (col + (row-1)*numCharsPerRow) % numCharsTotal;
      logger.log("Current pos: "); logger.appendln(currentCursorPosition);
      logger.log("Desired pos: "); logger.appendln(desiredPos);
      int adjustment = desiredPos-currentCursorPosition;
      logger.log("Need to move "); logger.append(adjustment); logger.appendln(" positions");
      if (adjustment) {
        moveBy(adjustment);
      }
    } else {
      //logger.logln("Invalid position");
    }
    //pinLed.off();
    //logger.end();
  }

  void reset() {
    logger.begin("reset");

    off(); // Just to avoid any flicker while resetting
    sendCommand(VFD_CMD_RESET);
    sendData(VFD_WRAP);
    clear();
    cursorHide(); // For production
//    cursorLine(); // For debug
//    cursorBlock(); // For debug
//    cursorBlink(); // For debug

    brightness(100); // Just to demo that "level>3" will default to "full brightness"
    logger.end();
  }

  void fill(char filler) {
    //logger.begin("fill");
    std::fill_n(buffer.begin(), buffer.size()-1, filler);
    //logger.logln(buffer.data());
    sendData(buffer.data());
    //logger.end();
  }


  void clear() {
    sendData(VFD_CLEAR);
    currentCursorPosition = 1;
  }

  /**
   * Caution! Using this to send visible characters is going to mess up positioning!
  **/
  void sendRawByte(int byte) {
    sendDataP(byte);
    //sendDataP(byte);
  }

  void setTextAt(int row, int col, String text) {
    //logger.begin("setTextAt");
    //logger.log("row: "); logger.appendln(row);
    //logger.log("col: "); logger.appendln(col);
    //logger.log("text: '"); logger.append(text.c_str()); logger.appendln("'");
    moveTo(row, col);
    sendData(text);
    //logger.end();
  }

  void setTextCentredAt(int row, int centre, String text) {
    //logger.begin("setTextCentredAt");
    int col = centre - text.length()/2;
    //logger.log("centre: "); logger.appendln(col);
    setTextAt(row, col, text);
    //logger.end();
  }

  /**
   * Row 1 (top) or 2 (bottom), slot 1..numSlots
   * Note: Does not clip overlong strings (will overflow adjacent slots)
  **/
  void setSlotText(int row, int slot, String text) {
    //logger.begin("setSlotText");
    int col = slotCentre.at(slot-1);
    //logger.log("'"); logger.append(text.c_str()); logger.appendln("'");
    //logger.log("row, slot: "); logger.append(row); logger.append(", "); logger.appendln(slot);
    setTextCentredAt(row, col, text);
    // Alternatively, with boundary clipping:
    // setTextCentredAt(row, col, text.substring(0, numCharsPerSlot));
    // (note that this is insufficient to deal with non-equal column widths)
    //logger.end();
  }
};

#endif