#include <Arduino.h>
#include <string>

#include "Util.h"

int Util::ensureWithinRange(char n, char min, char max) {
	// Keep text on same line
	if (n < min) {
		return min;
	}
	if (n > max) {
		return max;
	}
	return n;
}

String Util::repeatChar(char filler, char length) {
	if (length < 1) {
		return "";
	} else if (length == 1) {
		return filler;
	}
	std::vector<char> buffer(length, filler);
	return String(buffer.data()).substring(0, length);
	// NOTE `substring` is needed because vector may have additional capacity which is returned by`.data()`
}

String Util::centreText(const String &text, char length) {
	const char PAD_CHAR = ' ';
	int padLen = (int) (length - text.length());
	if (padLen < 1) {
		return text; // Not truncated, so may actually be longer than requested
	}
	int padR = padLen / 2;
	int padL = padLen % 2 ? padR + 1 : padR; // Add one for odd-length text
	return Util::repeatChar(PAD_CHAR, padL) + text + Util::repeatChar(PAD_CHAR, padR);
}