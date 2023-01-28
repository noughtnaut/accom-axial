#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
#include <string.h>

#include "05_vfd.h"

Logger logger;
Vfd vfd;

int setupDisplay() {
  Serial.print("display:");
  vfd = Vfd(2, 40, 6);
  vfd.cursorHide(); // Prettiest this way

  // Should print:
  // <.................................Hello,
  // World!.................................>
  vfd.fill('.');
  vfd.setTextAt(1, 0, "World!");
  vfd.setTextAt(0, 34, "Hello,");
  vfd.setTextAt(0, 0, "<");
  vfd.setTextAt(1, 39, ">");
  delay(2000);

  // Should print:
  // Slot1v Slot2v Slot3v Slot4v Slot5v Slot6
  // Slot1k Slot2k Slot3k Slot4k Slot5k Slot6
  // Note: rightmost slot only gets 5 chars (deal with it)
  vfd.clear();
  char buffer[10] = {0}; // Must be big enough
  for (int row=vfd.getHeight()-1; row>=0; row--) {
    for (int slot=vfd.getNumSlots()-1; slot>=0; slot--) {
//      Serial.printf("Row %i Slot %i: ", row, slot);
      sprintf(buffer, "Slot%i%s", 1+slot, row?"k":"v");
      vfd.setSlotText(row, slot, String(buffer));
    }
  }
  delay(1000);
  vfd.setSlotText(0, 3, "<-overflow->");
  vfd.setSlotText(1, 2, "<--spoils neighbours-->");
  delay(1000);
  vfd.fadeOut(800);

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

  // Blink the VFD thrice so we can see we're done
  // (even without a serial monitor)
  for (int i=0; i < 3; i++) {
    delay(300);
    vfd.off();
    delay(300);
    vfd.on();
  }

  Serial.println("ok");
  return 0;
}
#endif