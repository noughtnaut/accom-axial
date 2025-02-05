#ifndef ACCOM_AXIAL
#define ACCOM_AXIAL

/**
 * This code is a prototype. It is neither super pretty nor compliant with C++ best practices.
 *
 * WORKS:
 *   keyboard (all non-LKS keys, and sending "normal" keystrokes via USB)
 *   VFD
 * UNSURE:
 *   reading LKS (lighted key switches)
 * LATEST:
 * NOT YET IMPLEMENTED:
 *   lighting LKS (lighted key switches)
 *   rotary dials
 *   jog wheel
 *   track ball
**/

#include <Arduino.h>

#include "01_pin.h"
#include "01_logger.h"
#include "02_heartbeat.h"
#include "03_keymap.h"
#include "04_keyboard.h"
#include "06_display.h"

void setup() {
	Pin::getLed().blink(50,50);
	logger.enable();
	logger.logln("--- Accom Axial ---");
	logger.logln("version: accom_axial");
	logger.begin("initialise");
  //setupTeensyHeartbeat(logger);
// TODO setupStorage(logger);
// TODO setupConfig(logger);
   setupKeyboard();
   setupDisplay();

  logger.end("initialised ok");
  Pin::getLed().blink(0,500);
  Pin::getLed().blink(50,50);
  Pin::getLed().blink(50,50);
  Pin::getLed().blink(50,200);
  logger.logln("Do something funny! (Try pressing SYSTM, this should bring up the Start menu on your pc)");

  //doVfdDemoStuff();
}

bool started = false;

void loop() {
  if (!started) {
    logger.logln("---");
    logger.logln("main loop started");
    started = true;
  }
	scanKeyboard(); // TODO: Move loop to thread in keyboard module (as for the heartbeat), but there it seems to randomly just stop?
// TODO scan trackball, rotary dials, jog wheel
/**/
}

#endif