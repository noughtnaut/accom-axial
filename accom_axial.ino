#include <Arduino.h>

#ifndef ACCOM_AXIAL
#define ACCOM_AXIAL

#include "02_heartbeat.h"
#include "04_keyboard.h"
#include "06_display.h"

void startSerial() {
  Serial.begin(0);  // Will always be USB speed
  while (!Serial)
    ;  // wait for serial port to connect // TODO Don't *require* serial for production use
  Serial.println();
}

void loop() {
// FIXME Disabled while testing VFD on the same socket
//  scanKeyboard(); // TODO: Move loop to keyboard module, but there it seems to randomly just stop?
}

void setup() {
  Serial.println("--- Accom Axial ---");
  Serial.println("initialising ...");

  startSerial();
  setupTeensyHeartbeat();
//  setupConfig();
//  sdfsSetup();
  setupKeyboard();
  setupDisplay();

  Serial.println("initialised ok");
}

#endif