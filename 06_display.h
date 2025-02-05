#ifndef ACCOM_DISPLAY
#define ACCOM_DISPLAY

#include <Arduino.h>
#include <string.h>

#include "01_logger.h"
#include "05_vfd.h"

Vfd vfd;

void blockFill() {
  vfd.fill(Vfd::CHAR_BLOCK_FULL);
	delay(5000);
  vfd.fill(Vfd::CHAR_BLOCK_HATCHED);
	delay(500);
  vfd.clear();
	delay(500);
}

void helloWorld() {
	// Show display boundaries:
	// Hello,         Accom Axial             >
	// <             Control Panel       World!
  vfd.clear();
  vfd.setTextCentredAt(1, 21, "Accom Axial");
  vfd.setTextCentredAt(2, 21, "Control Panel");
	vfd.setTextAt(2, 35, "World!");
	vfd.setTextAt(2, 1, "<");
	vfd.setTextAt(1, 1, "Hello,");
	vfd.setTextAt(1, 40, ">");
	//delay(1500);
  //vfd.fadeOut(1500);
}

void propeller() {
	delay(500);
  for (int i=0; i<4; i++) {
    // propeller
    vfd.fill('\\');
    vfd.fill('|');
    vfd.fill('/');
    vfd.fill('-');
  }
}

void blockBuild() {
  // block builds up from underscore to full block and down again
	delay(500);
  for (int i=0; i<4; i++) {
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_1);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_2);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_3);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_4);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_5);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_6);
    vfd.fill(Vfd::CHAR_BLOCK_FULL);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_6);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_5);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_4);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_3);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_2);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_1);
  }

  // block builds up from right
	delay(500);
  for (int i=0; i<4; i++) {
    vfd.fill(Vfd::CHAR_BLOCK_FULL);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_4);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_3);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_2);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_1);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_2);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_3);
    vfd.fill(Vfd::CHAR_BLOCK_RIGHT_4);
  }
}

void charByCode() {
// Display specific character by hex code
//	delay(500);
//  vfd.fill(0xfd);
//	delay(500);
//  vfd.clear();
}

void slots() {
  // Should print:
  // Slot1v Slot2v Slot3v Slot4v Slot5v Slot6
  // Slot1k Slot2k Slot3k Slot4k Slot5k Slot6
  // Note: rightmost slot only gets 5 chars (deal with it)
  delay(3000);
  vfd.clear();
  vfd.setSlotText(1, 4, "<-1234->");
  vfd.setSlotText(1, 2, "Header");
  vfd.setSlotText(2, 2, "Value");

  // Show slots
  delay(1000);
  vfd.clear();
  vfd.setSlotText(1, 5, "Slot5k"); // This will wrap around and cause slot 1v to say "klot1v"
  vfd.setSlotText(2, 0, "Slot1v");
  delay(500);
  vfd.setSlotText(1, 4, "Slot4k");
  vfd.setSlotText(2, 1, "Slot1v");
  delay(500);
  vfd.setSlotText(1, 3, "Slot3k");
  vfd.setSlotText(2, 2, "Slot2v");
  delay(500);
  vfd.setSlotText(1, 2, "Slot2k");
  vfd.setSlotText(2, 3, "Slot3v");
  delay(500);
  vfd.setSlotText(1, 1, "Slot1k");
  vfd.setSlotText(2, 4, "Slot4v");
  delay(500);
  vfd.setSlotText(2, 5, "Slot5v"); // This will wrap around and cause slot 0k to say "vlot0k"
  vfd.setSlotText(1, 0, "Slot0k");
  delay(2500);

  vfd.setSlotText(1, 3, "<-overflow->");
  vfd.setSlotText(2, 2, "<--spoils neighbours-->");
  delay(3000);
  vfd.fadeOut(1000);
}

void bigDamnHeroes() {
  // Shiny!
  vfd.clear();
  vfd.off();
  vfd.setTextCentredAt(Vfd::ROW_TOP, 25, "Big damn heroes, sir.");
  vfd.setTextCentredAt(Vfd::ROW_BOTTOM, 15, "Ain't we just.");
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
}

void endlessMarquee() {
  // Endless marquee
  vfd.cursorLine(); // does not imply making it visible
  vfd.cursorShow();
  vfd.moveTo(1,1);
  vfd.brightness(1); // implies vfd.on()
  while (true) {
    for (byte c=0x20; c < 0xff; c++) {
      vfd.sendRawByte(c);
      delay(200);
    }
  }
  vfd.clear();
}

void doVfdDemoStuff() {
	logger.begin("vfd demo");

  //vfd.cursorInvertedBlock(); // does not imply making it visible
  //vfd.cursorBlink(); // implies making it visible
  vfd.clear();
  //blockFill();
  propeller();
  helloWorld();
  //blockBuild();
  //charByCode();
  //slots();
  //bigDamnHeroes();
  endlessMarquee(); // Note: will cause demo to not end
	logger.end("vfd demo done");
}

void setupDisplay() {
  logger.begin("init display");
	vfd = Vfd(2, 40, 6);
  logger.end();
}

#endif