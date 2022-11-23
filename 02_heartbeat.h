#ifndef ACCOM_HEARTBEAT
#define ACCOM_HEARTBEAT

#include <Arduino.h>
#include <TeensyThreads.h>
// Other includes must be listed in the main INO file

#include "01_pin.h"

void teensyHeartbeat() {
  bool colour = true;
  const int DELAY_MILLIS = 1250;
  Pin pinLed(LED_BUILTIN, OUTPUT, HIGH, HIGH);  // Heartbeat LED

  while (true) {
//    Serial.println(colour ? "♥" : "♡");
    pinLed.setActive(colour);
    threads.delay(DELAY_MILLIS);
    colour = !colour;
    threads.yield();
  }
}

// Blink the Teensy on-board LED
void setupTeensyHeartbeat() {
  threads.addThread(teensyHeartbeat);
}

#endif