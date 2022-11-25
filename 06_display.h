#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
// Other includes must be listed in the main INO file

#include "05_vfd.h"

Vfd vfd;

int setupDisplay() {
  Serial.print("display:");
  vfd = Vfd(2, 40, 6);
  vfd.reset();

  // Paint all characters so we can tell skips from overwrites
  for (int row=0; row < vfd.getHeight(); row++) {
    for (int col=0; col < vfd.getWidth(); col++) {
      vfd.sendCustomByte('.');
    }
  }
  // Should print:
  // <.................................Hello,
  // World!.................................>
  vfd.setTextAt(1, 0, "World!");
  vfd.setTextAt(0, 34, "Hello,");
  vfd.setTextAt(0, 0, "<");
  vfd.setTextAt(1, 39, ">");

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