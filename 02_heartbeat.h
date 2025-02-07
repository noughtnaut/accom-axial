#ifndef ACCOM_HEARTBEAT
#define ACCOM_HEARTBEAT

#include <Arduino.h>
#include <TeensyThreads.h>

#include "01_pin.h"

void teensyHeartbeat() {
  bool blackheart = true;
  const int DELAY_MILLIS = 1250;
  const int BLINK_RATIO = 12;
  Pin pinLed(LED_BUILTIN, OUTPUT, HIGH, true);

  while (true) {
//    Serial.println(blackheart ? "♥" : "♡");
    pinLed.on();
    threads.delay(DELAY_MILLIS);
    pinLed.off();
    threads.delay(DELAY_MILLIS/BLINK_RATIO);
    blackheart = !blackheart;
    threads.yield();
  }
}

// Blink the Teensy on-board LED
void setupTeensyHeartbeat(Logger logger) {
  logger.begin("init heartbeat");
  threads.addThread(teensyHeartbeat);
  logger.end();
}

#endif