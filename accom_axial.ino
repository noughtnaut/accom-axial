#include <Arduino.h>

/**
 * This code is a prototype. It is neither super pretty nor compliant with C++ best practices.
**/

#ifndef ACCOM_AXIAL
#define ACCOM_AXIAL

#include "01_logger.h"
#include "02_heartbeat.h"
// #include "04_keyboard.h"
#include "06_display.h"

void loop() {
//    scanKeyboard(); // TODO: Move loop to thread in keyboard module (as for the heartbeat), but there it seems to randomly just stop?
}

void setup() {
  Logger logger = Logger(Logger::WAIT_UNTIL_READY);
  logger.logln("--- Accom Axial ---");
  logger.begin("initialise");

  setupTeensyHeartbeat(logger);
//   setupStorage(logger);
//   setupConfig(logger);
//   setupKeyboard(logger);
//   setupDisplay(logger);

  logger.end("ok");
}

#endif