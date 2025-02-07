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
  for (int i=0; i<4; i++) {
    vfd.fill(Vfd::CHAR_BLOCK_FULL);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_6);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_5);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_4);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_3);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_2);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_1);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_2);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_3);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_4);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_5);
    vfd.fill(Vfd::CHAR_BLOCK_LOWER_6);
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
//  delay(500);
//  vfd.fill(0xfd);
//  delay(500);
//  vfd.clear();
}

void slots() {
  vfd.clear();
  // Show slots
  // Should print:
  // <--1->       <--3-->       <--5-->
  //       <--2-->       <--4-->       <--6->
  // Rows and slots are 1-based
  // Might also use Vfd::ROW_TOP and Vfd::ROW_BOTTOM
  // Note: with 40 chars and 6 slots, outermost slots only get 6 chars
  vfd.setSlotText(1, 1, "<--1->");
  vfd.setSlotText(2, 2, "<--2-->");
  vfd.setSlotText(1, 3, "<--3-->");
  vfd.setSlotText(2, 4, "<--4-->");
  vfd.setSlotText(1, 5, "<--5-->");
  vfd.setSlotText(2, 6, "<--6->");
  delay(1500);
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
      //Debug for experimenting with parallel comms
      //vfd.sendRawByte(0x66);
      // 0x48 = "H" = 0001 0010
      // 0x66 = "f" = 0110 0110
      delay(100);
    }
  }
}

void doVfdDemoStuff() {
  logger.begin("vfd demo");
  //vfd.cursorInvertedBlock(); // does not imply making it visible
  vfd.cursorLine(); // does not imply making it visible
  vfd.cursorBlink(); // implies making it visible
  vfd.cursorShow();
  //delay(1500);
  //blockFill();
  //delay(1500);
  //propeller();
  //delay(1500);
  //helloWorld();
  //delay(1500);
  //blockBuild();
  //delay(1500);
  //charByCode();
  //delay(1500);
  slots();
  //delay(1500);
  //bigDamnHeroes();
  //delay(1500);
  //endlessMarquee(); // Note: will cause demo to not end
  logger.end("vfd demo done");
}

void setupDisplay() {
  logger.begin("init display");
  vfd = Vfd(2, 40, 6);
  logger.end();
}

#endif
