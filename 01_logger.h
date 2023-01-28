#ifndef UTIL_LOGGER
#define UTIL_LOGGER

#include <Arduino.h>
#include <vector>
#include <string.h>

class Logger {

private:

  int indent = 0;
  std::vector<String> section;

  void startSerial(bool waitForSerial) {
    Serial.begin(0);  // Will always be USB speed
    if (waitForSerial && !Serial)
      while (!Serial)
        ; // wait for serial port to connect
  }

  void headerPush(const char *header) {
    section.push_back(String(header));
  }

  String headerPop() {
    String& item = section.at(indent);
    String header = String(item); // Copy because pop_back is going to destroy the original
    section.pop_back();
    return header;
  }

  void logAtIndent(const char *msg, int _indent) {
    for (int i=0; i<_indent; i++) Serial.print("  ");
    Serial.print(msg);
  }

public:

  static const bool WAIT_UNTIL_READY = true;
  static const bool IMMEDIATE = false;

  Logger(bool waitForSerial) {
    startSerial(waitForSerial);
  }

  void log() {
    log("\n");
  }

  void logln(char msg) {
    char arr[1] = {msg};
    log(arr);
    log("\n");
  }

  void logln(const char *msg) {
    log(msg);
    log("\n");
  }

  void log(const char *msg) {
    logAtIndent(msg, indent);
  }

  void append(const char *msg) {
    logAtIndent(msg, 0);
  }

  void begin(const char *msg) {
    log(msg);
    append(":");
    log();
    headerPush(msg);
    indent++;
  }

  void end(const char *msg) {
    indent--;
    String header = headerPop();
    log(header.c_str());
    logAtIndent(" ok", 0);
    log();
  }
};

#endif