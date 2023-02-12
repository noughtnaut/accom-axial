#include "AxialDisplay.h"
#include "Util.h"

AxialDisplay::DisplaySlot::DisplaySlot() : _pos(0), _width(0) {
}

AxialDisplay::DisplaySlot::DisplaySlot(char pos, char width) : _pos(pos), _width(width) {
}

char AxialDisplay::DisplaySlot::getPos() const {
	return _pos;
}

char AxialDisplay::DisplaySlot::getWidth() const {
	return _width;
}

void AxialDisplay::init(char newNumRows, char newNumCharsPerRow, char newNumSlots,
						char slotDims[][2]) {
	currentCursorPosition = 1;
	numRows = newNumRows;
	numCharsPerRow = newNumCharsPerRow;
	numCharsTotal = numRows * numCharsPerRow;
	slots.clear();
	if (newNumSlots) {
		for (int i = 0; i < newNumSlots; ++i) {
			slots.emplace_back(slotDims[i][0], slotDims[i][1]);
		}
	} else {
		slots.emplace_back(numCharsPerRow / 2, numCharsPerRow); // A single full-width slot
	}
	slots.shrink_to_fit(); // Make `.size()` report accurately

	// Setup Teensy UART, see: https://www.pjrc.com/teensy/td_uart.html
	Serial1.setTX(PIN_SERIAL_TX);
	Serial1.attachCts(PIN_BUSY);
	Serial1.begin(VFD_BAUD, SERIAL_8N1);

	reset();

	setTextAt(1, 40, "~");

	Serial.println("Display is ready");
}

void AxialDisplay::updateCursorPosition(char byte) {
	// Update cursor position unless we're sending non-printing characters
	if (byte >= 0x20) { // We're sending printing characters
		currentCursorPosition++;
	} else { // Low-numbered characters can do all sorts of funky stuff
		switch (byte) {
			case VFD_BS:
				currentCursorPosition--;
				break;
			case VFD_HT:
				currentCursorPosition++;
				break;
			case VFD_LF:
				currentCursorPosition += (int) numCharsPerRow;
				break;
			case VFD_CLEAR:
				currentCursorPosition = 1;
				break;
			case VFD_CR: { // This bracket limits the scope of `currentRow` so that it does not leak into the `default` scope
				// For a 2x20 display: 1->1, 21-> 21, 17->1, 37->21
				char currentRow = 1 + currentCursorPosition / numCharsPerRow;
				currentCursorPosition = 1 + currentRow * numCharsPerRow;
			}
			default:
				// Do not update position
				break;
		}
	}
	// Normalise position within display
	while (currentCursorPosition < 1) currentCursorPosition += (int) numCharsTotal;
	while (currentCursorPosition > numCharsTotal) currentCursorPosition -= (int) numCharsTotal;
}

void AxialDisplay::sendData(char byte) {
	if (pinBusy.isActive()) {
		Serial.print("Waiting for VFD to become ready ... ");
		while (pinBusy.isActive());  // wait for VFD to become ready
		Serial.println("ok");
	}
	Serial1.write(byte);
	updateCursorPosition(byte); // Update cursor position unless we're sending non-printing characters
}

void AxialDisplay::sendData(const String &text) {
	if (pinBusy.isActive()) {
		Serial.print("Waiting for VFD to become ready ... ");
		while (pinBusy.isActive());  // wait for VFD to become ready
		Serial.println("ok");
	}
	Serial1.print(text.c_str());
	currentCursorPosition += (int) text.length(); // TODO This assumes no non-printing characters
}

void AxialDisplay::sendCommand(char byte) {
	pinA0.on();
	sendData(byte);
	pinA0.off();
}

void AxialDisplay::moveBy(int adjustment) {
	// Normalise position within display
	while (adjustment < 0) adjustment += (int) numCharsTotal;
	while (adjustment > numCharsTotal) adjustment -= (int) numCharsTotal;
	if (!adjustment) {
		return;
	}

	// Move in the shortest direction, possibly by exploiting wrapping
	bool goForward = true;
	if (adjustment > numCharsTotal / 2) {
		adjustment = (int) numCharsTotal - adjustment;
		goForward = false;
	}

	// Move the cursor by the required adjustment: use HT to go right, or BS to go left)
	// TODO Make use of LF and CR to jump by halves and quarters
	char charToSend = goForward ? VFD_HT : VFD_BS;
	for (int m = 0; m < adjustment; m++) { // TODO Take wrapping mode into account
		sendData(charToSend);
	}
}

void AxialDisplay::moveTo(char row, char col) {
//	if (Util::isWithinRange(row, 1, getNumRows())
//		&& Util::isWithinRange(col, 1, getNumCharsPerRow())
//			) {
	// TODO Optimise to use direct position instead of relative
	// TODO Optimise to use VFD_CR to jump by quarter rows
	int desiredPos = (col + (row - 1) * numCharsPerRow) % numCharsTotal;
	int adjustment = desiredPos - currentCursorPosition;
	if (adjustment) {
		moveBy(adjustment);
	}
//	} else {
//		Serial.printf("Can't move to pos (%i, %i)", row, col);
//	}
}

char AxialDisplay::getNumRows() const {
	return numRows;
}

char AxialDisplay::getNumCharsPerRow() const {
	return numCharsPerRow;
}

char AxialDisplay::getNumSlots() const {
	return slots.size();
}

void AxialDisplay::on() {
	pinBL.off();
}

void AxialDisplay::off() {
	pinBL.on();
}

void AxialDisplay::cursorShow() {
	sendData(VFD_CURSOR_SHOW);
}

void AxialDisplay::cursorHide() {
	sendData(VFD_CURSOR_HIDE);
}

void AxialDisplay::cursorBlink() {
	sendData(VFD_CURSOR_BLINK);
}

void AxialDisplay::cursorLine() {
	sendData(VFD_CURSOR_LINE);
}

void AxialDisplay::cursorBlock() {
	sendData(VFD_CURSOR_BLOCK);
}

void AxialDisplay::cursorInvertedBlock() {
	sendData(VFD_CURSOR_INVERT);
}

void AxialDisplay::brightness(char level) {
	switch (level) {
		default: // Full brightness
			sendData(1);
			on();
			break;
		case 4: // DIM1 = full brightness
		case 3: // DIM2 =  75% brightness
		case 2: // DIM3 =  50% brightness
		case 1: // DIM4 =  25% brightness
			sendData(5 - level);
			on();
			break;
		case 0: // Turn display off
			off();
	}
}

void AxialDisplay::fadeIn(unsigned int duration) {
	// Assumes starting at zero brightness
	brightness(1);
	for (int level = 2; level <= 4; level++) {
		delay(duration / 4);
		brightness(level);
	}
	delay(duration / 4);
}

void AxialDisplay::fadeOut(unsigned int duration) {
	// Assumes starting at full brightness
	for (int level = 3; level >= 1; level--) {
		brightness(level);
		delay(duration / 4);
	}
	off();
	delay(duration / 4);
}

void AxialDisplay::clear() {
	sendData(VFD_CLEAR);
	delay(10);
}

void AxialDisplay::reset() {
//    sendCommand(VFD_CMD_RESET); // FIXME Requires parallel data?
	cursorHide(); // Factory default power on condition is underscore cursor
	clear();
	sendData(VFD_WRAP); // This is important for `updateCursorPosition()` to work right
	brightness(100); // Just to demo that "level>3" will default to "full brightness"
}

void AxialDisplay::fill(char filler) {
	setTextAt(1, 1, Util::repeatChar(filler, numCharsTotal));
}

void AxialDisplay::sendRawByte(char byte) {
	// Caution! Using this to send visible characters is going to mess up positioning!
	sendData(byte);
}

void AxialDisplay::setTextAt(char row, char col, const String &text) {
//	Serial.printf("setTextAt(%i, %i, '%s')\n", row, col, text.c_str());
	moveTo(row, col);
	sendData(text);
}

void AxialDisplay::setTextCentredAt(char row, char centre, const String &text) {
//	Serial.printf("setTextCentredAt(%i, %i, '%s')\n", row, centre, text.c_str());
	char textLen = (char) text.length();
	char halfLen = (textLen + 1) / 2; // +1 causes odd-length strings to  round up (half-assed right-alignment)
	char col = centre - halfLen + 1;
	char lastOkPos = numCharsPerRow - textLen+1;
	col = Util::ensureWithinRange(col, 1, lastOkPos);
	setTextAt(row, col, text);
}

/**
 * Note: Does not truncate overlong strings (will overflow adjacent slots).
 * Note: `slot` is 0-based.
**/
void AxialDisplay::setSlotText(char row, char slot, const String &text) {
	DisplaySlot &displaySlot = slots.at(Util::ensureWithinRange(slot, 0, getNumSlots() - 1));
	char centre = displaySlot.getPos();

	// Expand text to erase existing slot text
	String maskedText = Util::centreText(text, displaySlot.getWidth());
	// Set new slot text
	setTextCentredAt(row, centre, maskedText);
	// Alternatively, with boundary clipping: setTextCentredAt(row, col, text.substring(0, numCharsPerSlot));
}