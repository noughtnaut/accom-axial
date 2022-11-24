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

  vfd.setText(1, 1, "Hello, ");
  vfd.setText(1, 2, "World!");

  vfd.setCursorBlock();
  vfd.setCursorBlink();
  delay(2000);
//  vfd.setCursorHide();
//  delay(2000);
//  vfd.setCursorShow();
//  delay(2000);
//  vfd.setCursorMode(VFD_CURSOR_BLINK);
//  delay(500);
//  vfd.setCursorMode(VFD_CURSOR_INVERT);
//  delay(500);
//  vfd.setCursorMode(VFD_CURSOR_NORMAL);
//  delay(500);
//  vfd.setCursorMode(VFD_CURSOR_LIGHT);
//  delay(500);

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