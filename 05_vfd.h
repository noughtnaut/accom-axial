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
const int VFD_DC5 = 0x15; // col 1, row 5 -- cursor blinks
const int VFD_CM2 = 0x17; // col 1, row 7 -- block cursor

// TODO The pin numbers below are for the wrong socket!
Pin pinBusy(51, HIGH); // BUSY (r27)
Pin pinWR(40, OUTPUT, HIGH, LOW);  // WR (r17)
Pin pinA0(41, OUTPUT, LOW, HIGH);  // A0 (r19)
Pin pinCS(53, OUTPUT, HIGH, LOW);  // CS (r23)
Pin pinBL(30, OUTPUT, LOW, HIGH);  // BL (r29), pull down to blank
Pin pinDS(32, OUTPUT, HIGH, LOW);  // RxS (r33) Serial data

class Vfd {

private:

/*
A  0100 0001
Y  0101 1001 = 0x59 = c5r9
Z  0101 1010
h  0110 1000
o  0110 1111
y  0111 1001
z  0111 1010

<= 1111 0000
>= 1111 0001
_| 1111 1100
£  1111 1000
~  1111 1110
|> 1110 1011
oo 1111 0101
§  1111 1001
!  0010 0001
~  1111 1110
*/

  void sendSerialByte(int byte) {
    // Ensure VFD is ready to receive data
    if (pinBusy.isActive()) {
      // Serial.print("Waiting for VFD to become ready ... ");
      while (pinBusy.isActive())
        ;  // wait for VFD to become ready
      // Serial.println("ok");
    }

    // Do not set "Begin transmission" signal
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
    // Do not set "End transmission" signal
  }

  void sendCommand(int byte) {
    // Begin transmission
    pinWR.setActive(true);
    delayMicroseconds(VFD_BAUD_DELAY_MICROS);

    // Send command
    pinA0.setActive(true);
    sendSerialByte(byte);

    // End transmission
    pinWR.setActive(false);
  }

  void sendData(int byte) {
    pinA0.setActive(false);
    sendSerialByte(byte);
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

  void setCursorBlink() {
    sendData(VFD_DC5);
  }

  void setCursorBlock() {
    sendData(VFD_CM2);
  }

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
    Serial.printf("VFD row %i slot %i: '%s'\n", row, slot, text.c_str());
//    sendData(text); // TODO Rework this to send a byte at a time
  }
};

#endif