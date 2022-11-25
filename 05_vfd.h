#ifndef ACCOM_VFD
#define ACCOM_VFD

#include <Arduino.h>
// Other includes must be listed in the main INO file

#include "01_pin.h"
/**
 * Display driver for NEC FC40X2EA-AB
 *
 * No data should be sent while pin 27 (BUSY) is high.
 * Pin 23 (CS) should always be set low.
 * Pin 19 (A0) should be set low to send data, or high to send commands.
 * Pin 17 (WR) should be set high while sending a packet, then set low.
**/
class Vfd {

private:

  static const int VFD_BAUD_DELAY_MICROS = 105; // 1.000.000 / 9.600
  static const int VFD_CMD_RESET = 0xFF; // clears display and memory, sets power on condition
  static const int VFD_CURSOR_HIDE = 0x14; // DC4 "cursor turns off"
  static const int VFD_CURSOR_SHOW = 0x13; // DC3 "cursor turns on"
  static const int VFD_CURSOR_BLINK = 0x15; // DC5 "cursor turns on and blinks"
  static const int VFD_CURSOR_BLOCK = 0x17; // CM2 "lit
  //static const int VFD_CURSOR_LINE = 0x16; // CM1 ??? TODO check if this is true, datasheet says "no action"
  //static const int VFD_CURSOR_INVERT = 0x18; // CM3 "lit in reverse" ??? TODO verify this means "inverted"
  static const int VFD_HT = 0x09; // HT "cursor position shifts one character to the right (no wrap)"
  static const int VFD_CR = 0x0d; // CR "cursor position shifts to the left end"
  static const int VFD_ESC = 0x1b; // ESC "the cursor position may be defined by one byte after the ESC data"

  // TODO The pin numbers below are for the wrong socket!
  Pin pinBusy = Pin(51, HIGH); // BUSY (r27)
  Pin pinWR = Pin(40, OUTPUT, HIGH, LOW);  // WR (r17)
  Pin pinA0 = Pin(41, OUTPUT, LOW, HIGH);  // A0 (r19)
  Pin pinCS = Pin(53, OUTPUT, HIGH, LOW);  // CS (r23)
  Pin pinBL = Pin(30, OUTPUT, LOW, HIGH);  // BL (r29), pull down to blank
  Pin pinDS = Pin(32, OUTPUT, HIGH, LOW);  // RxS (r33) Serial data

  void sendSerialByte(int byte) {
    // Ensure VFD is ready to receive data
    if (pinBusy.isActive()) {
      // Serial.print("Waiting for VFD to become ready ... ");
      while (pinBusy.isActive())
        ;  // wait for VFD to become ready
      // Serial.println("ok");
    }

    // Send one "0" start bit
    pinDS.setActive(false);
    delayMicroseconds(VFD_BAUD_DELAY_MICROS);

    // Send one bit at a time (LSB to MSB)
    int mask = 0b00000001u;
    for (int b = 0; b < 8; b++) {
      pinDS.setActive(byte & mask);
      delayMicroseconds(VFD_BAUD_DELAY_MICROS);
      mask <<= 1;
    }

    // Send two "1" stop bits
    pinDS.setActive(true);
    delayMicroseconds(2 * VFD_BAUD_DELAY_MICROS);
    pinDS.setActive(false);
  }

  void sendCommand(int byte) {
    // Begin transmission
    pinWR.setActive(true);

    // Send command
    pinA0.setActive(true);
    sendSerialByte(byte);

    // End transmission
    pinWR.setActive(false);
  }

  void sendData(int byte) {
    pinA0.setActive(false);
    // Do not send "Begin transmission"
    sendSerialByte(byte);
    // Do not send "End transmission"
  }

  void sendData(String text) {
    pinA0.setActive(false);
    int len = text.length();
    const char *c = text.c_str();
    sendSerialByte(0); // FIXME Why does the first character get eaten?
    for (int i=0; i<len; i++) {
      sendSerialByte(c[i]);
    }
  }

public:

  Vfd() {
    Vfd(1, 20);
  }

  Vfd(int setHeight, int setWidth) {
    Vfd(setHeight, setWidth, 0);
  }

  Vfd(int setHeight, int setWidth, int setSlots) {
    Serial.print("vfd:");
    height = setHeight;
    width = setWidth;
    if (setSlots) {
      numSlots = setSlots;
      widthPerSlot = ROUNDED_INT_DIV(width, numSlots);
      for (int i=0; i<numSlots; i++) {
        int centre = i*widthPerSlot + widthPerSlot/2;
//        Serial.println(centre);
        slotCentre.push_back(centre);
      }
    }
    Serial.println("ok");
  }

  int getHeight() {
    return height;
  }

  int getWidth() {
    return width;
  }

  int getNumSlots() {
    return numSlots;
  }

  bool isBusy() {
    return pinBusy.isActive();
  }

  void on() {
    pinBL.setActive(false);
  }

  void off() {
    pinBL.setActive(true);
  }

  void cursorHide() { // FIXME Seems correct but doesn't work
    sendData(VFD_CURSOR_HIDE);
  }

  void cursorShow() { // FIXME Seems correct but doesn't work
    sendData(VFD_CURSOR_SHOW);
  }

  void cursorBlink() {
    sendData(VFD_CURSOR_BLINK);
  }

  void cursorBlock() {
    sendData(VFD_CURSOR_BLOCK);
  }

//  void cursorLine() {
//    sendData(VFD_CURSOR_LINE);
//  }

  void cursorPosition(int row, int col) {
    if(row >= 0 && row < getHeight()
    && col >= 0 && col < getWidth()
    ) {
      int target = col;
      for (int i=0; i < row; i++)
        target += getWidth();
      switch (0) { // FIXME Various approaches
        case 0: // Reposition relative to current position
          sendData(VFD_CR); // FIXME Only works first time???
          for (int i=0; i < target; i++)
            sendData(VFD_HT);
          break;
        case 1: // Use command to reposition
          sendCommand(0); // Set position 0
          // FIXME Works, but also clears all characters
          for (int i=0; i < target; i++)
            sendData(VFD_HT);
          break;
        case 2: // Use data to reposition
          sendData(VFD_ESC); // The next byte after this sets the position
          sendData(target); // FIXME No effect...
          break;
        case 3:
          // TODO Implement a software buffer and reset the entire display at every change
          break;
      }
    }
  }

  void reset() {
    sendCommand(VFD_CMD_RESET);
  }

  void sendCustomByte(int byte) {
    sendData(byte);
  }

  void setTextAt(int row, int col, String text) {
    Serial.printf("VFD row %i slot %i: '%s' ", row, col, text.c_str());
    cursorPosition(row, col);
    sendData(text);
    Serial.println("ok");
  }
};

#endif