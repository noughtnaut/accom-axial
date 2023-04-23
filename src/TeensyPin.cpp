#include <WProgram.h>

#include "TeensyPin.h"

TeensyPin::TeensyPin(char mode, char number, char activeState) : _number(number), _activeState(activeState) {
	pinMode(number, mode);
}

char TeensyPin::getNumber() const {
	return _number;
}

char TeensyPin::getActiveState() const {
	return _activeState;
}

bool TeensyPin::isActive() const {
	return TeensyPin::getActiveState() == digitalReadFast(_number);
}

InputPin::InputPin(char number, char activeState) : TeensyPin(INPUT_PULLUP, number, activeState) {
}

OutputPin::OutputPin(char number, char activeState) : TeensyPin(OUTPUT_OPENDRAIN, number, activeState) {
}

OutputPin::OutputPin(char number, char activeState, bool initiallyActive) : TeensyPin(OUTPUT_OPENDRAIN, number, activeState) {
	setActive(initiallyActive);
}

OutputPin OutputPin::getLED() {
	return OutputPin(LED_BUILTIN, HIGH); // NOLINT(modernize-return-braced-init-list)
}

void OutputPin::setState(char state) const {
	digitalWriteFast(getNumber(), state);
}

void OutputPin::on() const {
	getActiveState() == LOW ? setState(LOW) : setState(HIGH);
}

void OutputPin::off() const {
	getActiveState() == LOW ? setState(HIGH) : setState(LOW);
}

void OutputPin::setActive(bool activate) const {
	if (activate) on();
	else off();
}

void OutputPin::toggle() const {
	if (isActive()) off();
	else on();
}

void OutputPin::blink(unsigned int onDuration, unsigned int offDuration) const {
	OutputPin::on();
	delay(onDuration);
	OutputPin::off();
	delay(offDuration);
}

void OutputPin::occlude(unsigned int offDuration, unsigned int onDuration) const {
	OutputPin::off();
	delay(offDuration);
	OutputPin::on();
	delay(onDuration);
}