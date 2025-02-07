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
  char buffer[12]; // for integer-to-string conversion

	void initSerial() {
		pinLed.on();
		Serial.begin(0);  // Will always be USB speed
		pinLed.blink(50,50);
    Serial.print("[Logger initialised]\n");
	}

	void logAtIndent(const char* msg, int indent) {
		if (!isEnabled) return;
		for (int i=0; i<indent; i++) Serial.print("  ");
		Serial.print(msg);
	}

	const char* iToStr(int i) {
    sprintf(buffer, "%d", i);
    return buffer;
	}

public:

	Logger() {
		initSerial();
	}

	void enable() {
		isEnabled = true;
	}

	void disable() {
		isEnabled = false;
	}

  //Managing blocks (indentation)
	void begin(const char* msg) {
		logln(msg);
		indent++;
	}

	void end() {
		end("ok");
	}

	void end(const char* msg) {
		if (indent>0) indent--;
		logln(msg);
	}

  //Logging (with block indentation, and line feed)
	void logln(int i) {
    logln(iToStr(i));
	}

	void logln(const char* msg) {
		logAtIndent(msg, indent);
		append("\n");
	}

  //Logging (with block indentation, no line feed for continuation)
	void log(int i) {
    log(iToStr(i));
	}

	void log(const char* msg) {
		logAtIndent(msg, indent);
	}

  // Appending (without block indentation, and line feed)
	void appendln(int i) {
    appendln(iToStr(i));
	}

	void appendln(const char* msg) {
		logAtIndent(msg, 0);
		append("\n");
	}

  // Appending (without block indentation, no line feed for continuation)
	void append(int i) {
    append(iToStr(i));
	}

	void append(const char* msg) {
		logAtIndent(msg, 0);
	}
};

Logger logger = Logger();

#endif