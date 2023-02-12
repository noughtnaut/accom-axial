#include <map>

#include "Stopwatch.h"

unsigned long Stopwatch::lap(const String &name, bool verbose, bool deleteAfter) {
	unsigned long now = millis();
	auto it = slots.find(name);
	if (it != slots.end()) {
		unsigned long duration = now - it->second;
		if (verbose) {
			Serial.printf("%s took %.3fs", name.c_str(), duration/(float)1000); // NOLINT(cppcoreguidelines-narrowing-conversions)
		}
		if (deleteAfter) {
			slots.erase(it);
			Serial.println(".");
		} else {
			Serial.println(" so far...");
		}
		return duration;
	}
	if (verbose) {
		Serial.printf("'%s' has no timer\n", name.c_str());
	}
	return -1;
}

void Stopwatch::start(const String &name) {
	slots[name] = millis();
}

unsigned long Stopwatch::lap(const String &name, bool verbose) {
	return lap(name, verbose, false);
}

unsigned long Stopwatch::stop(const String &name, bool verbose) {
	return lap(name, verbose, true);
}