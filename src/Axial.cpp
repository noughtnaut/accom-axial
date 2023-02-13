#include <Arduino.h>
#include <map>

#include "Axial.h"

void Axial::init() {
	init_display();
	init_keyboard();

	Serial.println("Accom Axial is ready");
}

void Axial::init_keyboard() {
	k.init();
}

void Axial::init_display() {
	// The display has 6 'slots', sections correlating to the dial knobs.
	// .________________________________________.
	// |X<-(1,1)                                | Display character coordinates.
	// |                               (2,40)->X|
	// .~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
	// .________________________________________.
	// |         1         2         3         4| Display character positions.
	// |1234567890123456789012345678901234567890|
	// .~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
	// .________________________________________.
	// |1234567      1234567       123456       | Slot placement and extents.
	// |       123456       1234567      1234567| Most slots have 7 chars; #2 and 5 have only 6 chars.
	// .~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
	//     ^4    ^10    ^17    ^24   ^30    ^37   Slot centre positions.
	const char NUM_SLOTS = 6;
	char slotDims[NUM_SLOTS][2] = {
			{ 4, 7},
			{10, 6},
			{17, 7},
			{24, 7},
			{30, 6},
			{37, 7}};
	d.init(2, 40, NUM_SLOTS, slotDims);
	showoff();
}

void Axial::showoff() {
	Serial.println("Showing off ...");
	// Showing off misc. display functions
	d.cursorBlink();
	d.cursorBlock();
	d.setTextAt(2, 35, "World!");
	d.setTextAt(2, 1, "<");
	d.setTextAt(1, 40, ">");
	d.setTextAt(1, 1, "Hello,");

	delay(2000);
	d.cursorHide();
	// Showing off full-display update speed
	for (int i = 0; i < 3; ++i) {
		d.fill('-');
		d.fill('/');
		d.fill('-');
		d.fill('\\');
		d.fill('|');
	}
	d.fill('X');

	delay(1000);
	// Showing off setting slot text
	d.setSlotText(1, 1, "Slot 1");	d.setSlotText(2, 1, "Val. 1");
	d.setSlotText(1, 2, "Slot 2");	d.setSlotText(2, 2, "Val. 2");
	d.setSlotText(1, 3, "Slot 3");	d.setSlotText(2, 3, "Val. 3");
	d.setSlotText(1, 4, "Slot 4");	d.setSlotText(2, 4, "Val. 4");
	d.setSlotText(1, 5, "Slot 5");	d.setSlotText(2, 5, "Val. 5");
	d.setSlotText(1, 6, "Slot 6");	d.setSlotText(2, 6, "Val. 6");

	delay(1000);
	// Showing off update speed for adjustment of slot value
	char buffer[20] = {0};
	for (int slot = 1; slot <= d.getNumSlots(); ++slot) {
		sprintf(buffer, "Slot %i", slot);
		d.setSlotText(1, slot, String(buffer));
		d.setSlotText(2, slot, "0%");
	}
	delay(500);
	for (int slot = 1; slot <= d.getNumSlots(); ++slot) {
		for (int percentage = 1; percentage < 101; ++percentage) {
			sprintf(buffer, "%i%%", percentage);
			d.setSlotText(2, slot, String(buffer));
		}
	}

	// Say goodbye
	delay(1000);
	d.fadeOut(800);
	d.clear();
	d.setTextCentredAt(2, 20, "is READY!");
	d.setTextCentredAt(1, 20, "Accom Axial");
	d.fadeIn(800);
	d.fadeOut(400);
	d.fadeIn(400);
	d.fadeOut(400);
	d.fadeIn(400);
	Serial.println("Showing off ... ok");
}