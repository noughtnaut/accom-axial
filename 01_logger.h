#ifndef LOGGER
#define LOGGER

#include <Arduino.h>
#include <vector>
#include <string.h>

#include "01_pin.h"

class Logger {

Pin pinLed = Pin::getLed(); // For debugging, to indicate various operations

private:

  int indent = 0;
  bool isEnabled = false;

  void startSerial() {
    pinLed.on();
    Serial.begin(0);  // Will always be USB speed
    pinLed.blink(50,50);
  }

  void logAtIndent(const char* msg, int _indent) {
    if (!isEnabled) return;
    for (int i=0; i<_indent; i++) Serial.print("  ");
    Serial.print(msg);
  }

public:

  Logger() {
    startSerial();
  }

  void enable() {
    isEnabled = true;
  }

  void diable() {
    isEnabled = false;
  }

  void log() {
    log("\n");
  }

  void logln(char msg) {
    char arr[1] = {msg};
    logln(arr);
  }

  void logln(const char* msg) {
    log(msg);
    log();
  }

  void log(String msg) {
    log(msg.c_str());
  }

  void log(const char* msg) {
    logAtIndent(msg, indent);
  }

  void appendln(const char* msg) {
    append(msg);
    log();
  }

  void append(const char* msg) {
    logAtIndent(msg, 0);
  }

  void begin(const char* msg) {
    logln(msg);
    indent++;
  }

  void end() {
    end("ok");
  }

  void end(const char* msg) {
    indent--;
    logln(msg);
  }
};

Logger logger = Logger();

#endif