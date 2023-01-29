#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
#include <string.h>

#include "01_logger.h"
#include "05_vfd.h"

Vfd vfd;
bool isSetup = false;

void setupDisplay() {
//  logger.begin("display");
//  Pin pinLed = Pin::getLed(); // For debugging, to indicate various operations
  vfd = Vfd(2, 40, 6);

  // Should print:
  // Hello,.................................>
  // <.................................World!
  vfd.cursorInvertedBlock();
  vfd.cursorShow();
  delay(500);
//  vfd.cursorShow();
  vfd.fill('.');
  vfd.fill('*');
  vfd.fill('#');
  vfd.fill(0xef);
  vfd.fill(0x7f);
  vfd.fill(0x87);
  vfd.fill('.');
  vfd.fill('.');
  vfd.fill('*');
  vfd.fill('#');
  vfd.fill(0xef);
  vfd.fill(0x7f);
  vfd.fill(0x87);
  vfd.fill('.');
  vfd.fill('.');
  vfd.fill('*');
  vfd.fill('#');
  vfd.fill(0xef);
  vfd.fill(0x7f);
  vfd.fill(0x87);
  vfd.fill('.');
  vfd.fill('.');
  vfd.fill('*');
  vfd.fill('#');
  vfd.fill(0xef);
  vfd.fill(0x7f);
  vfd.fill(0x87);
  vfd.fill('.');

  logger.logln("vfd demo done"); /*
  delay(1000);
  vfd.setTextAt(2, 35, "World!");
  delay(1000);
  vfd.setTextAt(2, 1, "<");
  delay(1000);
  vfd.setTextAt(1, 1, "Hello,");
  delay(1000);
  vfd.setTextAt(1, 40, ">");
  vfd.cursorHide();

  // Should print:
  // Slot1v Slot2v Slot3v Slot4v Slot5v Slot6
  // Slot1k Slot2k Slot3k Slot4k Slot5k Slot6
  // Note: rightmost slot only gets 5 chars (deal with it)
  delay(3000);
  vfd.clear();
  vfd.setSlotText(1, 3, "<-1234->");
  vfd.setSlotText(1, 2, "Header");
  vfd.setSlotText(2, 2, "Value");
  char buffer[20] = {0}; // Must be big enough
  for (int row=vfd.getNumRows()-1; row>=0; row--) {
    for (int slot=vfd.getNumSlots()-1; slot>=0; slot--) {
      sprintf(buffer, "Row %i, Slot %i: ", row, slot);
      logger.logln(buffer);
      sprintf(buffer, "Slot%i%s", 1+slot, row?"k":"v");
      logger.logln(buffer);
      vfd.setSlotText(row, slot, "<-1234->");//String(buffer));
    }
  }
  delay(3000);
  vfd.setSlotText(0, 3, "<-overflow->");
  vfd.setSlotText(1, 2, "<--spoils neighbours-->");
  delay(3000);
  vfd.fadeOut(1000);

  // Shiny!
  vfd.clear();
  vfd.off();
  vfd.setTextCentredAt(0, 25, "Big damn heroes, sir.");
  vfd.setTextCentredAt(1, 15, "Ain't we just.");
  vfd.fadeIn(400);
  vfd.fadeOut(400);
  vfd.fadeIn(400);
  vfd.fadeOut(400);
  vfd.fadeIn(400);
  vfd.fadeOut(400);
  vfd.fadeIn(400);
  delay(500);
  vfd.fadeOut(800);

  // Blink the VFD thrice so we can see we're done even without a serial monitor
  logger.log("(blink)\n");
  for (int i=0; i < 3; i++) {
    delay(300);
    vfd.off();
    delay(300);
    vfd.on();
  }
*/
//  logger.end();
}

#endif