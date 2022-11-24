#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
// Other includes must be listed in the main INO file

#include "05_vfd.h"

Vfd vfd;

int setupDisplay() {
  Serial.print("display:");
  vfd = Vfd();
  vfd.reset();

  vfd.sendCustomByte(0b00000000u);
  // The above should not print anything,
  // or a slender up-arrow if the baud rate is too fast, because then
  // the first stop bit will be read as the last data bit.

  vfd.sendCustomByte(0b01011010u);
  // The above should print a capital 'Z' if the baud rate is correct,
  // even if the data is sent in the wrong order.

  vfd.sendCustomByte(0b01000101u);
  // The above should print a capital 'E' if the baud rate is correct
  // and the data is sent in the correct order.

  vfd.sendCustomByte(0x67); // 'g' = col 6 row 7 = 0b01100111u
  // The above should print a 'g' if the baud rate is correct
  // and the data is sent in the correct order.

  vfd.sendCustomByte(0b10000000u);
  // The above should print a slender up-arrow,
  // or not print anything if the baud rate is too slow, because then
  // the last data bit will be read as the first stop bit.

  // Blink the VFD so we can see we're done even without a serial monitor
  vfd.setBlank(true);
  delay(500);
  vfd.setBlank(false);
  delay(500);
  vfd.setBlank(true);
  delay(500);
  vfd.setBlank(false);
  Serial.println("ok");
  return 0;
}

#endif