#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
// Other includes must be listed in the main INO file

#include "05_vfd.h"

Vfd vfd;

int setupDisplay() {
  Serial.print("display:");
  vfd = Vfd(2, 40, 6);
//  vfd.reset();

  // Paint all characters so we can tell skips from overwrites
//  for (int row=0; row < vfd.getHeight(); row++) {
//    for (int col=0; col < vfd.getWidth(); col++) {
//      vfd.sendCustomByte('.');
//    }
//  }
  // Should print:
  // <.................................Hello,
  // World!.................................>
  vfd.setTextAt(1, 0, "World!");
  vfd.setTextAt(0, 34, "Hello,");
  vfd.setTextAt(0, 0, "<");
  vfd.setTextAt(1, 39, ">");
  delay(1000);
  vfd.setTextAt(0, 18, "yay me");

/*
  char buffer[10] = {0}; // Must be big enough
  for (int row=vfd.getHeight()-1; row>=0; row--) {
    for (int slot=vfd.getNumSlots()-1; slot>=0; slot--) {
//      Serial.printf("Row %i Slot %i: ", row, slot);
      sprintf(buffer, "Slot%i%s", 1+slot, row?"k":"v");
      vfd.setSlotText(row, slot, String(buffer));
//      delay(1000);
    }
  }
*/

  delay(500);
//  vfd.cursorBlock();
//  vfd.cursorBlink();
//  delay(2000);
//  vfd.cursorHide();
//  delay(2000);
//  vfd.cursorShow();
//  delay(2000);
//  vfd.cursorMode(VFD_CURSOR_BLINK);
//  delay(500);
//  vfd.cursorMode(VFD_CURSOR_INVERT);
//  delay(500);
//  vfd.cursorMode(VFD_CURSOR_NORMAL);
//  delay(500);
//  vfd.cursorMode(VFD_CURSOR_LIGHT);
//  delay(500);

  // Blink the VFD so we can see we're done even without a serial monitor
  delay(300);
  vfd.off();
  delay(300);
  vfd.on();
  delay(300);
  vfd.off();
  delay(300);
  vfd.on();
  delay(300);
  vfd.off();
  delay(300);
  vfd.on();

  Serial.println("ok");
  return 0;
}

#endif