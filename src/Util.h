#ifndef TEENSY_AXIAL_UTIL_H
#define TEENSY_AXIAL_UTIL_H

#include <vector>

class Util {
public:
	/**
	 * Adjusts number to be within a given range.
	 * @param n
	 * @param min
	 * @param max
	 * @return TRUE if the given number is within [min, max] (inclusive)
	 */
	static int ensureWithinRange(char n, char min, char max);
	/**
	 * Generates a string of identical characters
	 * @param filler the character to fill it with
	 * @param length how long you need it to be
	 * @return a String
	 */
	static String repeatChar(char filler, char length);
	/**
	 * Returns a new String with additional padding to centre the given text
	 * @param text the text to centre
	 * @param length how long you need it to be
	 */
	static String centreText(const String &text, char length);
};

#endif //TEENSY_AXIAL_UTIL_H