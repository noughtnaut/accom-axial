#ifndef ACCOM_VFD
#define ACCOM_VFD

#include <Arduino.h>
// Other includes must be listed in the main INO file
#include <vector>

#include "01_logger.h"
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
  Pin pinLed = Pin::getLed(); // For debugging, to indicate various operations

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
  static const int VFD_CURSOR_LINE = 0x16; // CM1 ??? TODO check if this is true, datasheet says "no action"
  static const int VFD_CURSOR_BLOCK = 0x17; // CM2 "cursor position lit"
  static const int VFD_CURSOR_INVERT = 0x18; // CM3 "cursor position lit in reverse" TODO verify this means "inverted"
  // SB registers a user character. Seems to require parallel (mentions use of "D0..D4" and saying "D5..D7 are invalid")
  static const int VFD_ESC = 0x1b; // ESC "the cursor position may be defined by one byte after the ESC data"

  Pin pinBusy = Pin(PIN_BUSY, INPUT, HIGH, false); // BUSY (r31)
  Pin pinA0 = Pin(12, OUTPUT, LOW, false); // A0 (r30)
  Pin pinBL = Pin(14, OUTPUT, LOW, false); // BL (r32)
  // pinCS is always LOW (hardwired to GND on the back of the VFD board; only needed when multiplexing several VFD's)

  int numCharsPerRow = 0;
  int numRows = 0;
  int numSlots = 0;
  int numCharsPerSlot = numCharsPerRow;
  int numCharsTotal = 0;
  std::vector<char> buffer;
  std::vector<int> slotCentre;

  void updateCursorPosition(int byte) {
//    logger.begin("updateCursorPosition(byte)");
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
//    logger.end();
  }

  void sendData(int byte) {
//    logger.begin("sendData(byte)");
//    pinLed.on();
    if (pinBusy.isActive()) {
      // Serial.print("Waiting for VFD to become ready ... ");
      while (pinBusy.isActive())
        ;  // wait for VFD to become ready
      // Serial.println("ok");
    }
    Serial1.write(byte);
    updateCursorPosition(byte); // Update cursor position unless we're sending non-printing characters
//    sprintf(buffer, "Current pos: %i", currentCursorPosition);
//    logger.logln(buffer);
//    pinLed.off();
//    logger.end();
  }

  void sendData(String text) {
//    logger.begin("sendByte(string)");
//    pinLed.on();
    if (pinBusy.isActive()) {
      // Serial.print("Waiting for VFD to become ready ... ");
      while (pinBusy.isActive())
        ;  // wait for VFD to become ready
      // Serial.println("ok");
    }
    Serial1.print(text.c_str());
    currentCursorPosition += text.length(); // TODO This assumes no non-printing characters
//    sprintf(buffer, "Current pos: %i", currentCursorPosition);
//    logger.logln(buffer);
//    pinLed.off();
//    logger.end();
  }

  void sendCommand(int byte) {
//    logger.begin("sendCommand(byte,bool)");
    pinA0.on();
    sendData(byte);
    pinA0.off();
//    logger.end();
  }

public:

  Vfd() {
    Vfd(1, 1, 0);
  }

  Vfd(int newNumRows, int newNumCharsPerRow, int newNumSlots) {
    logger.begin("vfd");
    pinLed.on();
  // Setup Teensy UART, see: https://www.pjrc.com/teensy/td_uart.html
    Serial1.setTX(PIN_SERIAL);
//    Serial1.attachCts(PIN_BUSY);
    Serial1.begin(VFD_BAUD, SERIAL_8N1);
//    sprintf(buffer, "Serial1.availableForWrite()=%i", Serial1.availableForWrite());
//    logger.logln(buffer);
//---------
    numRows = newNumRows;
    numCharsPerRow = newNumCharsPerRow;
    numCharsTotal = (numRows*numCharsPerRow);
    buffer.resize(numCharsTotal);
    numSlots = newNumSlots;
    if (numSlots) {
      numCharsPerSlot = ROUNDED_INT_DIV(numCharsPerRow, numSlots);
      for (int slot=0; slot<numSlots; slot++) {
        int centre = slot*numCharsPerSlot + numCharsPerSlot/2;
        slotCentre.push_back(centre);
      }
    } else {
      numCharsPerSlot = numCharsPerRow;
      slotCentre.push_back(numCharsPerRow/2);
    }
    reset();
    pinLed.off();
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
//    logger.logln("on");
  }

  void off() {
    pinBL.on();
    delay(5);
//    logger.logln("off");
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

  void cursorLine() {
    sendData(VFD_CURSOR_LINE);
  }

  void cursorBlock() {
    sendData(VFD_CURSOR_BLOCK);
  }

  void cursorInvertedBlock() {
    sendData(VFD_CURSOR_INVERT);
  }

  /**
   * Brightness control
   * 4=most bright (100%)
   * 1=least bright (25%)
   * 0=off
  **/
  void brightness(int level) {
//    logger.log("brightness: ");
    switch(level) {
      default: // Full brightness
        sendData(1);
        on();
        break;
      case 4: // DIM1 = 100%
      case 3: // DIM2 =  75%
      case 2: // DIM3 =  50%
      case 1: // DIM4 =  25%
        sendData(5-level);
        on();
        break;
      case 0: // Turn display off
        off();
    }
  }

  /**
   * Assumes starting at zero brightness
  **/
  void fadeIn(int duration) {
//    logger.begin("fadeIn");
    brightness(1);
    for (int level=2; level <= 4; level++) {
      delay(duration/4);
      brightness(level);
    }
//    logger.end();
  }

  /**
   * Assumes starting at full brightness
  **/
  void fadeOut(int duration) {
//    logger.begin("fadeOut");
    for (int level=3; level >= 1; level--) {
      brightness(level);
      delay(duration/4);
    }
    off();
//    logger.end();
  }

  void moveBy(int adjustment) {
//    logger.begin("moveBy");
    // Normalise position within display
    while (adjustment<0) adjustment += numCharsTotal;
    while (adjustment>numCharsTotal) adjustment -= numCharsTotal;
    if (!adjustment) {
//      logger.logln("Normalising obviates adjustment");
//      logger.end();
      return;
    }
//    sprintf(buffer, "Need to adjust by %i positions", adjustment);
//    logger.logln(buffer);

    // Move shortest direction (possibly wrapping)
    bool goForward = true;
    if (adjustment>numCharsTotal/2) {
//      logger.log("easier going the other way, ");
      adjustment = numCharsTotal-adjustment;
      goForward = false;
//      sprintf(buffer, "adjust by %i positions backward", adjustment);
//      logger.appendln(buffer);
    }

    // Move the cursor by the required adjustment: use HT to go right, or BS to go left)
    int charToSend = goForward ? VFD_HT : VFD_BS;
    for (int m = 0; m<adjustment; m++) { // TODO Take wrapping mode into account
      sendData(charToSend);
    }
//    logger.end();
  }

  void moveTo(int row, int col) {
//    logger.begin("moveTo");
    pinLed.on();
    if(row >= 1 && row <= getNumRows()
    && col >= 1 && col <= getNumCharsPerRow()
    ) {
      // TODO Optimise to use direct position instead of relative
      // TODO Optimise to use VFD_CR to jump by quarter rows
      int desiredPos = (col + (row-1)*numCharsPerRow) % numCharsTotal;
//      sprintf(buffer, "Current pos: %i", currentCursorPosition);
//      logger.logln(buffer);
//      sprintf(buffer, "Desired pos: %i", desiredPos);
//      logger.logln(buffer);
      int adjustment = desiredPos-currentCursorPosition;
//      sprintf(buffer, "Need to move %i positions ", adjustment);
//      logger.logln(buffer);
      if (adjustment) {
        moveBy(adjustment);
      }
    } else {
//      logger.logln("Invalid position");
    }
    pinLed.off();
//    logger.end();
  }

  void reset() {
    logger.begin("reset");
//    sendCommand(VFD_CMD_RESET);

//    off(); // Just to avoid any flicker while resetting
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
//    logger.begin("fill");
    std::fill_n(buffer.begin(), buffer.size(), filler);
    sendData(buffer.data());
//    logger.end();
  }

  void clear() {
    sendData(VFD_CLEAR);
    currentCursorPosition = 1;
  }

  /**
   * Caution! Using this to send visible characters is going to mess up positioning!
  **/
  void sendRawByte(int byte) {
    sendData(byte);
  }

  void setTextAt(int row, int col, String text) {
//    logger.begin("setTextAt");
//    sprintf(buffer, "pos: %i, %i", row, col);
//    logger.logln(buffer);
//    logger.log("text: ");
//    logger.appendln(text.c_str());
    moveTo(row, col);
    sendData(text);
//    logger.end();
  }

  void setTextCentredAt(int row, int centre, String text) {
//    logger.begin("setTextCentredAt");
    int col = centre - text.length()/2;
    setTextAt(row, col, text);
//    logger.end();
  }

  /**
   * Row 0 (top) or 1 (bottom), slot 0..5
   * Note: Does not clip overlong strings (will overflow adjacent slots)
  **/
  void setSlotText(int row, int slot, String text) {
//    logger.begin("setSlotText");
    int col = slotCentre.at(slot);
    setTextCentredAt(row, col, text);
    // Alternatively, with boundary clipping:
    // setTextCentredAt(row, col, text.substring(0, numCharsPerSlot));
//    logger.end();
  }
};

#endif