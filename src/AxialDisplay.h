#ifndef AXIAL_DISPLAY_H
#define AXIAL_DISPLAY_H

#include <Arduino.h>
#include <vector>

#include "TeensyPin.h"

/**
 * Display driver for NEC FC40X2EA-AB
 * (based on datasheets for FC20X1SA and FC20X2JA)
**/
class AxialDisplay {
private:

	class DisplaySlot {
		char _pos;
		char _width;
	public:
		DisplaySlot();
		DisplaySlot(char pos, char width);
		char getPos() const;
		char getWidth() const;
	};

	// Use Teensy UART, see: https://www.pjrc.com/teensy/td_uart.html
	static const int  VFD_BAUD = 9600; // Match hardware setting on VFD daughter board
	static const char PIN_BUSY = 25; // The Teensy pin connected to the VFD daughter board's BUSY signal

	static const char VFD_CMD_RESET = 0xFF; // clears display and memory, sets power on condition
	static const char VFD_BS = 0x08; // BS "cursor position shifts one character to the left (unless at edge)"
	static const char VFD_HT = 0x09; // HT "cursor position shifts one character to the right (unless at edge)"
	static const char VFD_LF = 0x0a; // LF "all characters cleared, position remains"
	static const char VFD_CLEAR = 0x0c; // Removes all text and moves cursor to top left (datasheet says "clears display and memory, sets power on condition" but this is not the case)
	static const char VFD_CR = 0x0d; // CR "cursor position shifts to the left end"
	static const char VFD_WRAP = 0x11; // DC1 "cursor shifts right after printing" (wrap when at edge)
	static const char VFD_SCROLL = 0x12; // DC2 "cursor shifts right after printing" (marquee when at edge)
	static const char VFD_CURSOR_SHOW = 0x13; // DC3 "cursor turns on"
	static const char VFD_CURSOR_HIDE = 0x14; // DC4 "cursor turns off"
	static const char VFD_CURSOR_BLINK = 0x15; // DC5 "cursor turns on and blinks"
	static const char VFD_CURSOR_LINE = 0x16; // CM1 "no action", but in reality underscore cursor
	static const char VFD_CURSOR_BLOCK = 0x17; // CM2 "cursor position lit", ie. solid block cursor
	static const char VFD_CURSOR_INVERT = 0x18; // CM3 "cursor position lit in reverse", ie. inverted block cursor
	// SB registers a user character. Seems to require parallel (mentions use of "D0..D4" and saying "D5..D7 are invalid")
	static const char VFD_ESC = 0x1b; // ESC "the cursor position may be defined by one byte after the ESC data"

	InputPin pinBusy = InputPin(PIN_BUSY, HIGH);
	OutputPin pinA0 = OutputPin(12, LOW);
	OutputPin pinBL = OutputPin(14, LOW);
	// pinCS is always LOW (hardwired to GND on the back of the VFD board; only needed when multiplexing several VFDs)

	int currentCursorPosition; // 1-based; a 2x40 display has 80 positions

	char numCharsPerRow = 0;
	char numRows = 0;
	char numCharsTotal = 0;
	std::vector<DisplaySlot> slots;
	// TODO Add doc comments to all of these methods
	void updateCursorPosition(char byte);
	void sendData(char byte);
	void sendData(const String &text);
	void sendCommand(char byte);
	void moveBy(int adjustment);
	void moveTo(char row, char col);

public:

	/**
	 * Initialises the instance (passive constructor).
	 * @param newNumRows the number of rows of characters
	 * @param newNumCharsPerRow the number of characters each row has
	 * @param newNumSlots the number of slots
	 * @param slotDims a two-dimensional array specifying the centre and width of each slot
	**/
	void init(char newNumRows, char newNumCharsPerRow, char newNumSlots, char slotDims[][2]);
	// TODO Add doc comments to all of these methods
	char getNumRows() const;
	char getNumCharsPerRow() const;
	char getNumSlots() const;
	void on();
	void off();
	void cursorShow();
	void cursorHide();
	void cursorBlink();
	void cursorLine();
	void cursorBlock();
	void cursorInvertedBlock();
	/**
	 * Sets a given brightness level.
	 * 4=most bright (100%)
	 * 1=least bright (25%)
	 * 0=off
	**/
	void brightness(char level);
	/**
	 * Fades in the display from off to full brightness.
	**/
	void fadeIn(unsigned int duration);
	/**
	 * Fades out the display from full brightness to off.
	**/
	void fadeOut(unsigned int duration);
	/**
	 * Clears the display and resets the cursor position.
	**/
	void clear();
	/**
	 * Clears the display and resets it to a known state.
	**/
	void reset();
	/**
	 * Fills the entire display with the given character.
	 * @param filler
	**/
	void fill(char filler);
	/**
	 * Caution! Using this to send visible characters is going to mess up positioning!
	**/
	void sendRawByte(char byte);
	void setTextAt(char row, char col, const String &text);
	void setTextCentredAt(char row, char centre, const String &text);
	/**
	 * Note: Does not truncate overlong strings (will overflow adjacent slots).
	 * Note: `slot` is 0-based.
	**/
	void setSlotText(char row, char slot, const String &text);
};

#endif