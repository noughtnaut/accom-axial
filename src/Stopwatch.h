#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <map>

#include "Axial.h"

class Stopwatch {
	std::map<String, unsigned long> slots; // A list of stopwatches
	/**
	 * Reports elapsed time for the stopwatch with the given name, and optionally deletes the stopwatch.
	 * @param name the name of the thing you're measuring
	 * @param verbose whether to print to standard out in addition to returning the time
	 * @return the elapsed time in milliseconds. If no stopwatch of that name exists, returns -1.
	 */
	unsigned long lap(const String &name, bool verbose, bool deleteAfter);
public:
	/**
	 * Starts a new stopwatch with the given name.
	 * @param name the name of the thing you're measuring
	 */
	void start(const String &name);
	/**
	 * Reports elapsed time for the stopwatch with the given name.
	 * @param name the name of the thing you're measuring
	 * @param verbose whether to print to standard out in addition to returning the time
	 * @return the elapsed time in milliseconds. If no stopwatch of that name exists, returns -1.
	 */
	unsigned long lap(const String &name, bool verbose);
	/**
	 * Reports elapsed time for the stopwatch with the given name, and stops and deletes the stopwatch.
	 * @param name the name of the thing you're measuring
	 * @param verbose whether to print to standard out in addition to returning the time
	 * @return the elapsed time in milliseconds. If no stopwatch of that name exists, returns -1.
	 */
	unsigned long stop(const String &name, bool verbose);
	// TODO flush() should print all remaining timers and clear itself
};

#endif