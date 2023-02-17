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

OutputPin::OutputPin(char number, char activeState) : TeensyPin(OUTPUT, number, activeState) {
}

OutputPin OutputPin::getLED() {
	return OutputPin(LED_BUILTIN, HIGH); // NOLINT(modernize-return-braced-init-list)
}

void OutputPin::on() const {
	digitalWriteFast(getNumber(), getActiveState() == LOW ? LOW : HIGH);
}

void OutputPin::off() const {
	digitalWriteFast(getNumber(), getActiveState() == LOW ? HIGH : LOW);
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