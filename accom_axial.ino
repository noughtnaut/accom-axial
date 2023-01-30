#ifndef ACCOM_AXIAL
#define ACCOM_AXIAL

/**
 * This code is a prototype. It is neither super pretty nor compliant with C++ best practices.
**/

#include <Arduino.h>

#include "01_pin.h"
#include "01_logger.h"
// #include "02_heartbeat.h"
// #include "04_keyboard.h"
#include "06_display.h"

void loop() {
//    scanKeyboard(); // TODO: Move loop to thread in keyboard module (as for the heartbeat), but there it seems to randomly just stop?
}

void setup() {
  Pin pinLed = Pin::getLed(); // For debugging, to indicate various operations
  pinLed.blink(50,50);
  logger.enable();
  logger.logln("--- Accom Axial ---");
  logger.begin("initialise");

//   setupTeensyHeartbeat(logger);
//   setupStorage(logger);
//   setupConfig(logger);
//   setupKeyboard(logger);
  setupDisplay();

  logger.end("initialised ok");
  pinLed.blink(0,500);
  pinLed.blink(50,50);
  pinLed.blink(50,50);
  pinLed.blink(500,0);
}

#endif