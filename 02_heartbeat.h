#ifndef ACCOM_HEARTBEAT
#define ACCOM_HEARTBEAT

#include <Arduino.h>
#include <TeensyThreads.h>

#include "01_pin.h"

void teensyHeartbeat() {
  bool colour = true;
  const int DELAY_MILLIS = 1250;
  Pin pinLed(LED_BUILTIN, OUTPUT, HIGH, true);  // Heartbeat LED

  while (true) {
    pinLed.setActive(true);
//    Serial.println(colour ? "♥" : "♡");
    threads.delay(DELAY_MILLIS);
    pinLed.setActive(false);
    threads.delay(DELAY_MILLIS/8);
    colour = !colour;
    threads.yield();
  }
}

// Blink the Teensy on-board LED
void setupTeensyHeartbeat() {
  Serial.print("heartbeat:");
  threads.addThread(teensyHeartbeat);
  Serial.println("ok");
}

#endif