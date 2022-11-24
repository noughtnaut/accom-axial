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

const int VFD_BAUD_DELAY_MICROS = 105; // 1.000.000 / 9.600
const int VFD_CMD_RESET = 0xFF; // clears display and memory, sets power on condition
const int VFD_CURSOR_HIDE = 0x14; // DC4 "cursor turns off"
const int VFD_CURSOR_SHOW = 0x13; // DC3 "cursor turns on"
const int VFD_CURSOR_BLINK = 0x15; // DC5 "cursor turns on and blinks"
const int VFD_CURSOR_BLOCK = 0x17; // CM2 "lit
//const int VFD_CURSOR_LINE = 0x16; // CM1 ??? TODO check if this is true, datasheet says "no action"
//const int VFD_CURSOR_INVERT = 0x18; // CM3 "lit in reverse" ??? TODO verify this means "inverted"

// TODO The pin numbers below are for the wrong socket!
Pin pinBusy(51, HIGH); // BUSY (r27)
Pin pinWR(40, OUTPUT, HIGH, LOW);  // WR (r17)
Pin pinA0(41, OUTPUT, LOW, HIGH);  // A0 (r19)
Pin pinCS(53, OUTPUT, HIGH, LOW);  // CS (r23)
Pin pinBL(30, OUTPUT, LOW, HIGH);  // BL (r29), pull down to blank
Pin pinDS(32, OUTPUT, HIGH, LOW);  // RxS (r33) Serial data

class Vfd {

private:

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
    Serial.print("vfd:");
    // Anything needed here?
    Serial.println("ok");
  }
  bool isBusy() {
    return pinBusy.isActive();
  }

  bool isBlank() {
    return pinBL.isActive();
  }

  void setBlank(bool toBlank) {
//    Serial.printf("VFD:setBlank(%s)\n", toBlank?"blank":"lit");
    pinBL.setActive(toBlank);
  }

  void setCursorHide() { // FIXME Seems correct but doesn't work
    sendData(VFD_CURSOR_HIDE);
  }

  void setCursorShow() { // FIXME Seems correct but doesn't work
    sendData(VFD_CURSOR_SHOW);
  }

  void setCursorBlink() {
    sendData(VFD_CURSOR_BLINK);
  }

  void setCursorBlock() {
    sendData(VFD_CURSOR_BLOCK);
  }

//  void setCursorLine() {
//    sendData(VFD_CURSOR_LINE);
//  }
//
//  void setCursorPosition(int pos) {
//    sendCommand(pos);
//  }
//
  void reset() {
    sendCommand(VFD_CMD_RESET);
  }

  void sendCustomByte(int byte) {
    sendData(byte);
  }

  /**
   * Slot 1..6, row 1 (top) or 2 (bottom)
  **/
  void setText(int slot, int row, String text) {
    Serial.printf("VFD row %i slot %i: '%s' ", row, slot, text.c_str());
//    setCursorPosition(12);
    sendData(text);
    Serial.println("ok");
  }
};

#endif